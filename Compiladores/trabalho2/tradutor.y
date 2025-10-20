%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);
extern FILE *yyin;
extern int yydebug;

typedef struct StringList { char *str; struct StringList *next; } StringList;
typedef struct Plan { char *name; char *trigger; char *ctx; StringList *body; struct Plan *next; } Plan;
typedef struct Agent { char *id; char *name; StringList *beliefs; char *goal; Plan *plans; struct Agent *next; } Agent;

Agent *program_agents = NULL;

StringList* create_string_node(char* s) { StringList* n = malloc(sizeof(StringList)); if(!n) exit(1); n->str = strdup(s); n->next = NULL; return n; }
Plan* create_plan_node(char* name) { Plan* n = malloc(sizeof(Plan)); if(!n) exit(1); n->name = strdup(name); n->trigger = n->ctx = NULL; n->body = NULL; n->next = NULL; return n; }
Agent* create_agent_node(char* id, char* name) { Agent* n = malloc(sizeof(Agent)); if(!n) exit(1); n->id = strdup(id); n->name = strdup(name); n->beliefs = NULL; n->goal = NULL; n->plans = NULL; n->next = NULL; return n; }

void generate_maspy_code(Agent *agents);
%}

%debug

%union {
    char *sval;
    struct StringList *string_list;
    struct Plan *plan;
    struct Agent *agent;
}

%token LBRACE RBRACE LBRACK RBRACK COMMA COLON
%token AGENTCODE_KEY BELIEFS_KEY GOAL_KEY PLANS_KEY TRIGGER_KEY CTX_KEY BODY_KEY
%token <sval> STRING

%type <string_list> string_list string_list_content
%type <plan> plan plan_list plan_definitions
%type <agent> agent agent_definitions

%%

program:
    LBRACE AGENTCODE_KEY COLON LBRACE agent_definitions RBRACE RBRACE
    { program_agents = $5; generate_maspy_code(program_agents); }
;

agent_definitions:
    agent
    { $$ = $1; }
    | agent_definitions COMMA agent
    { Agent* c = $1; while(c->next != NULL) c = c->next; c->next = $3; $$ = $1; }
;

agent:
    STRING COLON STRING COMMA BELIEFS_KEY COLON string_list COMMA GOAL_KEY COLON STRING COMMA PLANS_KEY COLON LBRACE plan_definitions RBRACE
    { $$ = create_agent_node($1, $3); $$->beliefs = $7; $$->goal = strdup($11); $$->plans = $16; free($1); free($3); }
;

string_list:
    LBRACK RBRACK { $$ = NULL; }
    | LBRACK string_list_content RBRACK { $$ = $2; }
;

string_list_content:
    STRING { $$ = create_string_node($1); free($1); }
    | string_list_content COMMA STRING { StringList* c = $1; while(c->next != NULL) c = c->next; c->next = create_string_node($3); $$ = $1; free($3); }
;

plan_definitions:
    /* empty */ { $$ = NULL; }
    | plan_list { $$ = $1; }
;

plan_list:
    plan { $$ = $1; }
    | plan_list COMMA plan { Plan* c = $1; while(c->next != NULL) c=c->next; c->next = $3; $$ = $1; }
;

plan:
    STRING COLON LBRACE TRIGGER_KEY COLON STRING COMMA CTX_KEY COLON STRING COMMA BODY_KEY COLON string_list RBRACE
    { $$ = create_plan_node($1); $$->trigger = strdup($6); $$->ctx = strdup($10); $$->body = $14; free($1); free($6); free($10); }
    | STRING COLON LBRACE TRIGGER_KEY COLON STRING COMMA BODY_KEY COLON string_list RBRACE
    { $$ = create_plan_node($1); $$->trigger = strdup($6); $$->ctx = NULL; $$->body = $10; free($1); free($6); }
;

%%

void yyerror(const char *s) { fprintf(stderr, "Erro de sintaxe: %s\n", s); }

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) { perror(argv[1]); return 1; }
        yyin = file;
    } else {
        yyin = stdin;
    }
    yydebug = 0; 
    yyparse();
    return 0;
}

void generate_maspy_code(Agent *agents) {
    printf("from maspy import *\n\n");
    Agent *current_agent = agents;
    while (current_agent != NULL) {
        char class_name[50];
        sprintf(class_name, "Agent_%s", current_agent->id);
        printf("class %s(Agent):\n", class_name);
        printf("    def __init__(self, agt_name):\n");
        printf("        super().__init__(agt_name)\n");
        StringList *belief = current_agent->beliefs;
        while (belief != NULL) {
            printf("        self.add(Belief(\"%s\"))\n", belief->str);
            belief = belief->next;
        }
        if (current_agent->goal && strlen(current_agent->goal) > 0) {
            printf("        self.add(Goal(\"%s\"))\n", current_agent->goal);
        }
        printf("\n");
        Plan *plan = current_agent->plans;
        while (plan != NULL) {
            char trigger_type[10] = "Belief";
            if (current_agent->goal && strcmp(plan->trigger, current_agent->goal) == 0) {
                 strcpy(trigger_type, "Goal");
            }
            printf("    @pl(gain, %s(\"%s\")", trigger_type, plan->trigger);
            if (plan->ctx) {
                printf(", Belief(\"%s\"))\n", plan->ctx);
            } else {
                printf(")\n");
            }
            char func_name[100];
            strcpy(func_name, plan->trigger);
            for (int i = 0; func_name[i]; i++) {
                if (func_name[i] == ' ') func_name[i] = '_';
            }
            printf("    def %s_(self, src):\n", func_name);
            StringList *body_item = plan->body;
            if (body_item == NULL) {
                printf("        pass\n");
            } else {
                while (body_item != NULL) { 
                    if (strncmp(body_item->str, "A_", 2) == 0) {
                        printf("        self.print(\"%s\")\n", body_item->str + 2);
                    } else if (strncmp(body_item->str, "G_", 2) == 0) {
                        printf("        self.add(Goal(\"%s\"))\n", body_item->str + 2);
                    } else if (strncmp(body_item->str, "B_", 2) == 0) {
                        printf("        self.add(Belief(\"%s\"))\n", body_item->str + 2);
                    }
                    body_item = body_item->next;
                }
            }
            printf("\n");
            plan = plan->next;
        }
        current_agent = current_agent->next;
    }
    printf("\n");
    current_agent = agents;
    while (current_agent != NULL) {
        printf("Agent_%s(\"%s\")\n", current_agent->id, current_agent->name);
        current_agent = current_agent->next;
    }
    printf("Admin().start_system()\n");
}