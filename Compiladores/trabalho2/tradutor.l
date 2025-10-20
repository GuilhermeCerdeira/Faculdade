%option noyywrap

%{
#include <string.h>
#include "tradutor.tab.h"
void yyerror(const char *s);
%}

%%
"\"agentCode\""     { return AGENTCODE_KEY; }
"\"beliefs\""       { return BELIEFS_KEY; }
"\"goal\""          { return GOAL_KEY; }
"\"plans\""         { return PLANS_KEY; }
"\"trigger\""       { return TRIGGER_KEY; }
"\"ctx\""           { return CTX_KEY; }
"\"body\""          { return BODY_KEY; }

"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
"["                 { return LBRACK; }
"]"                 { return RBRACK; }
","                 { return COMMA; }
":"                 { return COLON; }

\"[^\"\n]*\"        {
                        yytext[strlen(yytext)-1] = '\0';
                        yylval.sval = strdup(yytext + 1);
                        return STRING;
                    }

[ \t\n]+            ;

.                   { 
                        fprintf(stderr, "ERRO LEXICO: Caractere invalido encontrado: '%c' (Codigo ASCII: %d)\n", yytext[0], yytext[0]);
                        yyerror("Verifique o arquivo de entrada 'entrada.json'."); 
                    }
%%