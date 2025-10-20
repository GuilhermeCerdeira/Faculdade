/*
 * Funções Auxiliares para uma calculadora avançada
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdarg.h>
 #include <string.h>
 #include <math.h>
 #include "bison-calc.h"
 
 /* Funções em C para a Tabela de Símbolos */
 
 /* Função hashing */
 static unsigned symhash(char *sym) {
     unsigned int hash = 0;
     unsigned c;
 
     while ((c = *sym++))
         hash = hash * 9 ^ c;
 
     return hash;
 }
 
 struct symbol *lookup(char *sym) {
     struct symbol *sp = &symtab[symhash(sym) % NHASH];
     int scount = NHASH;
 
     while (--scount >= 0) {
         if (sp->name && !strcasecmp(sp->name, sym))
             return sp;
 
         if (!sp->name) { /* nova entrada na TS */
             sp->name = strdup(sym);
             sp->value = 0;
             sp->func = NULL;
             sp->syms = NULL;
             return sp;
         }
 
         if (++sp >= symtab + NHASH)
             sp = symtab; /* encadeamento circular */
     }

    yyerror("Overflow na tabela de símbolos\n");
    abort(); /* tabela está cheia */
 }
 
 /* Cria um novo nó AST binário */
 struct ast *newast(int nodetype, struct ast *l, struct ast *r) {
     struct ast *a = malloc(sizeof(struct ast));
 
     if (!a) {
         yyerror("Sem espaço");
         exit(0);
     }
 
     a->nodetype = nodetype;
     a->l = l;
     a->r = r;
     return a;
 }
 
 /* Cria um nó AST para números */
 struct ast *newnum(double d) {
     struct numval *a = malloc(sizeof(struct numval));
 
     if (!a) {
         yyerror("Sem espaço");
         exit(0);
     }
 
     a->nodetype = 'K';
     a->number = d;
     return (struct ast *)a;
 }
 
 /* Cria um nó AST para comparações */
 struct ast *newcmp(int cmptype, struct ast *l, struct ast *r) {
     struct ast *a = malloc(sizeof(struct ast));
 
     if (!a) {
         yyerror("Sem espaço");
         exit(0);
     }
 
     a->nodetype = '0' + cmptype;
     a->l = l;
     a->r = r;
     return a;
 }
 
 /* Cria um nó AST para funções internas (como sqrt, log, etc.) */
 struct ast *newfunc(int functype, struct ast *l) {
     struct fncall *a = malloc(sizeof(struct fncall));
 
     if (!a) {
         yyerror("Sem espaço");
         exit(0);
     }
 
     a->nodetype = 'F';
     a->l = l;
     a->functype = functype;
     return (struct ast *)a;
 }
 
 /* Cria um nó AST para chamadas de funções do usuário */
 struct ast *newcall(struct symbol *s, struct ast *l) {
     struct ufncall *a = malloc(sizeof(struct ufncall));
 
     if (!a) {
         yyerror("Sem espaço");
         exit(0);
     }
 
     a->nodetype = 'C';
     a->l = l;
     a->s = s;
     return (struct ast *)a;
 }
 
 /* Cria um nó AST para referência a variáveis */
 struct ast *newref(struct symbol *s) {
     struct symref *a = malloc(sizeof(struct symref));
 
     if (!a) {
         yyerror("Sem espaço");
         exit(0);
     }
 
     a->nodetype = 'N';
     a->s = s;
     return (struct ast *)a;
 }
 
 /* Cria um nó AST para atribuições */
 struct ast *newasgn(struct symbol *s, struct ast *v) {
     struct symasgn *a = malloc(sizeof(struct symasgn));
 
     if (!a) {
         yyerror("Sem espaço");
         exit(0);
     }
 
     a->nodetype = '=';
     a->s = s;
     a->v = v;
     return (struct ast *)a;
 }
 
/* Cria um novo nó AST para estruturas de controle de fluxo (if, while) */
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *el) {
    struct flow *a = malloc(sizeof(struct flow));

    if (!a) {
        yyerror("Sem espaço");
        exit(0);
    }

    a->nodetype = nodetype;
    a->cond = cond;
    a->tl = tl;
    a->el = el;
    return (struct ast *)a;
}

/* Libera uma árvore de AST */
void treefree(struct ast *a) {
    switch (a->nodetype) {
        /* Nós com duas subárvores */
        case '+':
        case '-':
        case '*':
        case '/':
        case '1': case '2': case '3': case '4': case '5': case '6':
        case 'L':
            treefree(a->r);
            // intencionalmente continua para liberar a->l

        /* Nós com uma subárvore */
        case 'C':
        case 'F':
            treefree(a->l);
            break;

        /* Nós sem subárvore */
        case 'K':
        case 'N':
            break;

        /* Atribuição */
        case '=':
            free(((struct symasgn *)a)->v);
            break;

        /* Controle de fluxo (if, while) com até 3 subárvores */
        case 'I':
        case 'W':
            treefree(((struct flow *)a)->cond);
            if (((struct flow *)a)->tl)
                treefree(((struct flow *)a)->tl);
            if (((struct flow *)a)->el)
                treefree(((struct flow *)a)->el);
            break;

        default:
            printf("Erro interno: free bad node %c\n", a->nodetype);
            break;
    }

    /* Sempre libera o próprio nó */
    free(a);
}

/* Cria uma nova lista de símbolos */
struct symlist *newsymlist(struct symbol *sym, struct symlist *next) {
    struct symlist *sl = malloc(sizeof(struct symlist));

    if (!sl) {
        yyerror("Sem espaço");
        exit(0);
    }

    sl->sym = sym;
    sl->next = next;
    return sl;
}

/* Libera uma lista de símbolos */
void symlistfree(struct symlist *sl) {
    struct symlist *nsl;

    while (sl) {
        nsl = sl->next;
        free(sl);
        sl = nsl;
    }
}

/* Etapa principal: avaliação de expressões, comandos, funções, etc. */

static double callbuiltin(struct fncall *);   // chamada de função embutida
static double calluser(struct ufncall *);     // chamada de função definida pelo usuário

double eval(struct ast *a) {
    double v;

    if (!a) {
        yyerror("Erro interno: null eval");
        return 0.0;
    }

    switch (a->nodetype) {
        /* Constante */
        case 'K':
            v = ((struct numval *)a)->number;
            break;

        /* Referência de variável */
        case 'N':
            v = ((struct symref *)a)->s->value;
            break;

        /* Atribuição */
        case '=':
            v = ((struct symasgn *)a)->s->value = eval(((struct symasgn *)a)->v);
            break;

        /* Expressões matemáticas */
        case '+': v = eval(a->l) + eval(a->r); break;
        case '-': v = eval(a->l) - eval(a->r); break;
        case '*': v = eval(a->l) * eval(a->r); break;
        case '/': v = eval(a->l) / eval(a->r); break;

        /* Operadores de comparação */
        case '1': v = (eval(a->l) >  eval(a->r)) ? 1 : 0; break;
        case '2': v = (eval(a->l) <  eval(a->r)) ? 1 : 0; break;
        case '3': v = (eval(a->l) != eval(a->r)) ? 1 : 0; break;
        case '4': v = (eval(a->l) == eval(a->r)) ? 1 : 0; break;
        case '5': v = (eval(a->l) >= eval(a->r)) ? 1 : 0; break;
        case '6': v = (eval(a->l) <= eval(a->r)) ? 1 : 0; break;

        /* Controle de fluxo: if-then-else */
        case 'I':
            if (eval(((struct flow *)a)->cond) != 0) {
                if (((struct flow *)a)->tl)
                    v = eval(((struct flow *)a)->tl);
                else
                    v = 0.0;
            } else {
                if (((struct flow *)a)->el)
                    v = eval(((struct flow *)a)->el);
                else
                    v = 0.0;
            }
            break;

        /* Estrutura de repetição: while */
        case 'W':
            v = 0.0;
            if (((struct flow *)a)->tl) {
                while (eval(((struct flow *)a)->cond) != 0)
                    v = eval(((struct flow *)a)->tl);
            }
            break;

        /* Lista de comandos: avalia os dois lados, mas retorna o da direita */
        case 'L':
            eval(a->l);
            v = eval(a->r);
            break;

        /* Funções embutidas (como sqrt, sin etc) */
        case 'F':
            v = callbuiltin((struct fncall *)a);
            break;

        /* Funções definidas pelo usuário */
        case 'C':
            v = calluser((struct ufncall *)a);
            break;

        default:
            printf("Erro interno: bad node %c\n", a->nodetype);
            break;
    }

    return v;
}

static double callbuiltin(struct fncall *f) {
    enum bifs functype = f->functype;
    double v = eval(f->l);

    switch (functype) {
        case Bsqrt:
            return sqrt(v);
        case Bexp:
            return exp(v);
        case Blog:
            return log(v);
        case Bprint:
            printf("=%4.4g\n", v);
            return v;
        default:
            yyerror("Funcao pre-definida %d desconhecida\n", functype);
            return 0.0;
    }
}

/* Função definida por usuário */
void dodef(struct symbol *name, struct symlist *syms, struct ast *func) {
    if (name->syms)
        symlistfree(name->syms);
    if (name->func)
        treefree(name->func);

    name->syms = syms;
    name->func = func;
}

static double calluser(struct ufncall *f) {
    struct symbol *fn = f->s;         /* Nome da função */
    struct symlist *sl;               /* Argumentos (originais) da função */
    struct ast *args = f->l;          /* Argumentos (usados) na função */
    double *oldval, *newval;          /* Salvar valores de argumentos */
    double v;
    int nargs, i;

    if (!fn->func) {
        yyerror("chamada para função %s indefinida", fn->name);
        return 0.0;
    }

    /* Contar argumentos */
    sl = fn->syms;
    for (nargs = 0; sl; sl = sl->next)
        nargs++;

    /* Preparar para salvar argumentos */
    oldval = (double *)malloc(nargs * sizeof(double));
    newval = (double *)malloc(nargs * sizeof(double));
    if (!oldval || !newval) {
        yyerror("Sem espaco em %s", fn->name);
        return 0.0;
    }

    /* Avaliação dos argumentos */
    for (i = 0; i < nargs; i++) {
        if (!args) {
            yyerror("Poucos argumentos na chamada da função %s", fn->name);
            free(oldval);
            free(newval);
            return 0.0;
        }

        if (args->nodetype == 'L') { /* Se for uma lista de nós */
            newval[i] = eval(args->l);
            args = args->r;
        } else {                     /* Se for o final da lista */
            newval[i] = eval(args);
            args = NULL;
        }
    }

    /* Salvar valores antigos e atribuir novos */
    sl = fn->syms;
    for (i = 0; i < nargs; i++) {
        struct symbol *s = sl->sym;
        oldval[i] = s->value;
        s->value = newval[i];
        sl = sl->next;
    }

    free(newval);

    /* Avaliação da função */
    v = eval(fn->func);

    /* Restaurar valores antigos */
    sl = fn->syms;
    for (i = 0; i < nargs; i++) {
        struct symbol *s = sl->sym;
        s->value = oldval[i];
        sl = sl->next;
    }

    free(oldval);
    return v;
}

void yyerror(char *s, ...) {
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "%d : error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

int main() {
    printf("> ");
    return yyparse();
}
