/*
 * Declarações para uma calculadora avançada
 */

/* Interface com o lexer */
extern int yylineno;

void yyrestart();
void yyerror(char *s, ...);
int yyparse();
int yylex();

/* Tabela de símbolos */
struct symbol {
    /* Um nome de variável */
    char *name;
    double value;
    struct ast *func;  /* Stmt para função */
    struct symlist *syms;  /* Lista de argumentos */
};

/* Tabela de símbolos de tamanho fixo */

struct symbol *lookup(char *);

/* Lista de símbolos, para uma lista de argumentos */
struct symlist {
    struct symbol *sym;
    struct symlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

/* Tipos de nós */
/*
 *  + - * /
 *  0-7 operadores de comparação, 04 igual, 02 menor que, 01 maior que
 *  L expressão ou lista de comandos
 *  I comando IF
 *  W comando WHILE
 *  N symbol de referência
 *  = atribuição
 *  S lista de símbolos
 *  F chamada de função predefinida
 *  C chamada de função definida pelo usuário
 */

enum bifs { /* Funções predefinidas */
    B_sqrt = 1,
    B_exp,
    B_log,
    B_print
};

/* Nós na AST */
/* Todos têm o "nodetype" inicial em comum */
struct ast {
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct fncall { /* Funções predefinidas */
    int nodetype;  /* Tipo F */
    struct ast *l;
    enum bifs functype;
};

struct ufncall { /* Funções do usuário */
    int nodetype;  /* Tipo C */
    struct ast *l;  /* Lista de argumentos */
    struct symbol *s;
};

struct flow {
    int nodetype;  /* Tipo I ou W */
    struct ast *cond;  /* Condição */
    struct ast *tl;  /* Ramo "then" ou lista "do" */
    struct ast *el;  /* Ramo opcional "else" */
};

struct stop {
    int nodetype;  /* Tipo I ou W */
    struct ast *init;  /* Condição */
    struct ast *cond;  /* Ramo "then" ou lista "do" */
    struct ast *inc;  /* Ramo opcional "else" */
    struct ast *list;  /* Ramo opcional "else" */
};

struct numval {
    int nodetype;  /* Tipo K */
    double number;
};

struct symref {
    int nodetype;  /* Tipo N */
    struct symbol *s;
};

struct symasgn {
    int nodetype;  /* Tipo = */
    struct symbol *s;
    struct ast *v;  /* Valor a ser atribuído */
};

/* Construção de uma AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);
struct ast *newstop(struct ast *init, struct ast *cond, struct ast *inc, struct ast *list);

/* Definição de uma função */
void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);

/* Avaliação de uma AST */
double eval(struct ast *);

/* Deletar e liberar uma AST */
void treefree(struct ast *);

/* Printar a saida */
void printOutput(double output);