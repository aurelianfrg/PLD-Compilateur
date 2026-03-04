grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' bloc ;

bloc : '{' instruction* '}' ;

instruction : 
    aff_stmt #instruction_aff_stmt
    |def_stmt #instruction_def_stmt
    |def_aff_stmt #instruction_def_aff_stmt
    |return_stmt #instruction_return_stmt
;

return_stmt : 
    RETURN expr ';' 
;

expr :
    expr '*' expr #expr_mult
    |expr '-' expr #expr_sub
    |expr '+' expr #expr_add
    |CONST #expr_const
    |VAR #expr_var
;

aff_stmt : 
    VAR '=' expr ';'
;

def_stmt :
    TYPE VAR ';' ;

def_aff_stmt : 
    TYPE VAR '=' expr ';'
;

TYPE : 'int' ;
RETURN : 'return' ;
CONST : [0-9]+ ;
VAR : [a-zA-Z_][a-zA-Z0-9_]* ;

COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
