///////////////////////////////////////////////////////////////////////////
///                                                                     ///
///         Sistema de Gerenciamento de Processos Computacionais        ///
///                                                                     ///
///             Arthur Romão Barreto | RA: 2474050                      ///
///          Guilherme de Souza Cerdeira | RA: 2474085                  ///
///                                                                     ///
///////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct Processo{//Struct dos processos
    int codigo;
    char nome[20];
    char estado[20];
    int prioridade;
}Processo;

typedef struct MaxHeap{//Struct da tabela heap
    Processo **vetor;
    int capacidade;
}MaxHeap;

typedef struct no{//Struct da árvore AVL
    Processo *processo;
    struct no *esquerda;
    struct no *direita;
    struct no *pai; 
}No;

typedef struct HashEa{//Struct do hash de endereçamento aberto
    Processo *processo;
    char status;
    int m;
}HashEa;

typedef struct HashEs{//Struct do hash de encadeamento separado
    Processo **ponteiro;
    int m;
    int capacidade;
}HashEs;

//funções gerais
Processo *criarProcesso(int codigo, char nome[], char estado[], int prioridade);
No *terminar(No *raiz, MaxHeap *heap, HashEa *hashEaSl, HashEa *hashEaSq, HashEs *hashEsSl, int chave, char nome[]);
void destruir(No *raiz, MaxHeap* heap, HashEs *hashEs);

//funções AVL
No *inserirAVL(No *no, Processo *processo);  
No *buscarAVL(No *no, int codigo);
void imprimirAVL(FILE *outfile, FILE *outAVL, No *no);
No *terminarAVL(No *raiz, MaxHeap *heap, HashEa *hashEaSl, HashEa *hashEaSq, HashEs *hashEsSl, int chave);
No *removerAVL(No *no, int chave);

//funções heap
MaxHeap *criarHeap(int capacidade);
void inserirHeap(MaxHeap *heap, Processo *processo);
void alterarHeap(MaxHeap *heap, No *raiz, int codigo, int prioridade);
void imprimirHeap(FILE* outfile, FILE* outheap, MaxHeap *heap);
Processo *removerHeap(MaxHeap *heap, No *raiz);
void destruirHeap(MaxHeap *heap);

//funções hash (endereçamento aberto)
HashEa *criarHashEa(HashEa *hash, int m);
void inserirHashEaSq(HashEa *hash, Processo *processo);
void inserirHashEaSl(HashEa *hash, Processo *processo);
void imprimirHashEaSl(FILE* outfile, FILE* outhash1, HashEa *hash, char estado[]);
void imprimirHashEaSq(FILE* outfile, FILE* outhash2, HashEa *hash, char estado[]);
void alterarHashEaSl(HashEa *hash, No *raiz, int codigo, char estado[]);
void alterarHashEaSq(HashEa *hash, No *raiz, int codigo, char estado[]);
void removerHashEaSl(HashEa *hash, No *raiz, int codigo);
void removerHashEaSq(HashEa *hash, No *raiz, int codigo);

//funções hash (encadeamento separado)
HashEs *criarHashEs(HashEs *hash);
void inserirHashEsSl(HashEs *hash, Processo *processo);
void imprimirHashEsSl(FILE* outfile, FILE* outhash3, HashEs *hash, char estado[]);
void alterarHashEsSl(HashEs *hash, No *raiz, int codigo, char estado[]);
void removerHashEsSl(HashEs *hash, No *raiz, int codigo);

int main(){
    //ponteiros para as estruturas de dados
    No *raiz = NULL;
    Processo *processo = NULL;
    MaxHeap *heap = criarHeap(10);
    HashEa *hashEaSl = criarHashEa(NULL, 50);
    HashEa *hashEaSq = criarHashEa(NULL, 50);
    HashEs *hashEs = criarHashEs(NULL);

    //variaveis para medir o desempenho
    clock_t inserirHashEaSlInicio, inserirHashEaSlFim, inserirHashEaSqInicio, inserirHashEaSqFim, inserirHashEsSlInicio, inserirHashEsSlFim;
    double tempoHashEasl, tempoHashEaSq, tempoHashEsSl;
    clock_t inserirHashEaSltotal = 0, inserirHashEaSqtotal = 0, inserirHashEsSltotal = 0;

    //abrindo o arquivo de entrada e os de saida
    FILE *infile = fopen("entrada.txt", "r");
    FILE *outfile = fopen("saida.txt", "w");
    FILE *outAVL = fopen("AVL.txt", "w");
    FILE *outheap = fopen("heap.txt", "w");
    FILE *outhash1 = fopen("hash_easl.txt", "w");
    FILE *outhash2 = fopen("hash_easq.txt", "w");
    FILE *outhash3 = fopen("hash_essl.txt", "w");

    //caso de erro na abertura dos arquivos
    if(infile == NULL || outfile == NULL){
        perror("Erro ao abrir arquivos.");
        return 1;
    }

    //variaveis para a chamada das funções
    char operacao[20];
    int codigo;
    char nome[20];
    char estado[20];
    int prioridade;
    int opcao = 0;
    int opcao2 = 0;
    int opcao3;

    //loop principal para a interação com as funções
    while(opcao != 3){
        printf("Insira uma opcao: \n1 - Inserir funcoes manualmente\n2 - Ler arquivo\n3 - Sair do programa\n");
        scanf("%d", &opcao);
        if(opcao == 1){
            while(opcao2 != 13){
                printf("Escolha uma operacao: \n1 - InserirAVL\n2 - ListarAVL\n3 - TerminarAVL\n4 - AlterarHeap\n5 - ListarHeap\n6 - RemoverHeap\n7 - BloquearHash\n8 - DesbloquearHash\n9 - ListarHash\n10 - RemoverHash\n11 - Executar\n12 - Terminar\n13 - Encerrar\n");
                scanf("%d", &opcao2);
                if(opcao2 == 1){//inserir avl
                    printf("Insira o codigo, nome, prioridade e o estado do programa que deseja inserir.\n");
                    printf("Codigo: ");
                    scanf("%d", &codigo);
                    printf("Nome: ");
                    scanf("%s", nome);
                    printf("Prioridade: ");
                    scanf("%d", &prioridade);
                    printf("Estado:\n");
                    opcao3 = 0;
                    while(opcao3 != 1 && opcao3 != 2 && opcao3 != 3){//o usuario pode escolher qual estado do processo
                        printf("1 - PRONTO\n2 - BLOQUEADO\n3 - EXECUTANDO\n");
                        scanf("%d", &opcao3);
                    }
                    if(opcao3 == 1)
                        strcpy(estado,"PRONTO");
                    if(opcao3 == 2)
                        strcpy(estado,"BLOQUEADO");
                    if(opcao3 == 3)
                        strcpy(estado,"EXECUTANDO");
                    if(codigo > 0 && prioridade > 0 && ((strcmp(estado, "PRONTO") == 0) || (strcmp(estado, "BLOQUEADO") == 0) || (strcmp(estado, "EXECUTANDO") == 0))){
                        if(buscarAVL(raiz, codigo) == NULL){
                            processo = criarProcesso(codigo, nome, estado, prioridade);
                            fprintf(outfile,"Processo criado com sucesso\n");
                            raiz = inserirAVL(raiz, processo);
                            fprintf(outfile,"Processo inserido na AVL com sucesso\n");
                            inserirHeap(heap, processo);
                            fprintf(outfile,"Processo inserido no heap com sucesso\n");
                            inserirHashEaSlInicio = clock();
                            inserirHashEaSl(hashEaSl, processo);
                            inserirHashEaSlFim = clock();
                            inserirHashEaSltotal += (inserirHashEaSlFim - inserirHashEaSlInicio);
                            inserirHashEaSqInicio = clock();
                            inserirHashEaSq(hashEaSq, processo);
                            inserirHashEaSqFim = clock();
                            inserirHashEaSqtotal += (inserirHashEaSqFim - inserirHashEaSqInicio);
                            inserirHashEsSlInicio = clock();
                            inserirHashEsSl(hashEs, processo);
                            inserirHashEsSlFim = clock();
                            inserirHashEsSltotal += (inserirHashEsSlFim - inserirHashEsSlInicio);
                            fprintf(outfile,"Processo %d inserido no hash com sucesso\n",codigo);
                        }
                    }else{
                        printf("Informacoes inseridas incorretamente.");
                        break;
                    }
                }
                if(opcao2 == 2){//listar avl
                    fprintf(outfile, "Arvore AVL:\n");
                    fprintf(outAVL, "Arvore AVL:\n");
                    imprimirAVL(outfile, outAVL, raiz);
                    fprintf(outfile, "\n");
                    fprintf(outAVL, "\n");
                }
                if(opcao2 == 3){//terminar avl
                    printf("Insira o codigo do nodo que deseja terminar: ");
                    scanf("%d", &codigo);
                    raiz = terminarAVL(raiz, heap, hashEaSl, hashEaSq, hashEs, codigo);
                    fprintf(outfile,"Processo %d terminado.\n",codigo);
                }
                if(opcao2 == 4){//alterar heap
                    printf("Insira o codigo e a prioridade que deseja alterar.\n");
                    printf("Codigo: ");
                    scanf("%d", &codigo);
                    printf("Prioridade: ");
                    scanf("%d", &prioridade);
                    alterarHeap(heap, raiz, codigo, prioridade);
                    fprintf(outfile,"Prioridade do processo %d foi alterada\n",codigo);
                }
                if(opcao2 == 5){//listar heap
                    imprimirHeap(outfile, outheap, heap);
                }
                if(opcao2 == 6){//remover heap
                    removerHeap(heap, raiz);
                    fprintf(outfile,"Processo %d removido da heap\n",codigo);
                }
                if(opcao2 == 7){//bloquear hash
                    printf("Insira o codigo do programa que deseja bloquear: ");
                    scanf("%d", &codigo);
                    alterarHashEaSl(hashEaSl, raiz, codigo, "BLOQUEADO");
                    alterarHashEaSq(hashEaSq, raiz, codigo, "BLOQUEADO");
                    alterarHashEsSl(hashEs, raiz, codigo, "BLOQUEADO");
                    fprintf(outfile,"Processo %d bloqueado\n",codigo);
                }
                if(opcao2 == 8){//desbloquear hash
                    printf("Insira o codigo do programa que deseja desbloquear: ");
                    scanf("%d", &codigo);
                    alterarHashEaSl(hashEaSl, raiz, codigo, "PRONTO");
                    alterarHashEaSq(hashEaSq, raiz, codigo, "PRONTO");
                    alterarHashEsSl(hashEs, raiz, codigo, "PRONTO");
                    fprintf(outfile,"Processo %d desbloqueado\n",codigo);
                }
                if(opcao2 == 9){//listar hash
                    printf("Escolha qual estado deseja listar:\n1 - PRONTO\n2 - BLOQUEADO\n3 - EXECUTANDO\n");
                    scanf("%d", &opcao3);
                    if(opcao3 == 1){//listar hashes prontos
                        imprimirHashEaSl(outfile, outhash1, hashEaSl, "PRONTO");
                        imprimirHashEaSq(outfile, outhash2, hashEaSq, "PRONTO");
                        imprimirHashEsSl(outfile, outhash3, hashEs, "PRONTO");
                    }
                    if(opcao3 == 2){//listar hashes bloqueados
                        imprimirHashEaSl(outfile, outhash1, hashEaSl, "BLOQUEADO");
                        imprimirHashEaSq(outfile, outhash2, hashEaSq, "BLOQUEADO");
                        imprimirHashEsSl(outfile, outhash3, hashEs, "BLOQUEADO");
                    }
                    if(opcao3 == 3){//listar hashes executando
                        imprimirHashEaSl(outfile, outhash1, hashEaSl, "EXECUTANDO");
                        imprimirHashEaSq(outfile, outhash2, hashEaSq, "EXECUTANDO");
                        imprimirHashEsSl(outfile, outhash3, hashEs, "EXECUTANDO");
                    }
                }
                if(opcao2 == 10){//remover hash
                    printf("Insira o codigo do programa que deseja remover: ");
                    scanf("%d", &codigo);
                    removerHashEaSl(hashEaSl, raiz, codigo);
                    removerHashEaSq(hashEaSq, raiz, codigo);
                    removerHashEsSl(hashEs, raiz, codigo);
                    fprintf(outfile,"Processo %d removido da hash\n",codigo);
                }
                if(opcao2 == 11){//executar
                    printf("Insira o codigo do programa que deseja executar: ");
                    scanf("%d", &codigo);
                    alterarHashEaSl(hashEaSl, raiz, codigo, "EXECUTANDO");
                    alterarHashEaSq(hashEaSq, raiz, codigo, "EXECUTANDO");
                    alterarHashEsSl(hashEs, raiz, codigo, "EXECUTANDO");
                    fprintf(outfile,"Processo %d executando\n",codigo);
                }
                if(opcao2 == 12){//terminar
                    printf("Insira o codigo e a nome do processo que deseja terminar.\n");
                    printf("Codigo: ");
                    scanf("%d", &codigo);
                    printf("Nome: ");
                    scanf("%s", nome);
                    raiz = terminar(raiz, heap, hashEaSl, hashEaSq, hashEs, codigo, nome);
                    fprintf(outfile,"Processo %d terminado\n",codigo);
                }
                if(opcao2 == 13){//encerrar
                fprintf(outfile,"Encerrando o sistema.\n");
                    break;
                }
            }
        }
        if(opcao == 2){//chamada das funções apartir de um arquivo
            while(fscanf(infile, "%s", operacao) != EOF){
                if(strcmp(operacao, "InserirAVL") == 0){
                    fscanf(infile, "%d %s %d %s", &codigo, nome, &prioridade, estado);
                    if((strcmp(estado, "PRONTO") == 0) || (strcmp(estado, "BLOQUEADO") == 0) || (strcmp(estado, "EXECUTANDO") == 0)){
                        if(buscarAVL(raiz, codigo) == NULL){
                            processo = criarProcesso(codigo, nome, estado, prioridade);
                            fprintf(outfile,"Processo criado com sucesso\n");
                            raiz = inserirAVL(raiz, processo);
                            fprintf(outfile,"Processo inserido na AVL com sucesso\n");
                            inserirHeap(heap, processo);
                            fprintf(outfile,"Processo inserido no heap com sucesso\n");
                            inserirHashEaSlInicio = clock();
                            inserirHashEaSl(hashEaSl, processo);
                            inserirHashEaSlFim = clock();
                            inserirHashEaSltotal += (inserirHashEaSlFim - inserirHashEaSlInicio);
                            inserirHashEaSqInicio = clock();
                            inserirHashEaSq(hashEaSq, processo);
                            inserirHashEaSqFim = clock();
                            inserirHashEaSqtotal += (inserirHashEaSqFim - inserirHashEaSqInicio);
                            inserirHashEsSlInicio = clock();
                            inserirHashEsSl(hashEs, processo);
                            inserirHashEsSlFim = clock();
                            inserirHashEsSltotal += (inserirHashEsSlFim - inserirHashEsSlInicio);
                            fprintf(outfile,"Processo inserido no hash com sucesso\n");
                        }
                    }else{
                        fprintf(outfile,"Estado invalido.\n");
                    }
                }else if(strcmp(operacao, "ListarAVL") == 0){
                    fprintf(outfile, "Arvore AVL:\n");
                    fprintf(outAVL, "Arvore AVL:\n");
                    imprimirAVL(outfile, outAVL, raiz);
                    fprintf(outfile, "\n");
                    fprintf(outAVL, "\n");
                }else if(strcmp(operacao, "TerminarAVL") == 0){
                    fscanf(infile, "%d", &codigo);
                    raiz = terminarAVL(raiz, heap, hashEaSl, hashEaSq, hashEs, codigo);
                    fprintf(outfile,"Processo %d terminado\n",codigo);
                }else if(strcmp(operacao, "AlterarHeap") == 0){
                    fscanf(infile, "%d %d", &codigo, &prioridade);
                    alterarHeap(heap, raiz, codigo, prioridade);
                    fprintf(outfile,"Prioridade do processo %d foi alterada\n",codigo);
                }else if(strcmp(operacao, "ListarHeap") == 0){
                    imprimirHeap(outfile, outheap, heap);
                }else if(strcmp(operacao, "RemoverHeap") == 0){
                    removerHeap(heap, raiz);
                    fprintf(outfile,"Processo %d removido da heap\n",codigo);
                }else if(strcmp(operacao, "BloquearHash") == 0){
                    fscanf(infile, "%d", &codigo);
                    alterarHashEaSl(hashEaSl, raiz, codigo, "BLOQUEADO");
                    alterarHashEaSq(hashEaSq, raiz, codigo, "BLOQUEADO");
                    alterarHashEsSl(hashEs, raiz, codigo, "BLOQUEADO");
                    fprintf(outfile,"Processo %d bloqueado\n",codigo);
                }else if(strcmp(operacao, "DesbloquearHash") == 0){
                    fscanf(infile, "%d", &codigo);
                    alterarHashEaSl(hashEaSl, raiz, codigo, "PRONTO");
                    alterarHashEaSq(hashEaSq, raiz, codigo, "PRONTO");
                    alterarHashEsSl(hashEs, raiz, codigo, "PRONTO");
                    fprintf(outfile,"Processo %d desbloqueado\n",codigo);
                }else if(strcmp(operacao, "ListarHash") == 0){
                    fscanf(infile, "%s", &estado[20]);
                    imprimirHashEaSl(outfile, outhash1, hashEaSl, estado);
                    imprimirHashEaSq(outfile, outhash2, hashEaSq, estado);
                    imprimirHashEsSl(outfile, outhash3, hashEs, estado);
                }else if(strcmp(operacao, "RemoverHash") == 0){
                    fscanf(infile, "%d", &codigo);
                    removerHashEaSl(hashEaSl, raiz, codigo);
                    removerHashEaSq(hashEaSq, raiz, codigo);
                    removerHashEsSl(hashEs, raiz, codigo);
                    fprintf(outfile,"Processo %d removido da hash\n",codigo);
                }else if(strcmp(operacao, "Executar") == 0){
                    fscanf(infile, "%d", &codigo);
                    alterarHashEaSl(hashEaSl, raiz, codigo, "EXECUTANDO");
                    alterarHashEaSq(hashEaSq, raiz, codigo, "EXECUTANDO");
                    alterarHashEsSl(hashEs, raiz, codigo, "EXECUTANDO");
                    fprintf(outfile,"Processo %d executando\n",codigo);
                }else if(strcmp(operacao, "Terminar") == 0){
                    fscanf(infile, "%d %s", &codigo, nome);
                    raiz = terminar(raiz, heap, hashEaSl, hashEaSq, hashEs, codigo, nome);
                    fprintf(outfile,"Processo %d terminado\n",codigo);
                }else if(strcmp(operacao, "Encerrar") == 0){
                    fprintf(outfile,"Encerrando o sistema.\n");
                    break;
                }else{
                    fprintf(outfile, "%s", operacao);
                    fprintf(outfile, "Essa operacao nao existe.\n\n");
                }
            }
        }
        if(opcao == 1 || opcao == 2 || opcao == 3) // caso as opções nao sejam validas
            break;
        else
            printf("Opcao invalida.\n");
    }

    //Medição de tempo das inserções
    printf("Tempo de insercao da hash com enderecamento aberto (sondagem linear): %.8lf ms.\n", ((double)inserirHashEaSltotal) / CLOCKS_PER_SEC * 1000);
    printf("Tempo de insercao da hash com enderecamento aberto (sondagem quadratica): %.8lf ms.\n", ((double)inserirHashEaSqtotal) / CLOCKS_PER_SEC * 1000);
    printf("Tempo de insercao da hash com encadeamento separado (sondagem linear): %.8lf ms.\n", ((double)inserirHashEsSltotal) / CLOCKS_PER_SEC * 1000);

    fprintf(outfile,"Tempo de insercao da hash com enderecamento aberto (sondagem linear): %.8lf ms.\n", ((double)inserirHashEaSltotal) / CLOCKS_PER_SEC * 1000);
    fprintf(outfile,"Tempo de insercao da hash com enderecamento aberto (sondagem quadratica): %.8lf ms.\n", ((double)inserirHashEaSqtotal) / CLOCKS_PER_SEC * 1000);
    fprintf(outfile,"Tempo de insercao da hash com encadeamento separado (sondagem linear): %.8lf ms.\n", ((double)inserirHashEsSltotal) / CLOCKS_PER_SEC * 1000);

    fprintf(outhash1,"Tempo de insercao da hash com enderecamento aberto (sondagem linear): %.8lf ms.\n", ((double)inserirHashEaSltotal) / CLOCKS_PER_SEC * 1000);
    fprintf(outhash2,"Tempo de insercao da hash com enderecamento aberto (sondagem quadratica): %.8lf ms.\n", ((double)inserirHashEaSqtotal) / CLOCKS_PER_SEC * 1000);
    fprintf(outhash3,"Tempo de insercao da hash com encadeamento separado (sondagem linear): %.8lf ms.\n", ((double)inserirHashEsSltotal) / CLOCKS_PER_SEC * 1000);

    //Desaloca as estruturas de dados
    destruir(raiz, heap, hashEs);

    //Fecha os arquivos
    fclose(infile);
    fclose(outfile);
    fclose(outAVL);
    fclose(outheap);
    fclose(outhash1);
    fclose(outhash2);
    fclose(outhash3);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     Funções privadas                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////////// 

int buscarPosicaoHashEaSq(HashEa *hash, char estado[], int codigo){
    if(hash){
        int *chave = (int *)(estado); //Converte a string de estado para inteiro
        int i;
        for (int incr = 0; incr < hash->m; incr++){
            i = (*chave + incr * incr) % hash->m; //Utiliza sondagem quadrática
            if(hash[i].processo != NULL && hash[i].processo->codigo == codigo){
                return i; //Retorna a posição do elemento caso o código de busca seja encontrado
            }
        }
    }
    return -1;
}

int buscarPosicaoHashEaSl(HashEa *hash, char estado[], int codigo){
    if (hash){
        int *chave = (int *)(estado); //Converte a string de estado para inteiro
        int i;
        for(int incr = 0; incr < hash->m; incr++){
            i = (*chave + incr) % hash->m; //Utiliza sondagem linear
            if(hash[i].processo != NULL && hash[i].processo->codigo == codigo){
                return i; //Retorna a posição do elemento caso o código de busca seja encontrado
            }
        }
    }
    return -1;
}

void troca(Processo **v1, Processo **v2){
    //Troca a posição de um processo por outro na heap, utilizando um ponteiro auxiliar
    Processo *aux = *v1;
    *v1 = *v2;
    *v2 = aux;
}

void heapify(MaxHeap *heap, int i){
    //Inicialmente, é assumido que o elemento inicial é o maior
    int maior = i;
    int filhoEsquerda = 2 * i;
    int filhoDireita = 2 * i + 1;

    //Verifica se o filho da esquerda de um elemento na heap é maior que ele
    if(filhoEsquerda <= heap->vetor[0]->prioridade && heap->vetor[filhoEsquerda]->prioridade > heap->vetor[maior]->prioridade){
        maior = filhoEsquerda; //Faz com que a posição do maior nodo seja armazenada
    }

    //Verifica se o filho da direita de um elemento na heap é maior que ele
    if(filhoDireita <= heap->vetor[0]->prioridade && heap->vetor[filhoDireita]->prioridade > heap->vetor[maior]->prioridade){
        maior = filhoDireita; //Faz com que a posição do maior nodo seja armazenada
    }

    if(maior != i){
        //Caso o elemento inicial não seja o maior, sua posição é trocada com o elemento de maior valor
        troca(&heap->vetor[i], &heap->vetor[maior]);
        heapify(heap, maior); //Realiza essa verificação para o novo maior elemento
    }
}

Processo *removerHeapChave(MaxHeap *heap, int chave){
    if(heap != NULL && heap->vetor != NULL){ 
        if(heap->vetor[0]->prioridade == 0){
            return NULL; //Retorna NULL se a tabela estiver vazia
        }
        Processo *processo = heap->vetor[chave];
        heap->vetor[chave] = heap->vetor[heap->vetor[0]->prioridade];
        heap->vetor[0]->prioridade--; //Decrementa a quantidade de elementos na heap
        //Verifica se a tabela continuará estruturada corretamente após a atualização, se não estiver ela será consertada
        heapify(heap, chave);
        return processo;
    }
    return NULL;
}

No *criarNo(Processo *processo){
    No *novo =(No *)malloc(sizeof(No)); //aloca um novo no do tipo no
    novo->processo = processo;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->pai = NULL;

    return novo; //retorna o novo
}

No *sucessor(No *no){ 
    while(no->esquerda != NULL){
        no = no->esquerda; // Percorre a árvore para a esquerda até encontrar o nó mais à esquerda
    }
    return no; // Retorna o nó mais à esquerda, que é o sucessor
}

void destruirHeap(MaxHeap *heap){
    free(heap->vetor); //Desaloca o vetor da tabela heap
    free(heap); //Desaloca o struct da tabela heap
}

void destruirAVL(No *no){
    if(no!=NULL){
        //Percorre a árvore em ordem e desaloca cada nó
        destruirAVL(no->esquerda);
        free(no);
        destruirAVL(no->direita);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     Funções públicas                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////////// 

void destruirHashEs(HashEs *hash){
    if(hash){
        for(int i = 0; i < 3; i++){ //Percorre os três índices
            for(int j = 0; j < (hash[i]).m; j++){ //Percorre todos os espaços de cada lista na tabela
                free(hash[i].ponteiro[j]); //Desaloca cada elemento das listas
            }
        }
    }
}

void removerHashEsSl(HashEs *hash, No *raiz, int codigo){
    No *no = buscarAVL(raiz, codigo); //Usa o código para buscar o processo na árvore AVL
    if (no != NULL){
        int chave = (int)(*no->processo->estado) % 3; //Converte a string de estado para inteiro e usa como indíce
        for(int i = 0; i < hash[chave].m; i++){ //Busca o processo que deve ser removido pelo estado do nó encontrado pelo código na AVL
            if(hash[chave].ponteiro[i] != NULL && hash[chave].ponteiro[i]->codigo == no->processo->codigo){
                for(int j = i; j < hash[chave].m - 1; j++){ //Corrige a posição os processos posteriores da lista
                    hash[chave].ponteiro[j] = hash[chave].ponteiro[j + 1];
                }
                hash[chave].m--; //Decrementa a quantidade de elementos da lista
                break;
            }
        }
    }
}

void removerHashEaSq(HashEa *hash, No *raiz, int codigo){
    No *no = buscarAVL(raiz, codigo); //Usa o código para buscar o processo na árvore AVL
    if(no != NULL){
        //Busca a posição do processo na hash usando o estado do nodo da árvore AVL (sondagem quadrática)
        int i = buscarPosicaoHashEaSq(hash, no->processo->estado, codigo);
        if(i != -1){
            hash[i].status = 'L'; //Libera o espaço na posição encontrada
            hash[i].processo = NULL;
        }
    }
}

void removerHashEaSl(HashEa *hash, No *raiz, int codigo){
    No *no = buscarAVL(raiz, codigo); //Usa o código para buscar o processo na árvore AVL
    if(no != NULL){
        //Busca a posição do processo na hash usando o estado do nodo da árvore AVL (sondagem linear)
        int i = buscarPosicaoHashEaSl(hash, no->processo->estado, codigo);
        if(i != -1){
            hash[i].status = 'L'; //Libera o espaço na posição encontrada
            hash[i].processo = NULL;
        }
    }
}

void imprimirHashEsSl(FILE* outfile, FILE* outhash3, HashEs *hash, char estado[]){
    if (hash) {
        int chave = (int)(*estado) % 3; //Converte a string de estado para inteiro e usa como índice
        fprintf(outfile, "Tabela hash com encadeamento separado (sondagem linear):\n");
        fprintf(outhash3, "Tabela hash com encadeamento separado (sondagem linear):\n");
        if (hash[chave].ponteiro) {
            for (int i = 0; i < hash[chave].m; i++) {
                if (hash[chave].ponteiro[i]) {  // Verifica se ponteiro[i] não é NULL
                    fprintf(outfile, "%d %s %s %d\n", hash[chave].ponteiro[i]->codigo, hash[chave].ponteiro[i]->nome, hash[chave].ponteiro[i]->estado, hash[chave].ponteiro[i]->prioridade);
                    fprintf(outhash3, "%d %s %s %d\n", hash[chave].ponteiro[i]->codigo, hash[chave].ponteiro[i]->nome, hash[chave].ponteiro[i]->estado, hash[chave].ponteiro[i]->prioridade);
                }
            }
        }
        fprintf(outfile, "\n");
        fprintf(outhash3, "\n");
    }
}

void imprimirHashEaSl(FILE* outfile, FILE* outhash1, HashEa *hash, char estado[]){
    if (hash){
        int chave = (int)(*estado); //Converte a string de estado para inteiro
        int i;
        fprintf(outfile, "Tabela hash com enderecamento aberto (sondagem linear):\n");
        fprintf(outhash1, "Tabela hash com enderecamento aberto (sondagem linear):\n");
        for (int incr = 0; incr < hash->m; incr++){
            i = (chave + incr) % hash->m;
            if(hash[i].processo && strcmp(hash[i].processo->estado, estado) == 0){
                fprintf(outfile, "%d %s %s %d\n", hash[i].processo->codigo, hash[i].processo->nome, hash[i].processo->estado, hash[i].processo->prioridade);
                fprintf(outhash1, "%d %s %s %d\n", hash[i].processo->codigo, hash[i].processo->nome, hash[i].processo->estado, hash[i].processo->prioridade);
            }
        }
        fprintf(outfile, "\n");
        fprintf(outhash1, "\n");
    }
}

void imprimirHashEaSq(FILE* outfile, FILE* outhash2, HashEa *hash, char estado[]){
    if (hash){
        int chave = (int)(*estado); //Converte a string de estado para inteiro
        int i;
        fprintf(outfile, "Tabela hash com enderecamento aberto (sondagem quadratica:\n");
        fprintf(outhash2, "Tabela hash com enderecamento aberto (sondagem quadratica):\n");
        for (int incr = 0; incr < hash->m; incr++) {
            i = (chave + incr * incr) % hash->m;
            if (hash[i].processo && strcmp(hash[i].processo->estado, estado) == 0){
                fprintf(outfile, "%d %s %s %d\n", hash[i].processo->codigo, hash[i].processo->nome, hash[i].processo->estado, hash[i].processo->prioridade);
                fprintf(outhash2, "%d %s %s %d\n", hash[i].processo->codigo, hash[i].processo->nome, hash[i].processo->estado, hash[i].processo->prioridade);
            }
        }
        fprintf(outfile, "\n");
        fprintf(outhash2, "\n");
    }
}

void alterarHashEsSl(HashEs  *hash, No *raiz, int codigo, char estado[]){
    No *no = buscarAVL(raiz, codigo); //Usa o código para buscar o processo na árvore AVL
    if (no != NULL){
        int chave = (int)(*no->processo->estado) % 3; //Converte a string de estado para inteiro e usa como índice
        for (int i = 0; i < hash[chave].m; i++){
            //Percorre a tabela em busca do processo com o código do elemento encontrado na AVL
            if (hash[chave].ponteiro[i] != NULL && hash[chave].ponteiro[i]->codigo == no->processo->codigo){
                Processo *processo = hash[chave].ponteiro[i]; //Guarda o processo encontrado para ser re-inserido posteiormente
                for (int j = i; j < hash[chave].m - 1; j++){ //Remove o processo da tabela
                    //Corrige a posição os processos posteriores da lista
                    hash[chave].ponteiro[j] = hash[chave].ponteiro[j + 1];
                }
                hash[chave].m--; //Decrementa a quantidade de elementos da lista
                strcpy(processo->estado, estado); //Atualiza o estado do processo
                inserirHashEsSl(hash, processo); //Insere o processo novamente na tabela
                break;
            }
        }
    }
}

void alterarHashEaSq(HashEa *hash, No *raiz, int codigo, char estado[]){
    No *no = buscarAVL(raiz, codigo); //Usa o código para buscar o processo na árvore AVL
    if(no != NULL){
        //Busca a posição do processo na hash usando o estado do nodo da árvore AVL (sondagem linear)
        int i = buscarPosicaoHashEaSq(hash, no->processo->estado, codigo);
        if(i != -1){
            Processo *processo = hash[i].processo; //Guarda o processo encontrado para ser re-inserido posteiormente
            hash[i].status = 'L'; //Libera o espaço na posição encontrada (remove o processo da tabela)
            hash[i].processo = NULL;
            
            strcpy(processo->estado, estado); //Atualiza o estado do processo
            inserirHashEaSq(hash,processo); //Insere o processo novamente na tabela
        }
    }
}

void alterarHashEaSl(HashEa *hash, No *raiz, int codigo, char estado[]){
    No *no = buscarAVL(raiz, codigo); //Usa o código para buscar o processo na árvore AVL
    if(no != NULL){
        //Busca a posição do processo na hash usando o estado do nodo da árvore AVL (sondagem linear)
        int i = buscarPosicaoHashEaSl(hash, no->processo->estado, codigo);
        if(i != -1){
            Processo *processo = hash[i].processo; //Guarda o processo encontrado para ser re-inserido posteiormente
            hash[i].status = 'L'; //Libera o espaço na posição encontrada (remove o processo da tabela)
            hash[i].processo = NULL;
            
            strcpy(processo->estado, estado); //Atualiza o estado do processo
            inserirHashEaSl(hash,processo); //Insere o processo novamente na tabela
        }
    }
}

void inserirHashEsSl(HashEs *hash, Processo *processo){
    int chave = (int)(*processo->estado) % 3; //Converte a string de estado para inteiro e usa como índice
    if (hash[chave].m >= hash[chave].capacidade){ //Verifica se a quantidade de processos inseridos na lista superou a capacidade
        //Dobra a capacidade se a verificação for confirmada
        hash[chave].capacidade *= 2;
        hash[chave].ponteiro = (Processo **)realloc(hash[chave].ponteiro, hash[chave].capacidade * sizeof(Processo *));
    }
    hash[chave].ponteiro[hash[chave].m] = processo; //Insere o processo no fim da lista do índice correspondente
    hash[chave].m++; //Incrementa a quantidade de elementos da lista
}

HashEs *criarHashEs(HashEs *hash){
    //Aloca três espaços na tabela, um para cada estado
    hash = (HashEs *)malloc(sizeof(HashEs) * 3);
    for (int i = 0; i < 3; i++){ //Inicia cada espaço com uma capacidade inicial de 2 processos
        hash[i].capacidade = 2;
        hash[i].m = 0;
        hash[i].ponteiro = (Processo **)malloc(hash[i].capacidade * sizeof(Processo *));
        for (int j = 0; j < hash[i].capacidade; j++){
            hash[i].ponteiro[j] = NULL; //Aponta todos os ponteiros para NULL
        }
    }
    return hash;
}

void inserirHashEaSl(HashEa *hash, Processo *processo){
    int *chave = (int *)(processo->estado); //Converte a string de estado para inteiro
    int i;
    for(int incr = 0; incr < hash->m; incr++){ //Utiliza sondagem linear para encontrar um espaço livre
        i = (*chave + incr) % hash->m;
        if(hash[i].status == 'L'){ //Ao encontrar um espaço livre, insere o processo na hash
            hash[i].processo = processo;
            hash[i].status = 'O'; //Muda o status para ocupado
            break;
        }
    }
}

void inserirHashEaSq(HashEa *hash, Processo *processo){
    int *chave = (int *)(processo->estado); //Converte a string de estado para inteiro
    int i;
    for(int incr = 0; incr < hash->m; incr++){ //Utiliza sondagem quadrática para encontrar um espaço livre
        i = (*chave + incr * incr) % hash->m;
        if (hash[i].status == 'L'){ //Ao encontrar um espaço livre, insere o processo na hash
            hash[i].processo = processo;
            hash[i].status = 'O'; //Muda o status para ocupado
            break;
        }
    }
}

HashEa *criarHashEa(HashEa *hash, int m){
    //Aloca a quantidade de espaços inserida na main
    hash = (HashEa *)malloc(sizeof(HashEa) * m);
    for(int i = 0; i < m; i++){ //Libera todos os espaços e os informa a capacidade da hash
        hash[i].processo = NULL;
        hash[i].status = 'L';
        hash[i].m = m;
    }
    return hash;
}

int buscarPosicaoHeap(MaxHeap *heap, int chave){
    if(heap != NULL && heap->vetor != NULL){
        if(heap->vetor[0]->prioridade != 0){ //Verifica se a heap não está vazia
            for(int i = 1; i <= heap->capacidade; i++){ //Busca a posição de um elemento na heap pelo número de prioridade
                if(heap->vetor[i]->prioridade == chave){
                    return i; //Retorna sua posição caso seja encontrado
                }
            }
        }
        return -1; //Caso o elemento não seja encontrado ou a heap esteja vazia, retorna -1
    }
    return -1;
}

void alterarHeap(MaxHeap *heap, No *raiz, int codigo, int prioridade){
    No *no = buscarAVL(raiz, codigo); //Usa o código para buscar o processo na árvore AVL
    if(no != NULL){
        //Busca a posição do elemento na heap usando a prioridade do nó encontrado na AVL
        int i = buscarPosicaoHeap(heap, no->processo->prioridade);
        if(i != -1){
            //Caso seja encontrado, sua prioridade é atualizada
            heap->vetor[i]->prioridade = prioridade;
            //Verifica se a tabela continuará estruturada corretamente após a atualização, se não estiver ela será consertada
            heapify(heap, i);
        }
    }
}

No *terminar(No *raiz, MaxHeap *heap, HashEa *hashEaSl, HashEa *hashEaSq, HashEs *hashEsSl, int chave, char nome[]){
    if(raiz){ //se a raiz nao for nula
        No *no = buscarAVL(raiz, chave); //novo no busca a chave a ser terminada
        if(no){ //verifica se o no nao é nulo
            if(strcmp(no->processo->nome,nome) == 0){
                int i = buscarPosicaoHeap(heap, no->processo->prioridade); //retorna a posicao do heap
                if(i != -1){
                    removerHeapChave(heap, i);
                    heapify(heap, i);
                }
                removerHashEaSl(hashEaSl, no, chave);
                removerHashEaSq(hashEaSq, no, chave);
                removerHashEsSl(hashEsSl, no, chave);
                return removerAVL(raiz, chave);
            }
        }
        return raiz;
    }
    return raiz;
}

No *terminarAVL(No *raiz, MaxHeap *heap, HashEa *hashEaSl, HashEa *hashEaSq, HashEs *hashEsSl, int chave){
    if(raiz){ //caso a raiz nao seja nula
        No *no = buscarAVL(raiz, chave); //um novo busca a chave a ser deletada
        if(no){
            int i = buscarPosicaoHeap(heap, no->processo->prioridade); //busca a posicao do heap
            if(i != -1){
                removerHeapChave(heap, i);
                heapify(heap, i);
            }
            removerHashEaSl(hashEaSl, no, chave);
            removerHashEaSq(hashEaSq, no, chave);
            removerHashEsSl(hashEsSl, no, chave);
            return removerAVL(raiz, chave);
        }
        return raiz;
    }
    return raiz;
}

MaxHeap *criarHeap(int capacidade){
    MaxHeap *heap =(MaxHeap*) malloc(sizeof(MaxHeap)); //cria um novo nodo do tipo heap
    heap->vetor =(Processo**) malloc((capacidade + 1)  *sizeof(Processo*)); 
    heap->capacidade = capacidade;
    heap->vetor[0] = NULL;
    heap->vetor[0] =(Processo*) malloc(sizeof(Processo));
    heap->vetor[0]->prioridade = 0;
    return heap; //retorna o heap
}

void inserirHeap(MaxHeap *heap, Processo *processo){
    if(heap != NULL && heap->vetor != NULL){
        if(heap->vetor[0]->prioridade == 0){
            //Caso a heap esteja vazia, o elemento é inserido na primeira posição
            heap->vetor[1] = processo;
            heap->vetor[0]->prioridade++; //Incrementa a quantidade de elementos
        }
        else{
            if(heap->vetor[0]->prioridade == heap->capacidade){
                //Dobra o tamanho da heap caso ela esteja lotada
                Processo **vet = realloc(heap->vetor,(heap->capacidade + 1) * 2 * sizeof(Processo*));
                if(vet != NULL){
                    heap->vetor = vet;
                    heap->capacidade *= 2;
                } else{
                    return;
                }
            }
            heap->vetor[0]->prioridade++; //Incrementa a quantidade de elementos
            //Insere o elemento no fim da heap
            int i = heap->vetor[0]->prioridade;
            int pai = i / 2;
            heap->vetor[i] = processo;

            while(i > 1 && heap->vetor[i]->prioridade > heap->vetor[pai]->prioridade){
                //Elemento inserido troca de posições com seus pais caso ele tenha mais prioridade que eles até encontrar a posição certa
                troca(&heap->vetor[i], &heap->vetor[pai]);
                i = pai;
                pai = i / 2;
            }
        }
    }
}

Processo *removerHeap(MaxHeap *heap, No *raiz){
    if(heap != NULL && heap->vetor != NULL){ //se o heap for nulo quer dizer que está vazio
        if(heap->vetor[0]->prioridade == 0){
            return NULL;
        }
        Processo *processo = heap->vetor[1]; 
        
        heap->vetor[1] = heap->vetor[heap->vetor[0]->prioridade];
        heap->vetor[0]->prioridade--;
        heapify(heap, 1);
        return processo;
    }
    return NULL;
}

void imprimirHeap(FILE* outfile, FILE* outheap, MaxHeap *heap){
    if(heap != NULL && heap->vetor != NULL){
        if(heap->vetor[0]->prioridade == 0){ //se a prioridade for 0 quer dizer que o heap está vazio
            fprintf(outfile, "Heap vazio!\n");
            fprintf(outheap, "Heap vazio!\n");
        } else{
            fprintf(outfile, "Heap: \n");
            fprintf(outheap, "Heap: \n");
            for(int i = 1; i <= heap->vetor[0]->prioridade; i++){ //imprime os valores do heap
                fprintf(outfile, "%d %s %s %d\n", heap->vetor[i]->codigo, heap->vetor[i]->nome, heap->vetor[i]->estado, heap->vetor[i]->prioridade);
                fprintf(outheap, "%d %s %s %d\n", heap->vetor[i]->codigo, heap->vetor[i]->nome, heap->vetor[i]->estado, heap->vetor[i]->prioridade);
            }
        }
        fprintf(outfile,"\n"); //escreve nos arquivos
        fprintf(outheap,"\n"); 
    }
}

Processo *criarProcesso(int codigo, char nome[], char estado[], int prioridade){
    Processo *novo =(Processo *)malloc(sizeof(Processo)); //cria um novo no do tipo processo e aloca o codigo,nome, estado e prioridade para ele
    novo->codigo = codigo;
    strcpy(novo->nome, nome);
    strcpy(novo->estado, estado);
    novo->prioridade = prioridade; 

    return novo; // retorna novo
}

No *inserirAVL(No *no, Processo *processo){  
    if(no == NULL){
        return criarNo(processo); //Cria um novo nó se a árvore estiver vazia
    }

    if(processo->codigo <= no->processo->codigo){
        no->esquerda = inserirAVL(no->esquerda, processo);  // Insere à esquerda se a chave for menor ou igual
        no->esquerda->pai = no;
    }
    else if(processo->codigo > no->processo->codigo){
        no->direita = inserirAVL(no->direita, processo); // Insere à direita se a chave for maior
        no->direita->pai = no;
    }
    
    return no;
}

No *buscarAVL(No *no, int codigo){  
    if(no == NULL){
        return NULL; // Retorna NULL se a árvore estiver vazia ou o valor não for encontrado
    }
    if(no->processo->codigo == codigo){
        return no; // Retorna o nó se o valor for encontrado
    }else if(codigo < no->processo->codigo){
        return buscarAVL(no->esquerda, codigo); // buscarAVL na subárvore esquerda se a chave for menor
    }else{
        return buscarAVL(no->direita, codigo); // buscarAVL na subárvore direita se a chave for maior
    }
}

No *removerAVL(No *no, int chave){ 
    if(no == NULL){
        return NULL; // Retorna NULL se a árvore estiver vazia
    }

    if(chave < no->processo->codigo){
        no->esquerda = removerAVL(no->esquerda, chave); // Remove da subárvore esquerda se a chave for menor
    }else if(chave > no->processo->codigo){
        no->direita = removerAVL(no->direita, chave); // Remove da subárvore direita se a chave for maior
    }
    else{
        // Caso em que encontramos o nó a ser removido
        if(no->esquerda == NULL){
            No *temp = no->direita;
            free(no->processo);
            free(no);
            return temp; // Se o nó não tiver filho esquerdo, é substituído pelo direito
        }
        else if(no->direita == NULL){
            No *temp = no->esquerda;
            free(no->processo);
            free(no);
            return temp; // Se o nó não tiver filho direito, é substituído pelo esquerdo
        }
        No *temp = sucessor(no->direita); // Encontra o sucessor na árvore direita
        no->processo->codigo = temp->processo->codigo; // Copia o valor do sucessor para o nó a ser removido
        strcpy(no->processo->nome, temp->processo->nome);
        strcpy(no->processo->estado, temp->processo->estado);
        no->processo->prioridade = temp->processo->prioridade;
        no->direita = removerAVL(no->direita, temp->processo->codigo); // Remove o sucessor
    }
    return no;
}

void imprimirAVL(FILE* outfile, FILE* outAVL, No *no){
    if(no == NULL) return;
    //Percorre a árvore em ordem imprimindo nos respectivos arquivos
    imprimirAVL(outfile, outAVL, no->esquerda);
    fprintf(outfile, "%d %s %s %d\n", no->processo->codigo, no->processo->nome, no->processo->estado, no->processo->prioridade);
    fprintf(outAVL, "%d %s %s %d\n", no->processo->codigo, no->processo->nome, no->processo->estado, no->processo->prioridade);
    imprimirAVL(outfile, outAVL, no->direita);
}

void destruir(No *raiz, MaxHeap* heap, HashEs *hashEs){
    destruirAVL(raiz);
    destruirHeap(heap);
    destruirHashEs(hashEs);
}