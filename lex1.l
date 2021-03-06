%{
  #include "IOMngr.h"
  #include "y.tab.h"

  #define YY_INPUT(buf,result,max_size)\
  { int c = getNextSourceChar();\
  result = (c == EOF) ? YY_NULL : (buf[0] = c, 1);\
  }

%}

letter [A-Za-z]
digit [0-9]

%%

if                            {return IF;}
else                          {return ELSE;}
true                          {return TRUE;}
false                         {return FALSE;}
while                         {return WHILE;}
for                           {return FOR;}
int                           {return Int;}
boolean                       {return Boolean;}
read                          {return Read;}
print                         {return Write;}
printspaces                   {return WriteSpaces;}
printlines                    {return WriteLines;}
{letter}({letter}|{digit})*   {return Ident;}
{digit}{digit}*               {return IntLit;}
\=\=                          {return EQ;}
\=                            {return '=';}
\+                            {return '+';}
\-                            {return '-';}
\*                            {return '*';}
\/                            {return '/';}
\>                            {return '>';}
\<                            {return '<';}
\!                            {return '!';}
\&                            {return '&';}
\|                            {return '|';}
\%                            {return '%';}
\,                            {return ',';}
\^                            {return '^';}
\;                            {return ';';}
\{                            {return '{';}
\}                            {return '}';}
\[                            {return '[';}
\]                            {return ']';}
\(                            {return '(';}
\)                            {return ')';}
[ ]                           {}
\t                            {}
\r                            {}
\n                            {}
.                             {writeIndicator(getCurrentColumnNum());
                               writeMessage("Illegal Character in lex");}

%%

int yywrap () {
 return 1;
}
