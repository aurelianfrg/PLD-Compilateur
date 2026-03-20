#include "IR.h"

// --- CFG METHODS ---

CFG::CFG(tree::ParseTree *ast)
{
	this->ast = ast;
	nextBBnumber = 0;
}

CFG::~CFG() {
	for (Block* b : blocks) {
		delete b;
	}
}

void CFG::gen_asm(ostream &os)
{
	//this->gen_asm_prologue(os);
	for (Block *b : blocks)
	{
		b->gen_asm(os);
	}
	//this->gen_asm_epilogue(os);
}

string CFG::new_BB_name()
{
	return "bloc_" + to_string(nextBBnumber);
}
void CFG::add_block(Block *b)
{
	blocks.push_back(b);
	nextBBnumber++;
	current_block = b;
}
BasicBlock *CFG::createChildBasicBlock(const SymbolsTable & parentSymbolsTable)
{
	string name = new_BB_name();
	BasicBlock *bb = new BasicBlock(this, name, parentSymbolsTable, true);
	bb->symbolsTable.setBlock(bb);
	add_block(bb);
	return bb;
}

BasicBlock *CFG::createSiblingBasicBlock(const SymbolsTable & siblingSymbolsTable)
{
	string name = new_BB_name();
	BasicBlock *bb = new BasicBlock(this, name, siblingSymbolsTable, false);
	bb->symbolsTable.setBlock(bb);
	add_block(bb);
	return bb;
}
FunctionBlock *CFG::createFunctionBlock(string label, vector<Type> paramsType, vector<string> paramsName)
{
	// TODO : check name of block does not already exists
	FunctionBlock *fb = new FunctionBlock(this, label, paramsType, paramsName);
	fb->symbolsTable.setBlock(fb);
	add_block(fb);
	return fb;
}

ostream &operator<<(ostream &os, const CFG &cfg)
{
	for (Block *block : cfg.blocks)
	{
		os << *block << endl;
	}
	os << "FunctionsTable : " << endl;
	os << cfg.functionsTable << endl;
	return os;
}



// --- BLOCK METHODS ---

Block::~Block() {
	for (IRInstr* instr : this->instrs) {
		delete instr;
	}
}

BasicBlock::BasicBlock(CFG *cfg, string label, SymbolsTable currentSymbolsTable, bool isAChild)
{
	this->exit_false = nullptr;
	this->exit_true = nullptr;
	this->label = label;
	this->test_var_name = "";
	this->cfg = cfg;

	// initialize the symbolsTable for this block with values from the previous one, depending on the relation between the blocks (child or sibling)
	this->symbolsTable = SymbolsTable(this);
	this->symbolsTable.setCurrentOffset(currentSymbolsTable.getCurrentOffset());
	this->symbolsTable.setTemporaryVarCount(currentSymbolsTable.getTemporaryVarCount());
	if (isAChild) {
		this->symbolsTable.setInheritedSymbols(currentSymbolsTable.getSymbols());
	}
	else {
		this->symbolsTable.setInheritedSymbols(currentSymbolsTable.getInheritedSymbols());
		this->symbolsTable.setLocalSymbols(currentSymbolsTable.getLocalSymbols());
	}
}

FunctionBlock::FunctionBlock(CFG *cfg, string label, vector<Type> paramsType, vector<string> paramsName)
{
	this->exit_false = nullptr;
	this->exit_true = nullptr;
	this->label = label;
	this->test_var_name = "";
	this->cfg = cfg;

	// initialize with a new SymbolsTable containing the parameters
	this->symbolsTable = SymbolsTable();
	for (int i = 0; i < paramsType.size(); ++i) {
		symbolsTable.create_new_var(paramsType.at(i), paramsName.at(i));
	}
}

void Block::gen_block_linking_asm(ostream &os, IRInstr* lastInstr) {
	// BLOCK JUMP LOGIC
	if (this->exit_true != nullptr and this->exit_false == nullptr)
	{
		// linear code
		os << "    jmp     " << this->exit_true->label << endl;
	}
	else if (this->exit_true != nullptr and this->exit_false != nullptr)
	{
		string condVarName = this->test_var_name;
		Symbol &condVar = symbolsTable.access(condVarName);
		string address = to_string(condVar.getOffset()) + "(%rbp)";
		os << "    movl    " << address << ", " << "%eax" << endl;
		os << "    cmpl    " << "$0, %eax" << endl;
		os << "    jne     " << this->exit_true->label << endl;
		os << "    jmp     " << this->exit_false->label << endl;
	}
	else if (this->exit_true == nullptr and this->exit_false == nullptr)
	{
		// check if previous instruction in block was a return statement : in that case, no need to write block exit assembly
		if (lastInstr != nullptr && lastInstr->getOp() == IRInstr::ret) {
			return;
		}
		else {
			// generate block epilogue
			os << "    movq    %rbp, %rsp" << endl;
			os << "    popq    %rbp" << endl;
			os << "    ret" << endl;
			os << endl; // make function ending more visible 
		}
	}
	else
	{
		cerr << "INTERNAL ERROR : Unproper block linking" << endl;
	}
}


void BasicBlock::gen_asm(ostream &os)
{
	IRInstr* lastInstr = nullptr; // trick to avoid writing return instructions twice in a block ending after a return

	os << this->label << ":" << endl;
	for (IRInstr *instr : this->instrs)
	{
		instr->gen_asm(os);
		lastInstr = instr;
	}

	gen_block_linking_asm(os, lastInstr);
}

void FunctionBlock::gen_asm(ostream &os) {
	// get the local size to move return stack pointer accordingly 
	int local_stack_size = this->symbolsTable.getLocalSize();
	string local_stack_size_string = string("$") + to_string(local_stack_size);

	os << ".globl  " << label << endl;
	os << ".type   " << label << ", @function" << endl;
	os << this->label << ":" << endl;

	// generate block prologue
	os << "    pushq   %rbp" << endl;
	os << "    movq    %rsp, %rbp" << endl;
	os << "    subq    " << local_stack_size_string << ", %rsp" << endl;

	// copy eventual passed parameters to local variables
	vector<string> paramsRegisters = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
	vector<string> paramsName = this->cfg->functionsTable.access(this->label).getParamsName();
	for (int i = 0; i < paramsName.size(); ++i ) {
		Symbol & param = this->symbolsTable.access(paramsName.at(i));
		string reg = paramsRegisters.at(i);
		os << "    movl    " << reg << ", " << param.getAdressx86() << endl;
	}

	// generate asm for internal instructions
	IRInstr* lastInstr;
	for (IRInstr *instr : this->instrs)
	{
		instr->gen_asm(os);
		lastInstr = instr;
	}

	gen_block_linking_asm(os, lastInstr);
}

void Block::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params)
{
	IRInstr *instr = new IRInstr(this, op, t, params);
	instrs.push_back(instr);
}

void BasicBlock::print(ostream& os) const {
	os << this->label << ":" << endl;
	for (IRInstr *instr : this->instrs)
	{
		os << *instr << endl;
	}
	if (this->exit_true != nullptr)
	{
		os << "exit_true : " << this->exit_true->label << endl;
	}
	if (this->exit_false != nullptr)
	{
		os << "exit_false : " << this->exit_false->label << endl;
	}
}

void FunctionBlock::print(ostream& os) const {
	os << this->label << " (function) : " << endl;
	for (IRInstr *instr : this->instrs)
	{
		os << *instr << endl;
	}
}

ostream &operator<<(ostream &os, const Block &b)
{
	b.print(os);
	return os;
}

// --- IRInstr METHODS ---
IRInstr::IRInstr(Block *b, Operation op, Type t, vector<string> params)
{
	this->block = b;
	this->op = op;
	this->t = t;
	this->params = params;
}

IRInstr::Operation IRInstr::getOp() {
	return this->op;
}


void IRInstr::gen_asm(ostream &os)
{
	// SPECIFIC LOGIC FOR INSTRUCTIONS
	switch (this->op)
	{
	case IRInstr::ldconst:
		this->gen_asm_ldconst(os);
		break;
	case IRInstr::ret:
		this->gen_asm_ret(os);
		break;
	case IRInstr::copy:
		this->gen_asm_copy(os);
		break;
	case IRInstr::add:
		this->gen_asm_add(os);
		break;
	case IRInstr::neg:
		this->gen_asm_neg(os);
		break;
	case IRInstr::not_:
		this->gen_asm_not(os);
		break;
	case IRInstr::bit_not:
		this->gen_asm_bit_not(os);
		break;
	case IRInstr::sub:
		this->gen_asm_sub(os);
		break;
	case IRInstr::mul:
		this->gen_asm_mul(os);
		break;
	case IRInstr::div:
		this->gen_asm_div(os);
		break;
	case IRInstr::mod:
		this->gen_asm_mod(os);
		break;
	case IRInstr::cmp_eq:
		this->gen_asm_eq(os);
		break;
	case IRInstr::cmp_diff:
		this->gen_asm_diff(os);
		break;
	case IRInstr::cmp_lt:
		this->gen_asm_lt(os);
		break;
	case IRInstr::cmp_le:
		this->gen_asm_le(os);
		break;
	case IRInstr::bit_and:
		this->gen_asm_and(os);
		break;
	case IRInstr::bit_xor:
		this->gen_asm_xor(os);
		break;
	case IRInstr::bit_or:
		this->gen_asm_or(os);
		break;
	case IRInstr::call:
		this->gen_asm_call(os);
		break;
	case IRInstr::ldchar:
		this->gen_asm_ldchar(os);
		break;
	case IRInstr::shl:
		this->gen_asm_shl(os);
		break;
	case IRInstr::shr:
		this->gen_asm_shr(os);
		break;
	case IRInstr::incr_prefix:
		this->gen_asm_incr_prefix(os);
		break;
	case IRInstr::decr_prefix:
		this->gen_asm_decr_prefix(os);
		break;
	case IRInstr::incr_postfix:
		this->gen_asm_incr_postfix(os);
		break;
	case IRInstr::decr_postfix:
		this->gen_asm_decr_postfix(os);
		break;
	default:
		cerr << "INTERNAL ERROR : Unknown instruction \"" << this->op << "\"encountered when generating assembly" << endl;
	}
}

void IRInstr::gen_asm_ldconst(ostream &os)
{
	string value = params.at(0);
	string tempVarName = params.at(1);

	Symbol &tempVar = block->symbolsTable.access(tempVarName);
	string address = to_string(tempVar.getOffset()) + "(%rbp)";
	string const_value = string("$") + value;
	os << "    movl    " << const_value << ", " << address << endl;
}

void IRInstr::gen_asm_ret(ostream &os)
{
	string tempVarName = params.at(0);
	Symbol &tempVar = block->symbolsTable.access(tempVarName);
	string address = to_string(tempVar.getOffset()) + "(%rbp)";
	os << "    movl    " << address << ", " << "%eax" << endl;

	// TEMPORARY WAY OF HANDLING RETURNS ANYWHERE
	os << "    movq    %rbp, %rsp" << endl;
	os << "    popq    %rbp" << endl;
	os << "    ret" << endl;
}

void IRInstr::gen_asm_eq(ostream &os)
{
	string resultVarName = params.at(0);
	string tempVarName1 = params.at(1);
	string tempVarName2 = params.at(2);
	Symbol &resultVar = block->symbolsTable.access(resultVarName);
	Symbol &tempVar1 = block->symbolsTable.access(tempVarName1);
	Symbol &tempVar2 = block->symbolsTable.access(tempVarName2);
	string resultAddress = to_string(resultVar.getOffset()) + "(%rbp)";
	string tempVar1Address = to_string(tempVar1.getOffset()) + "(%rbp)";
	string tempVar2Address = to_string(tempVar2.getOffset()) + "(%rbp)";

	os << "    movl    " << tempVar1Address << ", %eax" << endl;
	os << "    cmpl    " << "%eax" << ", " << tempVar2Address << endl;
	os << "    sete    %al" << endl;	   // sete %al to 1 if equal
	os << "    movzbl  %al, %eax" << endl; // move with conversion from byte to int
	os << "    movl    %eax, " << resultAddress << endl;
}

void IRInstr::gen_asm_diff(ostream &os)
{
	string resultVarName = params.at(0);
	string tempVarName1 = params.at(1);
	string tempVarName2 = params.at(2);
	Symbol &resultVar = block->symbolsTable.access(resultVarName);
	Symbol &tempVar1 = block->symbolsTable.access(tempVarName1);
	Symbol &tempVar2 = block->symbolsTable.access(tempVarName2);
	string resultAddress = to_string(resultVar.getOffset()) + "(%rbp)";
	string tempVar1Address = to_string(tempVar1.getOffset()) + "(%rbp)";
	string tempVar2Address = to_string(tempVar2.getOffset()) + "(%rbp)";

	os << "    movl    " << tempVar1Address << ", %eax" << endl;
	os << "    cmpl    " << "%eax" << ", " << tempVar2Address << endl;
	os << "    setne   %al" << endl;	   // sete %al to 1 if equal
	os << "    movzbl  %al, %eax" << endl; // move with conversion from byte to int
	os << "    movl    %eax, " << resultAddress << endl;
}

void IRInstr::gen_asm_lt(ostream &os)
{
	string resultVarName = params.at(0);
	string tempVarName1 = params.at(1);
	string tempVarName2 = params.at(2);
	Symbol &resultVar = block->symbolsTable.access(resultVarName);
	Symbol &tempVar1 = block->symbolsTable.access(tempVarName1);
	Symbol &tempVar2 = block->symbolsTable.access(tempVarName2);
	string resultAddress = to_string(resultVar.getOffset()) + "(%rbp)";
	string tempVar1Address = to_string(tempVar1.getOffset()) + "(%rbp)";
	string tempVar2Address = to_string(tempVar2.getOffset()) + "(%rbp)";

	os << "    movl    " << tempVar2Address << ", %eax" << endl;
	os << "    cmpl    " << "%eax" << ", " << tempVar1Address << endl;
	os << "    setl    %al" << endl;	   // sete %al to 1 if lower
	os << "    movzbl  %al, %eax" << endl; // move with conversion from byte to int
	os << "    movl    %eax, " << resultAddress << endl;
}

void IRInstr::gen_asm_le(ostream &os)
{
	string resultVarName = params.at(0);
	string tempVarName1 = params.at(1);
	string tempVarName2 = params.at(2);
	Symbol &resultVar = block->symbolsTable.access(resultVarName);
	Symbol &tempVar1 = block->symbolsTable.access(tempVarName1);
	Symbol &tempVar2 = block->symbolsTable.access(tempVarName2);
	string resultAddress = to_string(resultVar.getOffset()) + "(%rbp)";
	string tempVar1Address = to_string(tempVar1.getOffset()) + "(%rbp)";
	string tempVar2Address = to_string(tempVar2.getOffset()) + "(%rbp)";

	os << "    movl    " << tempVar2Address << ", %eax" << endl;
	os << "    cmpl    " << "%eax" << ", " << tempVar1Address << endl;
	os << "    setle   %al" << endl;	   // sete %al to 1 if lower
	os << "    movzbl  %al, %eax" << endl; // move with conversion from byte to int
	os << "    movl    %eax, " << resultAddress << endl;
}

void IRInstr::gen_asm_copy(ostream &os)
{
	string dest = params.at(0);
	string src = params.at(1);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &srcVar = block->symbolsTable.access(src);
	string srcAddress = to_string(srcVar.getOffset()) + "(%rbp)";
	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	os << "    movl    " << srcAddress << ", " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
}

void IRInstr::gen_asm_add(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	string v1Address = to_string(v1Var.getOffset()) + "(%rbp)";
	string v2Address = to_string(v2Var.getOffset()) + "(%rbp)";
	os << "    movl    " << v1Address << ", " << "%eax" << endl;
	os << "    addl    " << v2Address << ", " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
}

void IRInstr::gen_asm_neg(ostream &os)
{
	string dest = params.at(0);
	string src = params.at(1);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &srcVar = block->symbolsTable.access(src);
	string srcAddress = to_string(srcVar.getOffset()) + "(%rbp)";
	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	os << "    movl    " << srcAddress << ", " << "%eax" << endl;
	os << "    negl    " << "%eax" << endl;
	os << "    movl    %eax, " << destAddress << endl;
}

void IRInstr::gen_asm_not(ostream &os)
{
	string dest = params.at(0);
	string src = params.at(1);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &srcVar = block->symbolsTable.access(src);
	string srcAddress = to_string(srcVar.getOffset()) + "(%rbp)";
	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	
	os << "    cmpl    " << "$0" << ", " << srcAddress << endl;
	os << "    sete    %al" << endl;	   // sete %al to 1 if equal
	os << "    movzbl  %al, %eax" << endl; // move with conversion from byte to int
	os << "    movl    %eax, " << destAddress << endl;
}

void IRInstr::gen_asm_bit_not(ostream &os)
{
	string dest = params.at(0);
	string src = params.at(1);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &srcVar = block->symbolsTable.access(src);
	string srcAddress = to_string(srcVar.getOffset()) + "(%rbp)";
	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	os << "    movl    " << srcAddress << ", " << "%eax" << endl;
	os << "    notl    " << "%eax" << endl;
	os << "    movl    %eax, " << destAddress << endl;
}

void IRInstr::gen_asm_sub(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	string v1Address = to_string(v1Var.getOffset()) + "(%rbp)";
	string v2Address = to_string(v2Var.getOffset()) + "(%rbp)";
	os << "    movl    " << v1Address << ", " << "%eax" << endl;
	os << "    subl    " << v2Address << ", " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
}

void IRInstr::gen_asm_mul(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	string v1Address = to_string(v1Var.getOffset()) + "(%rbp)";
	string v2Address = to_string(v2Var.getOffset()) + "(%rbp)";
	os << "    movl    " << v1Address << ", " << "%eax" << endl;
	os << "    imull    " << v2Address << ", " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
}

void IRInstr::gen_asm_div(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	string v1Address = to_string(v1Var.getOffset()) + "(%rbp)";
	string v2Address = to_string(v2Var.getOffset()) + "(%rbp)";
	os << "    movl    " << v1Address << ", " << "%eax" << endl;
	os << "    cltd" << endl; // Extend %eax into %edx:%eax for division
	os << "    idivl    " << v2Address << endl;
	os << "    movl    " << "%eax" << ", " << destAddress << endl; // quotient is in %eax
}

void IRInstr::gen_asm_mod(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	string v1Address = to_string(v1Var.getOffset()) + "(%rbp)";
	string v2Address = to_string(v2Var.getOffset()) + "(%rbp)";
	os << "    movl    " << v1Address << ", " << "%eax" << endl;
	os << "    cltd" << endl; // Extend %eax into %edx:%eax for division
	os << "    idivl    " << v2Address << endl;
	os << "    movl    " << "%edx" << ", " << destAddress << endl; // Remainder is in %edx
}

void IRInstr::gen_asm_and(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	string v1Address = to_string(v1Var.getOffset()) + "(%rbp)";
	string v2Address = to_string(v2Var.getOffset()) + "(%rbp)";
	os << "    movl    " << v1Address << ", " << "%eax" << endl;
	os << "    andl    " << v2Address << ", " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
}

void IRInstr::gen_asm_xor(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	string v1Address = to_string(v1Var.getOffset()) + "(%rbp)";
	string v2Address = to_string(v2Var.getOffset()) + "(%rbp)";
	os << "    movl    " << v1Address << ", " << "%eax" << endl;
	os << "    xorl    " << v2Address << ", " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
}

void IRInstr::gen_asm_or(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	string v1Address = to_string(v1Var.getOffset()) + "(%rbp)";
	string v2Address = to_string(v2Var.getOffset()) + "(%rbp)";
	os << "    movl    " << v1Address << ", " << "%eax" << endl;
	os << "    orl    " << v2Address << ", " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
}

void IRInstr::gen_asm_call(ostream &os) {
	string functionName = params.at(0);
	string destVarName = params.at(1);
	Type returnType = VOID;
	if (functionName != "putchar" && functionName != "getchar"){
		Function & function = this->block->cfg->functionsTable.access(functionName);
		returnType = function.getType();
	}
	else if (functionName == "putchar"){
		functionName = "putchar@PLT";
		returnType = INT;
	}
	else {
		functionName = "getchar@PLT";
		returnType = INT;
	}
	
	Symbol & destVar = this->block->symbolsTable.access(destVarName);
	
	// max 6 arguments for now
	vector<string> paramsRegisters = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
	// move parameters to appropriate registers
	for (int i = 2; i < params.size(); ++i) {
		Symbol & param = this->block->symbolsTable.access(params.at(i));
		string paramAddr = param.getAdressx86();
		string reg = paramsRegisters.at(i-2);
		os << "    movl    " << paramAddr << ", " << reg << endl;
	}

	// execute call
	os << "    call    " << functionName << endl;

	// handle return value 
	if (functionName == "putchar@PLT"){
		os <<"movl	$0, %eax"<< endl;
	}
	else if (returnType != VOID) {
		os << "    movl    %eax, " << destVar.getAdressx86() << endl;
	}
}

void IRInstr::gen_asm_ldchar(ostream &os) {
	string value = params.at(0);
	int ascii = (int)value[1];
	string tempVarName = params.at(1);
	Symbol &tempVar = block->symbolsTable.access(tempVarName);
	string address = to_string(tempVar.getOffset()) + "(%rbp)";
	string char_value = string("$") + to_string(ascii);
	os << "    movl    " << char_value << ", " << address << endl;
}

void IRInstr::gen_asm_shl(ostream &os) {
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	string v1Address = to_string(v1Var.getOffset()) + "(%rbp)";
	string v2Address = to_string(v2Var.getOffset()) + "(%rbp)";
	os << "    movl    " << v1Address << ", " << "%eax" << endl;
	os << "    movl    " << v2Address << ", " << "%ecx" << endl;
	os << "    sall    " << "%cl" << ", " << "%eax"<< endl;		// cl est l'octet bas de ecx
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
}

void IRInstr::gen_asm_shr(ostream &os) {
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	string v1Address = to_string(v1Var.getOffset()) + "(%rbp)";
	string v2Address = to_string(v2Var.getOffset()) + "(%rbp)";
	os << "    movl    " << v1Address << ", " << "%eax" << endl;
	os << "    movl    " << v2Address << ", " << "%ecx" << endl; 
	os << "    sarl    " << "%cl" << ", " << "%eax"<< endl;	// cl est l'octet bas de ecx
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
}

void IRInstr::gen_asm_incr_prefix(ostream &os) {
	string varName = params.at(0);
	Symbol &var = block->symbolsTable.access(varName);
	string varAddress = to_string(var.getOffset()) + "(%rbp)";
	os << "    movl    " << varAddress << ", " << "%eax" << endl;
	os << "    incl    " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << varAddress << endl;
}

void IRInstr::gen_asm_decr_prefix(ostream &os) {
	string varName = params.at(0);
	Symbol &var = block->symbolsTable.access(varName);
	string varAddress = to_string(var.getOffset()) + "(%rbp)";
	os << "    movl    " << varAddress << ", " << "%eax" << endl;
	os << "    decl    " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << varAddress << endl;
}

void IRInstr::gen_asm_incr_postfix(ostream &os) {
	string dest = params.at(0);
	string src = params.at(1);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &srcVar = block->symbolsTable.access(src);
	string srcAddress = to_string(srcVar.getOffset()) + "(%rbp)";
	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	
	os << "    movl    " << srcAddress << ", " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
    os << "    incl    " << "%eax" << endl;
    os << "    movl    " << "%eax" << ", " << srcAddress << endl;
}

void IRInstr::gen_asm_decr_postfix(ostream &os) {
	string dest = params.at(0);
	string src = params.at(1);

	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &srcVar = block->symbolsTable.access(src);
	string srcAddress = to_string(srcVar.getOffset()) + "(%rbp)";
	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	
	os << "    movl    " << srcAddress << ", " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
    os << "    decl    " << "%eax" << endl;
    os << "    movl    " << "%eax" << ", " << srcAddress << endl;
}

ostream &operator<<(ostream &os, const IRInstr &irInstr)
{
	os << "    ";
	switch (irInstr.op)
	{
	case IRInstr::ldconst:
		os << "ldconst " << irInstr.params.at(0) << " --> " << irInstr.params.at(1);
		break;
	case IRInstr::ret:
		os << "ret     " << irInstr.params.at(0);
		break;
	case IRInstr::copy:
		os << "copy    " << irInstr.params.at(1) << " --> " << irInstr.params.at(0);
		break;
	case IRInstr::add:
		os << "add     " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " + " << irInstr.params.at(2);
		break;
	case IRInstr::not_:
		os << "add     " << irInstr.params.at(1) << " --> !" << irInstr.params.at(0);
		break;
	case IRInstr::neg:
		os << "neg     -" << irInstr.params.at(0);
		break;
	case IRInstr::bit_not:
		os << "bit_not " << irInstr.params.at(1) << " --> ~" << irInstr.params.at(0);
		break;
	case IRInstr::sub:
		os << "sub     " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " - " << irInstr.params.at(2);
		break;
	case IRInstr::mul:
		os << "mul     " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " * " << irInstr.params.at(2);
		break;
	case IRInstr::cmp_eq:
		os << "cmp_eq  " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " == " << irInstr.params.at(2);
		break;
	case IRInstr::cmp_diff:
		os << "cmp_diff " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " != " << irInstr.params.at(2);
		break;
	case IRInstr::div:
		os << "div     " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " / " << irInstr.params.at(2);
		break;
	case IRInstr::mod:
		os << "mod     " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " % " << irInstr.params.at(2);
		break;
	case IRInstr::cmp_lt:
		os << "cmp_lt  " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " < " << irInstr.params.at(2);
		break;
	case IRInstr::cmp_le:
		os << "cmp_le  " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " <= " << irInstr.params.at(2);
		break;
	case IRInstr::bit_and:
		os << "bit_and " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " & " << irInstr.params.at(2);
		break;
	case IRInstr::bit_xor:
		os << "bit_xor " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " ^ " << irInstr.params.at(2);
		break;
	case IRInstr::bit_or:
		os << "bit_or  " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " | " << irInstr.params.at(2);
		break;
	case IRInstr::call:
		os << "call    " << irInstr.params.at(0) << " --> " << irInstr.params.at(1);
		break;
	case IRInstr::ldchar:
		os << "ldchar  " << irInstr.params.at(0) << " --> " << irInstr.params.at(1);
		break;
	case IRInstr::shl:
		os << "shl     " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " << " << irInstr.params.at(2);
		break;
	case IRInstr::shr:
		os << "shr     " << irInstr.params.at(0) << " = " << irInstr.params.at(1) << " >> " << irInstr.params.at(2);
		break;
	case IRInstr::incr_prefix:
		os << "incr    " << "++" << irInstr.params.at(0);
		break;
	case IRInstr::decr_prefix:
		os << "decr    " << "--" << irInstr.params.at(0);
		break;
	case IRInstr::incr_postfix:
		os << "incr    " << irInstr.params.at(0) << "++";
		break;
	case IRInstr::decr_postfix:
		os << "decr    " << irInstr.params.at(0) << "--";
		break;
	default:
		os << "(unknown instruction) ";
		for (const string &param : irInstr.params)
		{
			os << param << " ";
		}
		break;
	}

	return os;
}