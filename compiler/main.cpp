#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"

#include "VariableVisitorV2.h"
#include "IRVisitor.h"

using namespace antlr4;
using namespace std;

int main(int argn, const char **argv) {
  stringstream in;
  if (argn == 2) {
    ifstream lecture(argv[1]);
    if (!lecture.good()) {
      cerr << "error: cannot read file: " << argv[1] << endl;
      exit(1);
    }
    in << lecture.rdbuf();
  } else {
    cerr << "usage: ifcc path/to/file.c" << endl;
    exit(1);
  }

  ANTLRInputStream input(in.str());

  ifccLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();

  ifccParser parser(&tokens);
  tree::ParseTree *tree = parser.axiom();

  if (parser.getNumberOfSyntaxErrors() != 0) {
    cerr << "\e[31mError: syntax error during parsing\e[39m" << endl;
    exit(1);
  }

  // clog << "Analyse des variables : " << endl;

	VariableVisitorV2 vv;
	vv.visit(tree);

	if (vv.getError())
	{
		exit(1);
	}

  // clog << endl << "Sortie : " << endl;

  IRVisitor irVisitor(tree);
  irVisitor.visit(tree);
  // asm output
  ofstream ofs("output.s");
  irVisitor.cfg->gen_asm(ofs);
  irVisitor.cfg->gen_asm(cout);

  // cfg debugging
  ofstream debug_ofs("debug.ir");
  debug_ofs << *(irVisitor.cfg) << endl;

  return 0;
}
