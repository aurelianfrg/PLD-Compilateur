#include "IR.h"


// --- CFG METHODS ---

CFG::CFG(tree::ParseTree* ast) {
	ast = ast;
	nextBBnumber = 0;
}

void CFG::gen_asm_prologue(ostream& o) {
	#ifdef __APPLE__
    cout<< ".globl _main\n" ;
    cout<< " _main: \n" ;
    #else
    cout<< ".globl main\n" ;
    cout<< " main: \n" ;
    #endif

    //prologue 
    cout << "    # prologue\n";
    cout << "    pushq   %rbp\n";
    cout << "    movq    %rsp, %rbp\n";
    cout << "\n";
}

void CFG::gen_asm_epilogue(ostream& o) {
	cout << "\n";
    cout << "    # epilogue\n";
    cout << "    popq    %rbp\n";
    
    cout << "    ret\n";
}

string CFG::new_BB_name() {
	return "bloc_" + to_string(nextBBnumber);
}

void CFG::add_bb(BasicBlock* bb) {
	bbs.push_back(bb);
	nextBBnumber++;
	current_bb = bb;
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



// --- BASIC BLOCK METHODS ---

void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params) {
	IRInstr* instr = new IRInstr(this, op, t, params);
	instrs.push_back(instr);
}

BasicBlock::BasicBlock(CFG* cfg, string entry_label) {
	exit_false = nullptr;
	exit_true = nullptr;
	label = entry_label;
	test_var_name = "";
	cfg = cfg;
}



// --- IRInstr METHODS ---
IRInstr::IRInstr(BasicBlock* bb_, Operation op, Type t, vector<string> params) {
	bb = bb_;
	op = op;
	t = t;
	params = params;
}

// specific instructions

