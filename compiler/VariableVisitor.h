#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include <unordered_set>

using namespace std;

class VariableVisitor : public ifccBaseVisitor {
	public:
        virtual std::any visitAff_stmt_var(ifccParser::Aff_stmt_varContext *ctx) override {
            string var1 = ctx->VAR().at(0)->getText();
            string var2 = ctx->VAR().at(1)->getText();
            clog << "aff " << var1 << " = " << var2 << endl;

            if (!exists(var2)) {
                undeclaredVariableError(var2);
            }
            use(var2);
            if (!exists(var1)) {
                undeclaredVariableError(var1);
            }

            return visitChildren(ctx);
        }

        virtual std::any visitAff_stmt_const(ifccParser::Aff_stmt_constContext *ctx) override {
            string var = ctx->VAR()->getText();
            clog << "def " << var << " = " << ctx->CONST()->getText() << endl;

            if (!exists(var)) {
                undeclaredVariableError(var);
            }

            return visitChildren(ctx);
        }

        virtual std::any visitDef_stmt(ifccParser::Def_stmtContext *ctx) override {
            string var = ctx->VAR()->getText();
            clog << "def " << var << endl;

            if (exists(var)) {
                redeclarationError(var);
            }
            add(var);

            return visitChildren(ctx);
        }

        virtual std::any visitDef_aff_stmt_const(ifccParser::Def_aff_stmt_constContext *ctx) override {
            string var = ctx->VAR()->getText();
            clog << "def aff const " << var << " = " << ctx->CONST()->getText() << endl;
            if (exists(var)) {
                redeclarationError(var);
            }
            add(var);

            return visitChildren(ctx);
        }

        virtual std::any visitDef_aff_stmt_var(ifccParser::Def_aff_stmt_varContext *ctx) override {
            string var1 = ctx->VAR().at(0)->getText();
            string var2 = ctx->VAR().at(1)->getText();
            clog << "def aff var " << var1 << " = " << var2 << endl;

            if (!exists(var2)) {
                undeclaredVariableError(var2);
            }
            use(var2);
            if (exists(var1)) {
                redeclarationError(var1);
            }
            add(var1);

            return visitChildren(ctx);
        }

        virtual std::any visitReturn_stmt_var(ifccParser::Return_stmt_varContext *ctx) override {
            string var = ctx->VAR()->getText();
            if (!exists(var)) undeclaredVariableError(var);
            use(var);

            return visitChildren(ctx);
        }

    protected:
        bool exists(const string & var) {
            return (declared_variables.find(var) != declared_variables.end());
        } 
        void add(const string & var) {
            declared_variables.insert(var);
        } 
        bool used(const string & var) {
            return (used_variables.find(var) != used_variables.end());
        }
        void use(const string & var) {
            used_variables.insert(var);
        }
        bool anyUnused() {
            bool result = false;
            for (const string & var: declared_variables) {
                if (!used(var)) {
                    result = true;
                    clog << "WARNING : Unused var" << var << endl;
                }
            }
            return result;
        }
        void undeclaredVariableError(const string & var) {
            clog << "ERROR : Undeclared var " << var << endl;
            noError = false;
        }
        void redeclarationError(const string & var) {
            clog << "ERROR : Redeclaration of var " << var << endl;
            noError = false;
        }
        unordered_set<string> declared_variables;
        unordered_set<string> used_variables;
        bool noError = true;
};