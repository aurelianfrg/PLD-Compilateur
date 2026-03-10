grammar ifcc;

// Program
axiom : prog EOF ;

prog : 'int' 'main' '(' ')' block ;

// Block
block : '{' stmt* '}' ; 

stmt : declaration_stmt 
    | assign_stmt 
    | return_stmt 
    | expr ;

declaration_stmt : 'int' declaration_item (',' declaration_item)* ';' ;
declaration_item : VAR ('=' expr)? ;
assign_stmt : VAR '=' expr ';' ;
return_stmt : 'return' expr ';' ;

// Expression
expr : '(' expr ')'         #parenthese
    |   '-' expr            #minus
    |   expr '*' expr       #multiplication
    |   expr ('+'|'-') expr #addSub
    |   literal             #literal_expr;


// Literals and identifiers
literal : CONST 
    | VAR ;

CONST : [0-9]+ ;
VAR : [a-zA-Z]+;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
