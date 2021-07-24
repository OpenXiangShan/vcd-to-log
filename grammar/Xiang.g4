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
    | expression op=BitAnd expression
    | expression op=BitOr expression
    | op=Not expression
    | op=Reverse expression
    | signal
    ;

signal
    : instance_path '.' Identifier
    | Constant
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

Constant
    : '0x' [a-f0-9]+ 
    | '0X' [A-F0-9]+
    | '0b'  [0|1]+
    | '0o'  [0-7]+
    | [0-9]+
    ;

Mul : '*' ;
Div : '/' ;
Add : '+' ;
Minus : '-' ;
And : '&&' ;
Or : '||' ;
Equal : '==';
NotEqual : '!=';
Not : '!';
Reverse : '~';
BitAnd : '&';
BitOr  : '|';

WS : [ \r\n\t]+ -> skip;
