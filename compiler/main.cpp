#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"

#include "VariableVisitor.h"
#include "IRVisitor.h"

using namespace antlr4;
using namespace std;

int main(int argn, const char **argv) {
  stringstream in;
  TargetArchi archi = amd64;
  if (argn >= 2 and argn <= 3) {
    ifstream lecture(argv[1]);
    if (!lecture.good()) {
      cerr << "error: cannot read file: " << argv[1] << endl;
      exit(1);
    }
    in << lecture.rdbuf();
  } else {
    cerr << "usage: ifcc path/to/file.c [target]" << endl;
    exit(1);
  }
  
  if (argn == 3) {
	string target(argv[2]);
	if (target == "amd64") 		{ archi == amd64; }
	else if (target == "aarch64")
	{ 
		archi == aarch64; 
		cerr << "[WIP] aarch64 compilation is not yet implemented" << endl; 
		exit(1); 
	}
	else 
	{
		cerr << "Invalid target. Compilation targets are" << endl;
		cerr << " - amd64" << endl;
		cerr << " - aarch64" << endl;
    	exit(1);
	}
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

	VariableVisitor vv;
	vv.visit(tree);

	if (vv.getError())
	{
		exit(1);
	}

  // clog << endl << "Sortie : " << endl;

  IRVisitor irVisitor(tree);
  irVisitor.visit(tree);

  // cfg debugging
  ofstream debug_ofs("debug.ir");
  debug_ofs << *(irVisitor.cfg) << endl;
  ofstream dot_ofs("cfg.dot");
  irVisitor.cfg->toDot(dot_ofs);


  // asm output
  ofstream ofs("output.s");
  switch (archi)
  {
  case amd64:
  	irVisitor.cfg->gen_asm_amd64(ofs);
  	irVisitor.cfg->gen_asm_amd64(cout);
	break;
  case aarch64:
  	irVisitor.cfg->gen_asm_aarch64(ofs);
  	irVisitor.cfg->gen_asm_aarch64(cout);
	break;
  
  default:
	break;
  }

  return 0;
}
