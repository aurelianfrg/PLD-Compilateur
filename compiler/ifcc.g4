grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' bloc ;

bloc : '{' instruction* '}' ;

instruction : 
    return_stmt #instruction_return_stmt
    |def_stmt #instruction_def_stmt
    |aff_stmt #instruction_aff_stmt
;

expr :
    OPEN_PAR expr CLOSE_PAR #expr_parenthesis
    |'-' expr #expr_minus
    |expr '*' expr #expr_mult
    |expr ('+' | '-') expr #expr_add_sub
    |CONST #expr_const
    |VAR #expr_var
;

return_stmt : 
    RETURN expr ';' 
;

aff_stmt : 
    VAR '=' expr ';'
;

def_stmt :
    TYPE VAR ('=' expr )? ';' ;

TYPE : 'int' ;
OPEN_PAR : '(' ;
CLOSE_PAR : ')' ;
RETURN : 'return' ;
CONST : [0-9]+ ;
VAR : [a-zA-Z_][a-zA-Z0-9_]* ;

COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
