#pragma once

#include <iostream>
#include <string>

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

                virtual std::any visitInstruction_def_aff_stmt(ifccParser::Instruction_def_aff_stmtContext *ctx) override;

                virtual std::any visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx) override;

                virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;

                virtual std::any visitExpr_const(ifccParser::Expr_constContext *ctx) override;

                virtual std::any visitExpr_var(ifccParser::Expr_varContext *ctx) override;

                virtual std::any visitAff_stmt(ifccParser::Aff_stmtContext *ctx) override;

                virtual std::any visitDef_stmt(ifccParser::Def_stmtContext *ctx) override;

                virtual std::any visitDef_aff_stmt(ifccParser::Def_aff_stmtContext *ctx) override;

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

        protected:
                SymbolsTable symbolsTable;
                string filename;
                ofstream ofs;
                bool noError;
                int currentOffset;
                unordered_map<string,int> typeSizes = {
                        {"int", 4}
                };
        };

