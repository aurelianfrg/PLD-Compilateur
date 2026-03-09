#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "SymbolsTable.h"



class CodeGenVisitor : public ifccBaseVisitor {
	public:
		CodeGenVisitor() : ifccBaseVisitor() {
			filename = "output.s";
			ofs = ofstream(filename);
			currentOffset = 0;
		}
		// CodeGenVisitor(string filename) : ifccBaseVisitor(), filename(filename) {
		//         ofs = ofstream(filename);
		// }
		virtual std::any visitProg(ifccParser::ProgContext *ctx) override;

		virtual std::any visitInstruction_aff_stmt(ifccParser::Instruction_aff_stmtContext *ctx) override;

		virtual std::any visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx) override;

		virtual std::any visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx) override;

		virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;

		virtual std::any visitExpr_const(ifccParser::Expr_constContext *ctx) override;

		virtual std::any visitExpr_var(ifccParser::Expr_varContext *ctx) override;

		virtual std::any visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx) override;

		virtual std::any visitExpr_minus(ifccParser::Expr_minusContext *ctx) override;

		virtual std::any visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx) override;

		virtual std::any visitExpr_mult(ifccParser::Expr_multContext *ctx) override;

		virtual std::any visitAff_stmt(ifccParser::Aff_stmtContext *ctx) override;

		virtual std::any visitDef_stmt(ifccParser::Def_stmtContext *ctx) override;

		void undeclaredVariableError(const string & var) {
			clog << "ERROR : Undeclared var " << var << endl;
			noError = false;
		}
		void redeclarationError(const string & var) {
			clog << "ERROR : Redeclaration of var " << var << endl;
			noError = false;
		}
		void uninitializedVariableError(const string & var) {
			clog << "ERROR : Uninitialized var " << var << endl;
			noError = false;
		}
		int newVarOffset(string type) {                       
			currentOffset -= typeSizes.at(type);
			return currentOffset;
		}
		// string getAvailableRegistry() {
		// 	// get a registry to perform operations on
		// 	// registry must be explicitely returned to be used again later
		// 	for (string & registry : registries) {
		// 		if (availableRegistry(registry)) {
		// 			availableRegistries.at(registry) = false;
		// 			return registry;
		// 		}
		// 	}
		// 	cerr << "ERROR : No available registry to perform operation" << endl;
		// 	exit(-1);
		// }
		// bool availableRegistry(const string & registry) {
		// 	return availableRegistries.at(registry);
		// }
		// void returnRegistry(const string & registry) {
		// 	if (availableRegistries.find(registry) != availableRegistries.end()){
		// 		availableRegistries.at(registry) = true;
		// 	}
		// }
		Symbol & newTemporaryVariable(string type) {
			string varName = string("!tmp") + to_string(temporaryVarCount++);
			symbolsTable.add(Symbol(varName, newVarOffset(type), true));
			return symbolsTable.access(varName);
		}

		void move_op(string source, string target) {
			cout << "    movl    " << source << ", " << target << endl;
		}
		void add_op(string expr1Reg, string expr2) {
			cout << "    addl    " << expr2 << ", " << expr1Reg << endl;
		}
		void sub_op(string expr1Reg, string expr2) {
			// performs expr1Reg - expr2 --> expr1Reg
			cout << "    subl    " << expr2 << ", " << expr1Reg << endl;
		}
		void minus_op(string expr) {
			cout << "    negl    " << expr << endl;
		}
		void mult_op(string expr1Reg, string expr2){
			cout << "    imull    " << expr2 << ", " << expr1Reg << endl;
		}

	protected:
		SymbolsTable symbolsTable;
		string filename;
		ofstream ofs;
		bool noError;
		int currentOffset;
		int temporaryVarCount = 0;
		unordered_map<string,int> typeSizes = {
			{"int", 4}
		};
		// vector<string> registries = {"%r8d","%r9d","%r10d","%r11d","%r12d","%r13d","%r14d","%r15d"};
		// unordered_map<string, bool> availableRegistries = {
		// 	{"%r8d", true},
		// 	{"%r9d", true},
		// 	{"%r10d", true},
		// 	{"%r11d", true},
		// 	{"%r12d", true},
		// 	{"%r13d", true},
		// 	{"%r14d", true},
		// 	{"%r15d", true},
		// };
	};

