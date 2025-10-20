#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//definição da struct
typedef struct contato{
    char nome[20];
    char numero[12];
    char email[20];
    struct contato *direita;
    struct contato *esquerda;
}contato;


//protótipo das funções
contato *criarno(char nome[], char numero[], char email[]);
contato *insereno(contato *raiz, char nome[], char numero[], char email[]);
contato *buscarno(contato *raiz, char nome[]);
void emordem(contato *raiz);
contato *trocar(contato *raiz, char nome[], char numero[], char email[]);
contato *remover(contato* raiz, char nome[]);
contato *sucessor(contato* raiz);


//main
int main(){

    //variáveis da main
    contato *raiz = NULL;
    int op = 1;
    char nome[20];
    char numero[12]; 
    char email[20];

    //loop do menu
    while(op!=6){
        printf("\n=========== MENU LEGAL ===========\n");
        printf("1-Inserir contato\n");
        printf("2-Imprimir os contatos em ordem alfabetica\n");
        printf("3-Buscar contato pelo nome\n");
        printf("4-Remover contato\n");
        printf("5-Alterar contato\n");
        printf("6-Sair");
        printf("\n==================================\n");
        scanf("%d", &op);
        if(op == 1){
            printf("\nInsira o nome do contato: ");
            scanf("%s", nome); 
            printf("\nInsira o numero do contato: ");
            scanf("%s", numero);
            printf("\nInsira o email do contato: ");
            scanf("%s", email);
            raiz = insereno(raiz, nome, numero, email);
            if (raiz == NULL) //caso tenha dado errado a inseção 
                printf("Raiz NULLA!\n");
            else {
                printf("Contato inserido com sucesso!\n");
            }
        }
        if(op == 2){
            printf("Imprimindo em ordem: ");
            emordem(raiz); //a impressão em ordem gera ordem alfabética
        }
        if(op == 3){ 
            printf("Insira o nome: ");
            scanf("%s", nome);
            buscarno(raiz, nome); 
        }
        if(op == 4){ 
            printf("Insira o nome a ser removido: ");
            scanf("%s", nome);
            raiz = remover(raiz, nome); 
        }
        if(op == 5){
            printf("Insira o nome do contato a ser trocado: ");
            scanf("%s", nome);
            printf("Insira o novo numero do contato a ser trocado: ");
            scanf("%s", numero);
            printf("Insira o novo email do contato a ser trocado: ");
            scanf("%s", email);
            raiz = trocar(raiz, nome, numero, email);
        }
    }
}

//cria o nó atribuindo o nome o numer e o email usando strcpy e retorna nó
contato *criarno(char nome[], char numero[], char email[]){
    contato *novo = (contato *)malloc(sizeof(contato));
    strcpy(novo->nome, nome);
    strcpy(novo->numero, numero);
    strcpy(novo->email, email);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

//insere o nó na arvore chamando a função de criar nó e strcmp para inserir bonitinho na arvore
contato *insereno(contato *raiz, char nome[], char numero[], char email[]){
   if(raiz == NULL){
    return criarno(nome, numero, email);
   }
   if (strcmp(nome, raiz->nome) < 0) {
        raiz->esquerda = insereno(raiz->esquerda, nome, numero, email);
    } else if (strcmp(nome, raiz->nome) > 0) {
        raiz->direita = insereno(raiz->direita, nome, numero, email);
    } else {
        printf("\nContato duplicado e nao sera adicionado!\n");
    }
    return raiz;
}


//buscar o no pelo nome usando strcmp e retorna o nó ou null
contato *buscarno(contato *raiz, char nome[]){
    if (raiz == NULL) {
        printf("Contato nao encontrado\n"); 
        return NULL;
    }
    if(strcmp(raiz->nome, nome) == 0){
        printf("Contato encontrado: %s\nNumero: %s\nEmail: %s\n", raiz->nome, raiz->numero, raiz->email);
        return raiz;
    }
    if (strcmp(nome, raiz->nome) < 0) {
        return buscarno(raiz->esquerda, nome); 
    }
    return buscarno(raiz->direita, nome); 
}

//imprime em ordem que gera ordem alfabética
void emordem(contato *raiz){
    if(raiz!=NULL){
        emordem(raiz->esquerda);
        printf("%s ", raiz->nome);
        emordem(raiz->direita);
    }
}

//troca os contatos usando a funcao buscar
contato *trocar(contato *raiz, char nome[], char numero[], char email[]){  
    raiz = buscarno(raiz, nome);
    if(raiz == NULL){
        return NULL;
    }else{
        strcpy(raiz->nome, nome);
        strcpy(raiz->numero, numero);
        strcpy(raiz->email, email);
        return raiz;   
    }    
}

//optei por trocar pelo sucessor (menor da direita) 
contato *sucessor(contato* raiz){
    raiz = raiz->direita;
    while(raiz->esquerda != NULL){
        raiz = raiz->esquerda;
    }
    return raiz;
}

//remove o nó da arvore e retorna a raiz atualizada
contato *remover(contato *raiz, char nome[]) {
    if (raiz == NULL) {
        return raiz;
    }
    if (strcmp(nome, raiz->nome) < 0) {
        raiz->esquerda = remover(raiz->esquerda, nome);
    } else if (strcmp(nome, raiz->nome) > 0) {
        raiz->direita = remover(raiz->direita, nome);
    } else {
        //caso o nó tenha apenas um filho ou nenhum filho
        if (raiz->esquerda == NULL) {
            contato *temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            contato *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        //caso o nó tenha dois filhos, encontrar o sucessor.
        contato *temp = sucessor(raiz->direita);
        strcpy(raiz->nome, temp->nome);
        strcpy(raiz->numero, temp->numero);
        strcpy(raiz->email, temp->email);
        raiz->direita = remover(raiz->direita, temp->nome);
    }
    return raiz;
}
