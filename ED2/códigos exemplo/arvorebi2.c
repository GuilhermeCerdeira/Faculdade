#include<stdio.h>
#include<stdlib.h>

typedef struct tno{
    int chave;
    struct tno *dir;
    struct tno *esq;
}tno;

tno *criarno(int chave);
tno *insereno(tno *raiz, int chave);
void buscarno(tno *raiz, int chave);
void emordem(tno *raiz);
void preordem(tno *raiz);
void posordem(tno *raiz);
tno *removeno(tno* raiz, int chave);
tno* sucessor(tno* raiz);

int main(){

    tno *raiz = NULL;
    int op = 1, chave = 0;
    while(op!=7){
        printf("\n=========== MENU LEGAL ===========\n");
        printf("n1-Inserir no\n");
        printf("n2-Em ordem\n");
        printf("n3-Pre-ordem\n");
        printf("n4-Pos-ordem\n");
        printf("n5-Buscar chave\n");
        printf("n6-remover no\n");
        printf("n7-Sair");
        printf("\n==================================\n");
        scanf("%d", &op);
            if(op == 1){
                printf("Digite a chave: ");
                scanf("%d", &chave);
                raiz = insereno(raiz, chave);
                if (raiz == NULL)
                    printf("Raiz NULLA!\n");
            }
            if(op == 2){
                printf("imprimindo em ordem: ");
                emordem(raiz);
            }
            if(op == 3){
                printf("imprimindo em pre-ordem: ");
                preordem(raiz);
            }
            if(op == 4){
                printf("imprimindo em pos-ordem: ");
                posordem(raiz);
            }
            if(op == 5){
                printf("insira a chave:");
                scanf("%d", &chave);
                buscarno(raiz, chave);
            }
            if(op == 6){
                printf("insira a chave a ser removida: ");
                scanf("%d", &chave);
                removeno(raiz, chave);
            }
    }
}

tno *criarno(int chave){
    tno *novo = malloc(sizeof(tno));
    if(novo){
        novo->chave = chave;
        novo->dir = NULL;
        novo->esq = NULL;
        return novo;
    }
    return NULL;
}

tno *insereno(tno *raiz, int chave){
    if(raiz == NULL){
        return criarno(chave);
    }
    else if(chave < raiz->chave){
        raiz->esq = insereno(raiz->esq, chave);
    } else{
        raiz->dir = insereno(raiz->dir, chave);
    }
    return raiz;
}

void emordem(tno *raiz){
    if(raiz!=NULL){
        emordem(raiz->esq);
        printf("%d ", raiz->chave);
        emordem(raiz->dir);
    }
}

void preordem(tno *raiz){
    if(raiz!=NULL){
        printf("%d ", raiz->chave);
        preordem(raiz->esq);
        preordem(raiz->dir);
    }
}

void posordem(tno *raiz){
    if(raiz!=NULL){
        preordem(raiz->esq);
        preordem(raiz->dir);
        printf("%d ", raiz->chave);
    }
}

void buscarno(tno *raiz, int chave){
    if(raiz == NULL){
        printf("chave nao encontrada");
        return;
    }
    if(raiz->chave == chave){
        printf("chave encontrada!");
        return;
    }
    if(chave < raiz->chave){
        buscarno(raiz->esq, chave);
    }
    else{
        buscarno(raiz->dir, chave);
    }
}

tno *sucessor(tno* raiz){
    raiz = raiz->dir;
    while(raiz->esq != NULL){
        raiz = raiz->esq;
    }
    return raiz;
}

tno *removeno(tno* raiz, int chave){
    if(raiz == NULL){
        printf("no nao encontrado\n");
        return NULL;
    }
    if(chave < raiz->chave){
        raiz->esq = removeno(raiz->esq, chave);
    }
    else if(chave > raiz->chave){
        raiz->dir = removeno(raiz->dir, chave);
    }
    else{
        if(raiz->esq == NULL && raiz->dir == NULL){
            free(raiz);
            return NULL;
        }
        else if(raiz->dir == NULL && raiz->esq != NULL){
            tno* temp = raiz->esq;
            free(raiz);
            return temp;
        }
        else if(raiz->esq == NULL && raiz->dir != NULL){
            tno* temp = raiz->dir;
            free(raiz);
            return temp;
        }
    tno* temp = sucessor(raiz);
    raiz->chave = temp->chave;
    raiz->dir = removeno(raiz->dir, temp->chave);
    }
    return raiz;
}