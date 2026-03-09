#include "IR.h"


// --- CFG METHODS ---

CFG::CFG(tree::ParseTree* ast) {
	ast = ast;
	nextBBnumber = 0;
}

void CFG::gen_asm_prologue(ostream& os) {
	#ifdef __APPLE__
    os<< ".globl _main\n" ;
    os<< " _main: \n" ;
    #else
    os<< ".globl main\n" ;
    os<< " main: \n" ;
    #endif

    //prologue 
    os << "    # prologue\n";
    os << "    pushq   %rbp\n";
    os << "    movq    %rsp, %rbp\n";
    os << "\n";
}

void CFG::gen_asm_epilogue(ostream& os) {
	os << "\n";
    os << "    # epilogue\n";
    os << "    popq    %rbp\n";
    
    os << "    ret\n";
}

void CFG::gen_asm(ostream& os) {
	this->gen_asm_prologue(os);
	for (BasicBlock* bb : bbs) {
		bb->gen_asm(os);
	}
	this->gen_asm_epilogue(os);
}



string CFG::new_BB_name() {
	return "bloc_" + to_string(nextBBnumber);
}
void CFG::add_bb(BasicBlock* bb) {
	bbs.push_back(bb);
	nextBBnumber++;
	current_bb = bb;
}
BasicBlock* CFG::createBasicBlock() {
	string name = new_BB_name();
	BasicBlock* bb = new BasicBlock(this, name);
	add_bb(bb);
	return bb;
}


// temporary variables system
Symbol & CFG::create_new_tempvar(Type t) {
	// create the Symbol inside the table and return a reference to it
	string varName = string("!tmp") + to_string(temporaryVarCount++);
	symbolsTable.add(Symbol(varName, newVarOffset(t), true));
	return symbolsTable.access(varName);
}
void CFG::add_to_symbol_table(Symbol s) {
	symbolsTable.add(s);
}
Symbol & CFG::access_symbol(string name) {
	return symbolsTable.access(name);
}
int CFG::newVarOffset(Type type) {                       
	currentOffset -= typeSizes.at(type);
	return currentOffset;
}

ostream & operator << (ostream & os, const CFG & cfg) {
	os << "CFG content : " << endl;
	for (BasicBlock* block : cfg.bbs) {
		os << *block << endl;
	}
	return os;
}

// --- BASIC BLOCK METHODS ---

BasicBlock::BasicBlock(CFG* cfg, string entry_label) {
	exit_false = nullptr;
	exit_true = nullptr;
	label = entry_label;
	test_var_name = "";
	cfg = cfg;
}

void BasicBlock::gen_asm(ostream& os) {
	os << this->label << ":" << endl;
	for (IRInstr* instr : this->instrs) {
		instr->gen_asm(os);
	}
	if (this->exit_true != nullptr or this->exit_false != nullptr) {
		// INSERER ICI LA LOGIQUE DE JUMP CONDITIONNELLE ENTRE BLOCS
	}
}

void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params) {
	IRInstr* instr = new IRInstr(this, op, t, params);
	instrs.push_back(instr);
}

ostream & operator << (ostream & os, const BasicBlock & bb) {
	os << "BasicBlock "  << bb.label << endl;
	for (IRInstr* instr : bb.instrs) {
		os << *instr << endl;
	}
	return os;
}

// --- IRInstr METHODS ---
IRInstr::IRInstr(BasicBlock* bb_, Operation op_, Type t_, vector<string> params_) {
	bb = bb_;
	op = op_;
	t = t_;
	params = params_;
}

void IRInstr::gen_asm(ostream& os) {
	if (this->op == IRInstr::ldconst) {
		string value = params.at(0);
		string reg = params.at(1);
		string const_value = string("$") + value;
		os << "    movl    " << const_value << ", " << reg << endl;
	}
	else if (this->op == IRInstr::ret){
		string reg = params.at(0);
		if (!(reg == "%eax")) {
			os << "    movl    " << reg << ", " << "%eax" << endl;
		}
	}
	else {
		cerr << "Unknown operation" << endl;
	}
	
}

ostream & operator << (ostream & os, const IRInstr & irInstr) {
	vector<string> operations = { // for printing 
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
		"cmp_le"
	};
	os << "    " << operations[irInstr.op] << " ";
	for (const string & param : irInstr.params) {
		os << param << " ";
	}
	return os;
}


