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

// usage: ifcc path/to/file.c [options]

// options :
// -h / --help 
// -t / --target x86 | arm
// -d / --debug
// -a / --assemble 
// -o / --output output_filename

int main(int argc, const char **argv) {

	string program = ""; 
	string output_filename = "";
	TargetArchi archi = amd64;
	bool debug = false;
	bool assemble = false;
	int current_arg = 1;

	while (current_arg < argc) {
		string arg = argv[current_arg];
		if (arg == "-h" or arg == "--help") {
			cout << "usage: ifcc path/to/file.c [options]" << endl;
			cout << "options :" << endl;
			cout << "-h / --help " << endl;
			cout << "-t / --target x86 | arm" << endl;
			cout << "-d / --debug" << endl;
			cout << "-a / --assemble " << endl;
			cout << "-o / --output output_filename" << endl;
			exit(0);
		}
		else if (arg == "-t" or arg == "--target") {
			++current_arg;
			arg = argv[current_arg];
			if (arg == "x86") archi = amd64;
			else if (arg == "arm") archi = aarch64;
			else {
				cout << "Incorrect value for option -t / --target : must be 'x86' or 'arm'" << endl;
				exit(-1);
			}
		}
		else if (arg == "-o" or arg == "--output") {
			++current_arg;
			arg = argv[current_arg];
			output_filename = arg;
		}
		else if (arg == "-d" or arg == "--debug") {
			debug = true;
		}
		else if (arg == "-a" or arg == "--assemble") {
			assemble = true;
		}
		else {
			// argument specified without option means it is the program file
			program = arg;
		}
		++current_arg;
	}

	// check input sanity
	if (program == "") {
		cout << "usage: ifcc path/to/file.c [options]" << endl;
		exit(-1);
	}

	clog << "ifcc " << program << " architecture " << archi << " debug " << debug << " assemble " << assemble << endl;

	stringstream in;
	ifstream lecture(argv[1]);
	if (!lecture.good()) {
		cerr << "error: cannot read file: " << argv[1] << endl;
		exit(1);
	}
	in << lecture.rdbuf();

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

	VariableVisitor vv;
	vv.visit(tree);

	if (vv.getError())
	{
		exit(1);
	}

	IRVisitor irVisitor(tree);
	irVisitor.visit(tree);

	// cfg debugging
	if (debug) {
		ofstream debug_ofs("debug.ir");
		debug_ofs << *(irVisitor.cfg) << endl;
		ofstream dot_ofs("cfg.dot");
		irVisitor.cfg->toDot(dot_ofs);
		popen("dot -Tpng cfg.dot -o cfg.dot.png", "w");
	}

	// asm output
	ofstream ofs;
	if (output_filename == "" or assemble) {
		ofs = ofstream("output.s");
	}
	else {
		ofs = ofstream(output_filename);
	}

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

	if (output_filename == "") output_filename = "a.out";

	if (assemble) {
		stringstream command;
		command << "gcc output.s -o " << output_filename;
		popen(command.str().c_str(), "w");
	}

	return 0;
}
