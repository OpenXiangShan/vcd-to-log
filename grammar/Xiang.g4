grammar Xiang;

main
    : event*
    ;

event
    : StringLiteral ':' '{' expression '}'
    ;

expression
    : '(' inner=expression ')'
    | expression op=(Mul | Div) expression
    | expression op=(Add | Minus) expression
    | expression op=(Equal | NotEqual) expression
    | expression op=And expression
    | expression op=Or expression
    | signal
    ;

signal
    : instance_path '.' Identifier
    ;

instance_path
    : Identifier ('.' Identifier)*
    ;

Identifier
    : [a-zA-Z_] [a-zA-Z0-9_]*
    ;

StringLiteral
    : '"' [a-zA-Z0-9_ ]+ '"'
    ;

Mul : '*' ;
Div : '/' ;
Add : '+' ;
Minus : '-' ;
And : '&&' ;
Or : '||' ;
Equal : '==';
NotEqual : '!=';

WS : [ \r\n\t]+ -> skip;
