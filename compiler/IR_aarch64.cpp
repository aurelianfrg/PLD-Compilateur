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
		// linear code, unconditional jump
		os << "    b      " << this->exit_true->label << endl;
	}
	else if (this->exit_true != nullptr and this->exit_false != nullptr)
	{
		// conditional jump
		string condVarName = this->test_var_name;
		Symbol &condVar = symbolsTable.access(condVarName);
		os << "    ldr     w0, " << condVar.getAdressAarch64() << endl;
		os << "    cbnz    w0, " << this->exit_true->label << endl;
		os << "    cbz     w0, " << this->exit_false->label << endl;
	}
	else if (this->exit_true == nullptr and this->exit_false == nullptr)
	{
		/// check if previous instruction in block was a return statement : in that case, no need to
        // write block exit assembly
        if (lastInstr != nullptr && lastInstr->getOp() == IRInstr::ret) {
            return;
        } else {
            // generate function epilogue
			// generate block prologue
			os << "LDP SYMBOLSTABLE" << endl;
			os << &(this->symbolsTable) << endl;
			int local_stack_size = this->symbolsTable.getLocalSize()+16;
			string local_stack_size_string = to_string(local_stack_size);
			os << "    ldp     x29, x30, [sp], " << local_stack_size_string << endl;
            os << endl; // make function ending more visible
        }
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
	Function & currentFunction = cfg->functionsTable.access(this->label);
	int local_stack_size = currentFunction.getLocalSize();
	string local_stack_size_string = string("#") + to_string(local_stack_size);

	os << ".global  " << label << endl;
	os << ".type    " << label << ", %function" << endl;
	os << this->label << ":" << endl;

	// generate block prologue
	os << "STP SYMBOLSTABLE" << endl;
	os << &(this->symbolsTable) << endl;
	os << "    stp     x29, x30, [sp, -" << local_stack_size_string << "]!" << endl;
	os << "    mov     x29, sp" << endl;

	// copy eventual passed parameters to local variables
	vector<string> paramsRegisters = {"w0", "w1", "w2", "w3", "w4", "w5"};
	vector<string> paramsName = this->cfg->functionsTable.access(this->label).getParamsName();
	for (int i = 0; i < paramsName.size(); ++i ) {
		Symbol & param = this->symbolsTable.access(paramsName.at(i));
		string reg = paramsRegisters.at(i);
		os << "    str     "<< reg << ", " << param.getAdressAarch64() << endl;
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
	case IRInstr::bit_not:
		this->gen_asm_bit_not_aarch64(os);
		break;
	case IRInstr::incr_prefix:
		this->gen_asm_incr_prefix_aarch64(os);
		break;
	case IRInstr::incr_postfix:
		this->gen_asm_incr_postfix_aarch64(os);
		break;
	case IRInstr::decr_prefix:
		this->gen_asm_decr_prefix_aarch64(os);
		break;
	case IRInstr::decr_postfix:
		this->gen_asm_decr_postfix_aarch64(os);
		break;
	case IRInstr::shr:
		this->gen_asm_shr_aarch64(os);
		break;
	case IRInstr::shl:
		this->gen_asm_shl_aarch64(os);
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
	os << "    mov     w0, " << const_value << endl;
	os << "    str     w0, " << tempVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_ret_aarch64(ostream &os)
{
	string tempVarName = params.at(0);
	Symbol &tempVar = block->symbolsTable.access(tempVarName);
	os << "    ldr     w0, " << tempVar.getAdressAarch64() << endl;

	os << "LDP SYMBOLSTABLE" << endl;
	os << &(block->symbolsTable) << endl;
	int local_stack_size = block->symbolsTable.getLocalSize()+16;
	string local_stack_size_string = to_string(local_stack_size);
	os << "    ldp     x29, x30, [sp], " << local_stack_size_string << endl;
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

	os << "    ldr     w0, " << tempVar1.getAdressAarch64() << endl;
	os << "    ldr     w1, " << tempVar2.getAdressAarch64() << endl;
	os << "    cmp     w0, w1" << endl;
	os << "    cset    w0, EQ" << endl;
	os << "    str     w0, " << resultVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_diff_aarch64(ostream &os)
{
	string resultVarName = params.at(0);
	string tempVarName1 = params.at(1);
	string tempVarName2 = params.at(2);
	Symbol &resultVar = block->symbolsTable.access(resultVarName);
	Symbol &tempVar1 = block->symbolsTable.access(tempVarName1);
	Symbol &tempVar2 = block->symbolsTable.access(tempVarName2);

	os << "    ldr     w0, " << tempVar1.getAdressAarch64() << endl;
	os << "    ldr     w1, " << tempVar2.getAdressAarch64() << endl;
	os << "    cmp     w0, w1" << endl;
	os << "    cset    w0, NE" << endl;
	os << "    str     w0, " << resultVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_lt_aarch64(ostream &os)
{
	string resultVarName = params.at(0);
	string tempVarName1 = params.at(1);
	string tempVarName2 = params.at(2);
	Symbol &resultVar = block->symbolsTable.access(resultVarName);
	Symbol &tempVar1 = block->symbolsTable.access(tempVarName1);
	Symbol &tempVar2 = block->symbolsTable.access(tempVarName2);

	os << "    ldr     w0, " << tempVar1.getAdressAarch64() << endl;
	os << "    ldr     w1, " << tempVar2.getAdressAarch64() << endl;
	os << "    cmp     w0, w1" << endl;
	os << "    cset    w0, LT" << endl;
	os << "    str     w0, " << resultVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_le_aarch64(ostream &os)
{
	string resultVarName = params.at(0);
	string tempVarName1 = params.at(1);
	string tempVarName2 = params.at(2);
	Symbol &resultVar = block->symbolsTable.access(resultVarName);
	Symbol &tempVar1 = block->symbolsTable.access(tempVarName1);
	Symbol &tempVar2 = block->symbolsTable.access(tempVarName2);

	os << "    ldr     w0, " << tempVar1.getAdressAarch64() << endl;
	os << "    ldr     w1, " << tempVar2.getAdressAarch64() << endl;
	os << "    cmp     w0, w1" << endl;
	os << "    cset    w0, LE" << endl;
	os << "    str     w0, " << resultVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_copy_aarch64(ostream &os)
{
	string dest = params.at(0);
	string src = params.at(1);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &srcVar = block->symbolsTable.access(src);

	os << "    ldr     w0, " << srcVar.getAdressAarch64() << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_add_aarch64(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	os << "    ldr     w0, " << v1Var.getAdressAarch64() << endl;
	os << "    ldr     w1, " << v2Var.getAdressAarch64() << endl;
	os << "    add     w0, w0, w1" << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_neg_aarch64(ostream &os)
{
	string dest = params.at(0);
	string src = params.at(1);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &srcVar = block->symbolsTable.access(src);

	os << "    ldr     w0, " << srcVar.getAdressAarch64() << endl;
	os << "    neg     w0, w0" << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_not_aarch64(ostream &os)
{
	string dest = params.at(0);
	string src = params.at(1);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &srcVar = block->symbolsTable.access(src);
	
	os << "    ldr     w0, " << srcVar.getAdressAarch64() << endl;
	os << "    cmp     w0, wzr" << endl;
	os << "    cset    w0, EQ" << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_sub_aarch64(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	os << "    ldr     w0, " << v1Var.getAdressAarch64() << endl;
	os << "    ldr     w1, " << v2Var.getAdressAarch64() << endl;
	os << "    sub     w0, w0, w1" << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_mul_aarch64(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	os << "    ldr     w0, " << v1Var.getAdressAarch64() << endl;
	os << "    ldr     w1, " << v2Var.getAdressAarch64() << endl;
	os << "    mul     w0, w0, w1" << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_div_aarch64(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	os << "    ldr     w0, " << v1Var.getAdressAarch64() << endl;
	os << "    ldr     w1, " << v2Var.getAdressAarch64() << endl;
	os << "    sdiv    w0, w0, w1" << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_mod_aarch64(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	os << "    ldr     w0, " << v1Var.getAdressAarch64() << endl;
	os << "    ldr     w1, " << v2Var.getAdressAarch64() << endl;
	os << "    sdiv    w2, w0, w1" << endl;
	os << "    msub    w3, w2, w1, w0" << endl;
	os << "    str     w3, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_and_aarch64(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	os << "    ldr     w0, " << v1Var.getAdressAarch64() << endl;
	os << "    ldr     w1, " << v2Var.getAdressAarch64() << endl;
	os << "    and     w0, w0, w1" << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_xor_aarch64(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	os << "    ldr     w0, " << v1Var.getAdressAarch64() << endl;
	os << "    ldr     w1, " << v2Var.getAdressAarch64() << endl;
	os << "    eor     w0, w0, w1" << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_or_aarch64(ostream &os)
{
	string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	os << "    ldr     w0, " << v1Var.getAdressAarch64() << endl;
	os << "    ldr     w1, " << v2Var.getAdressAarch64() << endl;
	os << "    orr     w0, w0, w1" << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_call_aarch64(ostream &os) {
	string functionName = params.at(0);
	string destVarName = params.at(1);
	Function & function = this->block->cfg->functionsTable.access(functionName);
	Symbol & destVar = this->block->symbolsTable.access(destVarName);
	
	// max 6 arguments for now
	vector<string> paramsRegisters = {"w0", "w1", "w2", "w3", "w4", "w5"};
	// move parameters to appropriate registers
	for (int i = 2; i < params.size(); ++i) {
		Symbol & param = this->block->symbolsTable.access(params.at(i));
		string reg = paramsRegisters.at(i-2);
		os << "    ldr     "<< reg << ", " << param.getAdressAarch64() << endl;
	}

	// execute call
	os << "    bl      " << functionName << endl;

	// handle return value 
	if (function.getType() != VOID) {
		os << "    str     w0, " << destVar.getAdressAarch64() << endl;
	}
}

void IRInstr::gen_asm_ldchar_aarch64(ostream &os) {
	string value = params.at(0);
	int ascii = (int)value[1];
	string tempVarName = params.at(1);
	Symbol &tempVar = block->symbolsTable.access(tempVarName);
	string char_value = string("#") + to_string(ascii);
	os << "    mov     w0, " << char_value << endl;
	os << "    str     w0, " << tempVar.getAdressAarch64() << endl;
}


void IRInstr::gen_asm_shl_aarch64(ostream &os) {
    string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	os << "    ldr     w0, " << v1Var.getAdressAarch64() << endl;
	os << "    ldr     w1, " << v2Var.getAdressAarch64() << endl;
	os << "    lsl     w0, w0, w1" << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_shr_aarch64(ostream &os) {
    string dest = params.at(0);
	string v1 = params.at(1);
	string v2 = params.at(2);
	Symbol &destVar = block->symbolsTable.access(dest);
	Symbol &v1Var = block->symbolsTable.access(v1);
	Symbol &v2Var = block->symbolsTable.access(v2);

	os << "    ldr     w0, " << v1Var.getAdressAarch64() << endl;
	os << "    ldr     w1, " << v2Var.getAdressAarch64() << endl;
	os << "    lsr     w0, w0, w1" << endl;
	os << "    str     w0, " << destVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_incr_prefix_aarch64(ostream &os) {
    string varName = params.at(0);
    Symbol &var = block->symbolsTable.access(varName);
    os << "    ldadd   wzr, #1, " << var.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_decr_prefix_aarch64(ostream &os) {
    string varName = params.at(0);
    Symbol &var = block->symbolsTable.access(varName);
    os << "    ldadd   wzr, #-1, " << var.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_incr_postfix_aarch64(ostream &os) {
    string dest = params.at(0);
    string src = params.at(1);
    Symbol &destVar = block->symbolsTable.access(dest);
    Symbol &srcVar = block->symbolsTable.access(src);

    os << "    ldr    w0, " << srcVar.getAdressAarch64() << endl;
    os << "    str    w0, " << destVar.getAdressAarch64() << endl;
    os << "    ldadd   wzr, #1, " << srcVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_decr_postfix_aarch64(ostream &os) {
    string dest = params.at(0);
    string src = params.at(1);
    Symbol &destVar = block->symbolsTable.access(dest);
    Symbol &srcVar = block->symbolsTable.access(src);

    os << "    ldr    w0, " << srcVar.getAdressAarch64() << endl;
    os << "    str    w0, " << destVar.getAdressAarch64() << endl;
    os << "    ldadd   wzr, #-1, " << srcVar.getAdressAarch64() << endl;
}

void IRInstr::gen_asm_bit_not_aarch64(ostream &os) {
    string dest = params.at(0);
    string src = params.at(1);
	Symbol &destVar = block->symbolsTable.access(dest);
    Symbol &srcVar = block->symbolsTable.access(src);

	os << "    ldr     w0, " << srcVar.getAdressAarch64() << endl;
	os << "    mvn     w1, w0" << endl;
	os << "    str     w1, " << destVar.getAdressAarch64() << endl;
}