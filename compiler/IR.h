#pragma once

#include <initializer_list>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"

using namespace antlr4;
using namespace std;

class Block;
class BasicBlock;
class FunctionBlock;
class CFG;
class IRInstr;

#include "FunctionsTable.h"
#include "Symbol.h"
#include "SymbolsTable.h"
#include "Type.h"


typedef enum {
	amd64, 	// x86
	aarch64 // ARM
} TargetArchi;


//! The class for one 3-address instruction
class IRInstr {
  public:
    /** The instructions themselves -- feel free to subclass instead */
    typedef enum {
        // L'IR manipule les variables par leur nom (temporaire ou pas)
        ldconst,      // VAR<-CONST 	        [const, var]
        ret,          // RETURN EXPR 	        [expr_address]
        copy,         // VAR<-VAR 	 	        [dest, source]
        add,          // VAR<-VAR+VAR  	      [dest, v1, v2]
        sub,          // VAR<-VAR-VAR  	      [dest, v1, v2]
        mul,          // VAR<-VAR*VAR  	      [dest, v1, v2]
        div,          // VAR<-VAR/VAR  	      [dest, v1, v2]
        mod,          // VAR<-VAR%VAR  	      [dest, v1, v2]
        rmem,         // VAR<-*ADDR	 	        [var, addr]
        wmem,         // *ADDR<-VAR	 	        [var, addr]
        call,         // FUNC [->VAR] VAR*	  [func, dest, param1, param2...]
        cmp_eq,       // VAR<-VAR==VAR 	      [dest, v1, v2]
        cmp_diff,     // VAR<-VAR!=VAR 	      [dest, v1, v2]
        cmp_lt,       // VAR<-VAR<VAR  	      [dest, v1, v2]
        cmp_le,       // VAR<-VAR<=VAR 	      [dest, v1, v2]
        neg,          // VAR<- -VAR    	      [dest, source]
        not_,         // VAR<- !VAR    	      [dest, source]
        bit_not,      // VAR<- ~VAR    	      [dest, source]
        bit_and,      // VAR<-VAR&VAR 	      [dest, v1, v2]
        bit_xor,      // VAR<-VAR^VAR 	      [dest, v1, v2]
        bit_or,       // VAR<-VAR|VAR 	      [dest, v1, v2]
        shl,          // VAR<-VAR<<VAR 	      [dest, v1, v2]
        shr,          // VAR<-VAR>>VAR 	      [dest, v1, v2]
        ldchar,       // VAR<-CHAR            [char, var]
        incr_prefix,  // ++VAR      			[var]
        decr_prefix,  // --VAR       			[var]
        incr_postfix, // VAR++      			[dest, source]
        decr_postfix  // VAR--       			[dest, source]
    } Operation;

    /**  constructor */
    IRInstr(Block *b, Operation op, Type t, vector<string> params);

	/** AMD64 code generation */
	void gen_asm_amd64(ostream &os);
	void gen_asm_ldconst_amd64(ostream &os);
	void gen_asm_ret_amd64(ostream &os);
	void gen_asm_copy_amd64(ostream &os);
	void gen_asm_add_amd64(ostream &os);
	void gen_asm_sub_amd64(ostream &os);
	void gen_asm_mul_amd64(ostream &os);
	void gen_asm_div_amd64(ostream &os);
	void gen_asm_mod_amd64(ostream &os);
	void gen_asm_neg_amd64(ostream &os);
	void gen_asm_not_amd64(ostream &os);
	void gen_asm_eq_amd64(ostream &os);
	void gen_asm_diff_amd64(ostream &os);
	void gen_asm_lt_amd64(ostream &os);
	void gen_asm_le_amd64(ostream &os);
	void gen_asm_and_amd64(ostream &os);
	void gen_asm_xor_amd64(ostream &os);
	void gen_asm_or_amd64(ostream &os);
	void gen_asm_call_amd64(ostream &os);
	void gen_asm_ldchar_amd64(ostream &os);
	void gen_asm_bit_not_amd64(ostream &os);
    void gen_asm_shl_amd64(ostream &os);
    void gen_asm_shr_amd64(ostream &os);
    void gen_asm_incr_prefix_amd64(ostream &os);
    void gen_asm_decr_prefix_amd64(ostream &os);
    void gen_asm_incr_postfix_amd64(ostream &os);
    void gen_asm_decr_postfix_amd64(ostream &os);

	/** AARCH64 code generation */
	void gen_asm_aarch64(ostream &os);
	void gen_asm_ldconst_aarch64(ostream &os);
	void gen_asm_ret_aarch64(ostream &os);
	void gen_asm_copy_aarch64(ostream &os);
	void gen_asm_add_aarch64(ostream &os);
	void gen_asm_sub_aarch64(ostream &os);
	void gen_asm_mul_aarch64(ostream &os);
	void gen_asm_div_aarch64(ostream &os);
	void gen_asm_mod_aarch64(ostream &os);
	void gen_asm_neg_aarch64(ostream &os);
	void gen_asm_not_aarch64(ostream &os);
	void gen_asm_eq_aarch64(ostream &os);
	void gen_asm_diff_aarch64(ostream &os);
	void gen_asm_lt_aarch64(ostream &os);
	void gen_asm_le_aarch64(ostream &os);
	void gen_asm_and_aarch64(ostream &os);
	void gen_asm_xor_aarch64(ostream &os);
	void gen_asm_or_aarch64(ostream &os);
	void gen_asm_call_aarch64(ostream &os);
	void gen_asm_ldchar_aarch64(ostream &os);
	void gen_asm_bit_not_aarch64(ostream &os);
    void gen_asm_shl_aarch64(ostream &os);
    void gen_asm_shr_aarch64(ostream &os);
    void gen_asm_incr_prefix_aarch64(ostream &os);
    void gen_asm_decr_prefix_aarch64(ostream &os);
    void gen_asm_incr_postfix_aarch64(ostream &os);
    void gen_asm_decr_postfix_aarch64(ostream &os);


    friend ostream &operator<<(ostream &os, const IRInstr &irInstr);
    void toDot(ostream &os); // generate graph in dot format

    Operation getOp();

  private:
    Block *block; /**< The BB this instruction belongs to, which provides a pointer to the CFG this
                     instruction belong to */
    Operation op;
    Type t;
    vector<string> params; /**< For 3-op instrs: d, x, y; for ldconst: d, c;  For call: label, d,
                              params;  for wmem and rmem: choose yourself */
    // if you subclass IRInstr, each IRInstr subclass has its parameters and the previous (very
    // important) comment becomes useless: it would be a better design.
};

/**  The class for a basic block */

/* A few important comments.
IRInstr has no jump instructions.
cmp_* instructions behaves as an arithmetic two-operand instruction (add or
mult), returning a boolean value (as an int)

Assembly jumps are generated as follows:
BasicBlock::gen_asm() first calls IRInstr::gen_asm() on all its instructions,
and then if  exit_true  is a  nullptr, the epilogue is generated else if
exit_false is a nullptr, an unconditional jmp to the exit_true branch is
generated else (we have two successors, hence a branch) an instruction comparing
the value of test_var_name to true is generated, followed by a conditional
branch to the exit_false branch, followed by an unconditional branch to the
exit_true branch The attribute test_var_name itself is defined when converting
the if, while, etc of the AST  to IR.

Possible optimization:
a cmp_* comparison instructions, if it is the last instruction of its block,
generates an actual assembly comparison
followed by a conditional jump to the exit_false branch
*/

class Block
{
public:
	virtual ~Block();
	
	virtual void gen_asm_amd64(ostream &os) = 0; 
	virtual void gen_asm_aarch64(ostream &os) = 0; 
	void gen_block_linking_asm_amd64(ostream &os, IRInstr *lastInstr);
	void gen_block_linking_asm_aarch64(ostream &os, IRInstr *lastInstr);

    void add_IRInstr(IRInstr::Operation op, Type t, vector<string> params);

    virtual void print(ostream &os) const = 0;
    friend ostream &operator<<(ostream &os, const Block &b);
    void toDot(ostream &os); // generate graph in dot format

    string label;             /**< label of the BB, also will be the label in the generated code */
    CFG *cfg = nullptr;                 /** < the CFG where this block belongs */
    vector<IRInstr *> instrs; /** < the instructions themselves. */
    SymbolsTable symbolsTable;
    BasicBlock *exit_true = nullptr; /**< pointer to the next basic block, true branch. If nullptr,
                                        return from procedure */
    BasicBlock *exit_false =
        nullptr; /**< pointer to the next basic block, false branch. If null_ptr, the basic block
                    ends with an unconditional jump */
    string test_var_name; /** < when generating IR code for an if(expr) or while(expr) etc,
                                                          store here the name of the variable that
                             holds the value of expr */
};

class BasicBlock : public Block
{
public:
	BasicBlock(CFG *cfg, string label, SymbolsTable currentSymbolsTable, bool isAChild);
	void gen_asm_amd64(ostream &os) override; 
	void gen_asm_aarch64(ostream &os) override; 

    virtual void print(ostream &os) const override;
};

class FunctionBlock : public Block
{
public:
	FunctionBlock(CFG *cfg, string label, vector<Type> paramsType, vector<string> paramsName);
	void gen_asm_amd64(ostream &os) override; 
	void gen_asm_aarch64(ostream &os) override; 

    virtual void print(ostream &os) const override;
};

/** The class for the control flow graph, also includes the symbol table */

/* A few important comments:
The entry block is the one with the same label as the AST function name.
(it could be the first of bbs, or it could be defined by an attribute value)
The exit block is the one with both exit pointers equal to nullptr.
(again it could be identified in a more explicit way)

*/
class CFG {
  public:
    CFG(tree::ParseTree *ast);
    virtual ~CFG();

    void add_block(Block *b);
    BasicBlock *createChildBasicBlock(Block *parentBlock, string namePrefix = "block"); // create a new basicblock that inherits its parents Symbols and has a new local table
    BasicBlock *createSiblingBasicBlock(Block *siblingBlock, string namePrefix = "block"); // create a new basicblock that copies its siblings symbols
    FunctionBlock *createFunctionBlock(string label, vector<Type> paramsType, vector<string> paramsName); // create a new function block, with a completely new symbolsTable initialized with its parameters

	// x86 code generation: could be encapsulated in a processor class in a
	// retargetable compiler
	void gen_asm_amd64(ostream &os); 
	void gen_asm_aarch64(ostream &os); 

    // block management
    string new_BB_name(string prefix = "block") { return prefix + "_" + to_string(nextBBnumber); }

    friend ostream &operator<<(ostream &os, const CFG &cfg);
    void toDot(ostream &os); // generate graph in dot format

    void pushContinueBlock(Block *block) { continueBlocks.push(block); }
    void pushBreakBlock(Block *block) { breakBlocks.push(block); }
    Block *topContinueBlock() { return continueBlocks.top(); }
    Block *topBreakBlock() { return breakBlocks.top(); }
    Block *popContinueBlock() {
        Block *topBlock = continueBlocks.top();
        continueBlocks.pop();
        return topBlock;
    }
    Block *popBreakBlock() {
        Block *topBlock = breakBlocks.top();
        breakBlocks.pop();
        return topBlock;
    }
    size_t getContinueBlocksSize() { return continueBlocks.size(); }
    size_t getBreakBlocksSize() { return breakBlocks.size(); }


    tree::ParseTree *ast; /**< The AST this CFG comes from */
    Block *current_block;
    FunctionsTable functionsTable;
    string currentFunctionName;

  protected:
    int nextBBnumber; /**< just for naming */

    vector<Block *> blocks; /**< all the basic blocks of this CFG*/
    stack<Block *> continueBlocks; // stack of Block to jump to when a "continue" statement is encountered;
    stack<Block *> breakBlocks; // stack of Block to jump to when a "break" statement is encountered;
};
