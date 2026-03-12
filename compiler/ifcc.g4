grammar ifcc;

axiom: prog EOF;

prog: 'int' 'main' '(' ')' bloc;

bloc: '{' instruction* '}';

instruction:
	return_stmt	# instruction_return_stmt
	| def_stmt	# instruction_def_stmt
	| aff_stmt	# instruction_aff_stmt
	| if_stmt	# instruction_if_stmt;

expr:
	OPEN_PAR expr CLOSE_PAR						# expr_parenthesis
	| '-' expr									# expr_minus
	| expr OP = ('*' | '/' | '%') expr			# expr_mult_div_mod
	| expr OP = ('+' | '-') expr				# expr_add_sub
	| expr OP = ('<=' | '<' | '>=' | '>') expr	# expr_comp
	| expr OP = ('==' | '!=') expr				# expr_eq_diff
	| expr '&' expr								# expr_and
	| expr '^' expr								# expr_xor
	| expr '|' expr								# expr_or
	| VAR '=' expr								# expr_aff // affectations should return a value
	| CONST										# expr_const
	| VAR										# expr_var;

return_stmt: RETURN expr ';';

aff_stmt: VAR '=' expr ';';

def_stmt: TYPE VAR ('=' expr)? ';';

if_stmt:
	'if' '(' IF_EXPR = expr ')' IF_BLOC = bloc (
		'else' 'if' '(' ELSEIF_EXPR = expr ')' ELSEIF_BLOC = bloc
	)*? ('else' ELSE_BLOC = bloc)?;

TYPE: 'int';
OPEN_PAR: '(';
CLOSE_PAR: ')';
RETURN: 'return';
CONST: [0-9]+;
VAR: [a-zA-Z_][a-zA-Z0-9_]*;

COMMENT: '/*' .*? '*/' -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);