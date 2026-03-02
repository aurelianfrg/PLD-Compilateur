grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' bloc ;

bloc : '{' instruction* '}' ;

instruction : 
    aff_stmt #affectation_statement
    |def_stmt #definition_statement
    |def_aff_stmt #definition_affectation_statement
    |return_stmt #return_statement
;

return_stmt : 
    RETURN CONST ';' #return_const
    |RETURN VAR ';' #return_var
;

aff_stmt : 
    VAR '=' VAR ';' #affectation_var
    |VAR '=' CONST ';' #affectation_const
;

def_stmt :
    TYPE VAR ';' ;

def_aff_stmt : 
    TYPE VAR '=' CONST ';' #definition_affectation_const
    |TYPE VAR '=' VAR ';'  #definition_affectation_var
;

TYPE : 'int' ;
RETURN : 'return' ;
CONST : [0-9]+ ;
VAR : [a-zA-Z_][a-zA-Z0-9_]* ;

COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
