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
    RETURN CONST ';' #return_stmt_const
    |RETURN VAR ';' #return_stmt_var
;

aff_stmt : 
    VAR '=' VAR ';' #aff_stmt_var
    |VAR '=' CONST ';' #aff_stmt_const
;

def_stmt :
    TYPE VAR ';' ;

def_aff_stmt : 
    TYPE VAR '=' CONST ';' #def_aff_stmt_const
    |TYPE VAR '=' VAR ';'  #def_aff_stmt_var
;

TYPE : 'int' ;
RETURN : 'return' ;
CONST : [0-9]+ ;
VAR : [a-zA-Z_][a-zA-Z0-9_]* ;

COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
