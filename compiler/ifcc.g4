grammar ifcc;

axiom: prog EOF;

prog: function_def+ ;

function_def: type_function VAR '(' ((TYPE VAR ',')* TYPE VAR)? ')' bloc ; 

bloc: '{' instruction* '}';

instruction:
	return_stmt		# instruction_return_stmt
	| def_stmt		# instruction_def_stmt
    | if_stmt 		# instruction_if_stmt
	| while_stmt 	# instruction_while_stmt
	| expr ';'		# instruction_expr 
;

expr:
	'(' expr ')'								# expr_parenthesis
	| call										# expr_call
	| OP=('-' | '!') expr						# expr_minus_not
	| expr OP = ('*' | '/' | '%') expr			# expr_mult_div_mod
	| expr OP = ('+' | '-') expr				# expr_add_sub
	| expr OP = ('<=' | '<' | '>=' | '>') expr	# expr_comp
	| expr OP = ('==' | '!=') expr				# expr_eq_diff
	| expr '&' expr								# expr_and
	| expr '^' expr								# expr_xor
	| expr '|' expr								# expr_or
	| VAR '=' expr								# expr_aff
	| CONST										# expr_const
	| VAR										# expr_var;

return_stmt: RETURN expr ';';

aff_stmt: VAR '=' expr ';';

def_stmt: TYPE def_item (',' def_item)* ';';
def_item: VAR ('=' expr)?;

if_stmt :
    'if' '(' expr ')' bloc ('else' 'if' '(' expr ')' bloc)* ('else' bloc)?;

while_stmt :
	'while' '(' expr ')' bloc ;

call : VAR '(' ((expr ',')* expr)? ')' ; 

type_function: 'void' | TYPE ;
TYPE: 'int';
RETURN: 'return';
CONST: [0-9]+;
VAR: [a-zA-Z_][a-zA-Z0-9_]*;

COMMENT: '/*' .*? '*/' -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);