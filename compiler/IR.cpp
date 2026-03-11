#include "IR.h"

// --- CFG METHODS ---

CFG::CFG(tree::ParseTree *ast)
{
	this->ast = ast;
	nextBBnumber = 0;
}

void CFG::gen_asm_prologue(ostream &os)
{
#ifdef __APPLE__
	os << ".globl _main\n";
	os << " _main: \n";
#else
	os << ".globl main\n";
	os << " main: \n";
#endif

	// prologue
	os << "    # prologue\n";
	os << "    pushq   %rbp\n";
	os << "    movq    %rsp, %rbp\n";
	os << "\n";
}

void CFG::gen_asm_epilogue(ostream &os)
{
	os << "\n";
	os << "    # epilogue\n";
	os << "    popq    %rbp\n";

	os << "    ret\n";
}

void CFG::gen_asm(ostream &os)
{
	this->gen_asm_prologue(os);
	for (BasicBlock *bb : bbs)
	{
		bb->gen_asm(os);
	}
	this->gen_asm_epilogue(os);
}

string CFG::new_BB_name()
{
	return "bloc_" + to_string(nextBBnumber);
}
void CFG::add_bb(BasicBlock *bb)
{
	bbs.push_back(bb);
	nextBBnumber++;
	current_bb = bb;
}
BasicBlock *CFG::createBasicBlock()
{
	string name = new_BB_name();
	BasicBlock *bb = new BasicBlock(this, name);
	add_bb(bb);
	return bb;
}

// temporary variables system
Symbol &CFG::create_new_tempvar(Type t)
{
	// create the Symbol inside the table and return a reference to it
	string varName = string("!tmp") + to_string(temporaryVarCount++);
	symbolsTable.add(Symbol(varName, newVarOffset(t), true));
	return symbolsTable.access(varName);
}

// temporary variables system
Symbol &CFG::create_new_var(Type t, string varName)
{
	// create the Symbol inside the table and return a reference to it
	symbolsTable.add(Symbol(varName, newVarOffset(t), true));
	return symbolsTable.access(varName);
}

void CFG::add_to_symbol_table(Symbol s)
{
	symbolsTable.add(s);
}
Symbol &CFG::access_symbol(string name)
{
	return symbolsTable.access(name);
}
int CFG::newVarOffset(Type type)
{
	currentOffset -= typeSizes.at(type);
	return currentOffset;
}

ostream &operator<<(ostream &os, const CFG &cfg)
{
	os << "CFG content : " << endl;
	for (BasicBlock *block : cfg.bbs)
	{
		os << *block << endl;
	}
	return os;
}

// --- BASIC BLOCK METHODS ---

BasicBlock::BasicBlock(CFG *cfg, string entry_label)
{
	exit_false = nullptr;
	exit_true = nullptr;
	label = entry_label;
	test_var_name = "";
	this->cfg = cfg;
}

void BasicBlock::gen_asm(ostream &os)
{
	os << this->label << ":" << endl;
	for (IRInstr *instr : this->instrs)
	{
		instr->gen_asm(os);
	}
	if (this->exit_true != nullptr or this->exit_false != nullptr)
	{
		// INSERER ICI LA LOGIQUE DE JUMP CONDITIONNELLE ENTRE BLOCS
	}
}

void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params)
{
	IRInstr *instr = new IRInstr(this, op, t, params);
	instrs.push_back(instr);
}

ostream &operator<<(ostream &os, const BasicBlock &bb)
{
	os << "BasicBlock " << bb.label << endl;
	for (IRInstr *instr : bb.instrs)
	{
		os << *instr << endl;
	}
	return os;
}

// --- IRInstr METHODS ---
IRInstr::IRInstr(BasicBlock *bb_, Operation op_, Type t_, vector<string> params_)
{
	bb = bb_;
	op = op_;
	t = t_;
	params = params_;
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
	case IRInstr::sub:
		this->gen_asm_sub(os);
		break;
	case IRInstr::mul:
		this->gen_asm_mul(os);
		break;
	default:
		cerr << "Unknown operation" << endl;
	}
}

void IRInstr::gen_asm_ldconst(ostream &os)
{
	string value = params.at(0);
	string tempVarName = params.at(1);

	Symbol &tempVar = bb->cfg->access_symbol(tempVarName);
	string address = to_string(tempVar.getOffset()) + "(%rbp)";
	string const_value = string("$") + value;
	os << "    movl    " << const_value << ", " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << address << endl;
}

void IRInstr::gen_asm_ret(ostream &os)
{
	string tempVarName = params.at(0);
	Symbol &tempVar = bb->cfg->access_symbol(tempVarName);
	string address = to_string(tempVar.getOffset()) + "(%rbp)";
	os << "    movl    " << address << ", " << "%eax" << endl;
}

void IRInstr::gen_asm_copy(ostream &os)
{
	string dest = params.at(0);
	string src = params.at(1);

	Symbol &destVar = bb->cfg->access_symbol(dest);
	Symbol &srcVar = bb->cfg->access_symbol(src);
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

	Symbol &destVar = bb->cfg->access_symbol(dest);
	Symbol &v1Var = bb->cfg->access_symbol(v1);
	Symbol &v2Var = bb->cfg->access_symbol(v2);

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

	Symbol &destVar = bb->cfg->access_symbol(dest);
	Symbol &srcVar = bb->cfg->access_symbol(src);
	string srcAddress = to_string(srcVar.getOffset()) + "(%rbp)";
	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	os << "    movl    " << srcAddress << ", " << "%eax" << endl;
	os << "    negl    " << "%eax" << endl;
	os << "    movl    %eax, " << destAddress << endl;
}

void IRInstr::gen_asm_sub(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);

	Symbol &destVar = bb->cfg->access_symbol(dest);
	Symbol &v1Var = bb->cfg->access_symbol(v1);
	Symbol &v2Var = bb->cfg->access_symbol(v2);

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

	Symbol &destVar = bb->cfg->access_symbol(dest);
	Symbol &v1Var = bb->cfg->access_symbol(v1);
	Symbol &v2Var = bb->cfg->access_symbol(v2);

	string destAddress = to_string(destVar.getOffset()) + "(%rbp)";
	string v1Address = to_string(v1Var.getOffset()) + "(%rbp)";
	string v2Address = to_string(v2Var.getOffset()) + "(%rbp)";
	os << "    movl    " << v1Address << ", " << "%eax" << endl;
	os << "    imull    " << v2Address << ", " << "%eax" << endl;
	os << "    movl    " << "%eax" << ", " << destAddress << endl;
}

ostream &operator<<(ostream &os, const IRInstr &irInstr)
{
	vector<string> operations = {// for printing
								 "ldconst",
								 "ret",
								 "copy",
								 "add",
								 "sub",
								 "mul",
								 "rmem",
								 "wmem",
								 "call",
								 "cmp_eq",
								 "cmp_lt",
								 "cmp_le",
								 "neg"};
	os << "    " << operations[irInstr.op] << " ";
	for (const string &param : irInstr.params)
	{
		os << param << " ";
	}
	return os;
}
