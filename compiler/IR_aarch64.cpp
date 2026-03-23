#include "IR.h"

// --- CFG METHODS ---

void CFG::gen_asm_aarch64(ostream &os)
{
	for (Block *b : blocks)
	{
		b->gen_asm_aarch64(os);
	}
}

// --- BLOCK METHODS ---

void Block::gen_block_linking_asm_aarch64(ostream &os, IRInstr *lastInstr) {
	// BLOCK JUMP LOGIC
	if (this->exit_true != nullptr and this->exit_false == nullptr)
	{
		// linear code
		os << "    b     " << this->exit_true->label << endl;
	}
	else if (this->exit_true != nullptr and this->exit_false != nullptr)
	{
		string condVarName = this->test_var_name;
		Symbol &condVar = symbolsTable.access(condVarName);
		string address = "[sp, "+to_string(condVar.getOffset())+"]";
		os << "    mov    " << "w0, " << address << endl;
		os << "    cmp    " << "w0, #0" << endl;
		os << "    bcz     " << this->exit_true->label << endl;
		os << "    b     " << this->exit_false->label << endl;
	}
	else if (this->exit_true == nullptr and this->exit_false == nullptr)
	{
		// generate block epilogue
		os << "    ldp	x29, x30, [sp], 32" << endl;
		os << "    ret" << endl;
		os << endl; // make function ending more visible 
	}
	else
	{
		cerr << "INTERNAL ERROR : Unproper block linking" << endl;
	}
}


void BasicBlock::gen_asm_aarch64(ostream &os)
{
	IRInstr *lastInstr = nullptr; // trick to avoid writing return instructions twice in a block
	os << this->label << ":" << endl;
	for (IRInstr *instr : this->instrs)
	{
		instr->gen_asm_aarch64(os);
		lastInstr = instr;
	}
	gen_block_linking_asm_aarch64(os, lastInstr);
}

void FunctionBlock::gen_asm_aarch64(ostream &os) {
	// get the local size to move return stack pointer accordingly 
	int local_stack_size = this->symbolsTable.getLocalSize();
	string local_stack_size_string = string("#") + to_string(local_stack_size);

	os << ".globl  " << label << endl;
	os << ".type   " << label << ", %function" << endl;
	os << this->label << ":" << endl;

	// generate block prologue
	os << "    sub     sp, sp, " << local_stack_size_string << endl;

	// copy eventual passed parameters to local variables
	vector<string> paramsRegisters = {"w0", "w1", "w2", "w3", "w4", "w5"};
	vector<string> paramsName = this->cfg->functionsTable.access(this->label).getParamsName();
	for (int i = 0; i < paramsName.size(); ++i ) {
		Symbol & param = this->symbolsTable.access(paramsName.at(i));
		string reg = paramsRegisters.at(i);
		os << "    mov    " << param.getAdressAarch64() << ", " << reg << endl;
	}

	// generate asm for internal instructions
	IRInstr *lastInstr = nullptr; // trick to avoid writing return instructions twice in a block
	for (IRInstr *instr : this->instrs)
	{
		instr->gen_asm_aarch64(os);
		lastInstr = instr;
	}

	gen_block_linking_asm_aarch64(os, lastInstr);
}

void IRInstr::gen_asm_aarch64(ostream &os)
{
	// SPECIFIC LOGIC FOR INSTRUCTIONS
	switch (this->op)
	{
	case IRInstr::ldconst:
		this->gen_asm_ldconst_aarch64(os);
		break;
	case IRInstr::ret:
		this->gen_asm_ret_aarch64(os);
		break;
	case IRInstr::copy:
		this->gen_asm_copy_aarch64(os);
		break;
	case IRInstr::add:
		this->gen_asm_add_aarch64(os);
		break;
	case IRInstr::neg:
		this->gen_asm_neg_aarch64(os);
		break;
	case IRInstr::not_:
		this->gen_asm_not_aarch64(os);
		break;
	case IRInstr::sub:
		this->gen_asm_sub_aarch64(os);
		break;
	case IRInstr::mul:
		this->gen_asm_mul_aarch64(os);
		break;
	case IRInstr::div:
		this->gen_asm_div_aarch64(os);
		break;
	case IRInstr::mod:
		this->gen_asm_mod_aarch64(os);
		break;
	case IRInstr::cmp_eq:
		this->gen_asm_eq_aarch64(os);
		break;
	case IRInstr::cmp_diff:
		this->gen_asm_diff_aarch64(os);
		break;
	case IRInstr::cmp_lt:
		this->gen_asm_lt_aarch64(os);
		break;
	case IRInstr::cmp_le:
		this->gen_asm_le_aarch64(os);
		break;
	case IRInstr::bit_and:
		this->gen_asm_and_aarch64(os);
		break;
	case IRInstr::bit_xor:
		this->gen_asm_xor_aarch64(os);
		break;
	case IRInstr::bit_or:
		this->gen_asm_or_aarch64(os);
		break;
	case IRInstr::call:
		this->gen_asm_call_aarch64(os);
		break;
	case IRInstr::ldchar:
		this->gen_asm_ldchar_aarch64(os);
		break;
	default:
		cerr << "INTERNAL ERROR : Unknown instruction \"" << this->op << "\"encountered when generating assembly" << endl;
	}
}

void IRInstr::gen_asm_ldconst_aarch64(ostream &os)
{
	string value = params.at(0);
	string tempVarName = params.at(1);
	Symbol &tempVar = block->symbolsTable.access(tempVarName);
	string const_value = string("#") + value;
	os << "    mov    " << tempVar.getAdressAarch64() << ", " << const_value << endl;
}

void IRInstr::gen_asm_ret_aarch64(ostream &os)
{
	string tempVarName = params.at(0);
	Symbol &tempVar = block->symbolsTable.access(tempVarName);
	string address = to_string(tempVar.getOffset()) + "(%rbp)";
	os << "    movl    w0, " << tempVar.getAdressAarch64() << endl;

	// PERMANENT WAY OF HANDLING RETURNS ANYWHERE
	os << "    movq    %rbp, %rsp" << endl;
	os << "    popq    %rbp" << endl;
	os << "    ret" << endl;
}

void IRInstr::gen_asm_eq_aarch64(ostream &os)
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

void IRInstr::gen_asm_diff_aarch64(ostream &os)
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

void IRInstr::gen_asm_lt_aarch64(ostream &os)
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

void IRInstr::gen_asm_le_aarch64(ostream &os)
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

void IRInstr::gen_asm_copy_aarch64(ostream &os)
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

void IRInstr::gen_asm_add_aarch64(ostream &os)
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

void IRInstr::gen_asm_neg_aarch64(ostream &os)
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

void IRInstr::gen_asm_not_aarch64(ostream &os)
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

void IRInstr::gen_asm_sub_aarch64(ostream &os)
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

void IRInstr::gen_asm_mul_aarch64(ostream &os)
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

void IRInstr::gen_asm_div_aarch64(ostream &os)
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

void IRInstr::gen_asm_mod_aarch64(ostream &os)
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

void IRInstr::gen_asm_and_aarch64(ostream &os)
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

void IRInstr::gen_asm_xor_aarch64(ostream &os)
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

void IRInstr::gen_asm_or_aarch64(ostream &os)
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

void IRInstr::gen_asm_call_aarch64(ostream &os) {
	string functionName = params.at(0);
	string destVarName = params.at(1);
	Function & function = this->block->cfg->functionsTable.access(functionName);
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
	if (function.getType() != VOID) {
		os << "    movl    %eax, " << destVar.getAdressx86() << endl;
	}
}

void IRInstr::gen_asm_ldchar_aarch64(ostream &os) {
	string value = params.at(0);
	int ascii = (int)value[1];
	string tempVarName = params.at(1);
	Symbol &tempVar = block->symbolsTable.access(tempVarName);
	string address = to_string(tempVar.getOffset()) + "(%rbp)";
	string char_value = string("$") + to_string(ascii);
	os << "    movl    " << char_value << ", " << address << endl;
}