/*
 * Lexer para uma calculadora avançada
 */

/* Reconhecimento de tokens para a calculadora */

%option noyywrap nodefault yylineno

%{
  #include "bison-calc.h"
  #include "bison-calc.tab.h"
%}

/* Expressão para expoente float */
EXP ([Ee][+-]?[0-9]+)

%%

/* Operadores de caractere único */
"+"        { return yytext[0]; }
"-"        { return yytext[0]; }
"*"        { return yytext[0]; }
"/"        { return yytext[0]; }
"="        { return yytext[0]; }
","        { return yytext[0]; }
";"        { return yytext[0]; }
"("        { return yytext[0]; }
")"        { return yytext[0]; }

/* Operadores de comparação, todos retornam token CMP */
">"        { yylval.fn = 1; return CMP; }
"<"        { yylval.fn = 2; return CMP; }
"<>"       { yylval.fn = 3; return CMP; }
"=="       { yylval.fn = 4; return CMP; }
">="       { yylval.fn = 5; return CMP; }
"<="       { yylval.fn = 6; return CMP; }

/* Palavras-chave */
"if"       { return IF; }
"then"     { return THEN; }
"else"     { return ELSE; }
"while"    { return WHILE; }
"do"       { return DO; }
"let"      { return LET; }

/* Funções pré-definidas */
"sqrt"     { yylval.fn = Bsqrt; return FUNC; }
"exp"      { yylval.fn = Bexp;  return FUNC; }
"log"      { yylval.fn = Blog;  return FUNC; }
"print"    { yylval.fn = Bprint; return FUNC; }

/* Nomes de variáveis e funções definidas pelo usuário */
[a-zA-Z][a-zA-Z0-9]*   { yylval.s = lookup(yytext); return NAME; }

/* Números (com ponto decimal e opcionalmente expoente) */
[0-9]+"."[0-9]*{EXP}?  |
"."?[0-9]+{EXP}?       { yylval.d = atof(yytext); return NUMBER; }

/* Comentários de linha */
"//".*                 { /* ignorar */ }

/* Espaços em branco */
[ \t]                  { /* ignorar */ }

/* Continuação de linha com barra invertida */
\\\n                   { printf("c> "); }

/* Fim de linha */
\n                     { return EOL; }

/* Qualquer outro caractere gera erro */
.                      { yyerror("Caractere desconhecido %c\n", *yytext); }

%%