#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void quickSort(int v[], int ini, int fim);
int buscar(int x, int v[], int b, int a);
int buscarRecursivo(int x, int v[], int b, int a);

int main(){
    int v[1001], chave;
    int b = 1000, x = 4995; //
    
    srand(time(NULL));
    
    for(int i = 0; i < 1001; i++) {
        do {
            chave = rand() % 5001;
        } while(buscar(chave, v, b, 0) != -1); // Verifica se o número já existe no vetor
        v[i] = chave;
    }
    
    quickSort(v, 0, 1000); // Alterado para 1000
    
    int d = buscar(x, v, b, 0); //busca binaria iterativa
    int e = buscarRecursivo(x, v, b, 0); //busca binaria recursiva
    int f = buscalinear(x, v, b, 0); //busca linear iterativa
    int g = buscalinearrecursiva(x, v, b, 0); //busca linear recursiva
    
    for(int i = 0; i < 1001; i++) {
        printf("%d\n", v[i]);
    }
    printf("========\\=========\n");
    printf("Busca binaria iterativa:\n");
    if(v[d]==-1){
     		printf("Chave nao encontrada!\n");
    }else{
     		printf("Chave encontrada!\n");
        printf("Posicao: %d\n",v[d]);
        printf("Tempo de execucao: \n");
    }
    printf("Busca binaria recursiva:\n");
    if(v[e]==-1){
     		printf("Chave nao encontrada!\n");
    }else{
     		printf("Chave encontrada!\n");
        printf("Posicao: %d\n",v[e]);
        printf("Tempo de execucao: \n");
    }
    printf("Busca iterativa linear:\n");
    if(v[e]==-1){
     		printf("Chave nao encontrada!\n");
    }else{
     		printf("Chave encontrada!\n");
        printf("Posicao: %d\n",v[e]);
        printf("Tempo de execucao: \n");
    }
    printf("Busca binaria recursiva:\n");
    if(v[e]==-1){
    		printf("Chave nao encontrada!\n");
    }else{
    	printf("Chave encontrada!\n");
        printf("Posicao: %d\n",v[e]);
        printf("Tempo de execucao: \n");
    }
    return 0;
}

void quickSort(int v[], int ini, int fim) {
    int i = ini, j = fim, pivo = v[ini], aux;
    while(i <= j) {
        while(v[i] < pivo) {
            i = i + 1;
        }
        while (v[j] > pivo) {
            j = j - 1;
        }
        if (i <= j) {
            aux = v[j];
            v[j] = v[i];
            v[i] = aux;
            i = i + 1;
            j = j - 1;
        }
    }
    if(ini < j)
        quickSort(v, ini, j);
    if (i < fim)
        quickSort(v, i, fim);
}

int buscarlinear(int x, int v[], int b, int a){
   for(int i=0; i < b; i++){
   		 if(v[i] == x){
       return i;
       }
   }
   return -1;
}

int buscalinearrecursiva(int x, int v[], int b, int a){
		if(a > b){
    	return -1;
    }
    return buscalinearrecursiva(x, v, b, a+1);
}

int buscar(int x, int v[], int b, int a) {
    while (a <= b) {
        int m = a + (b - a) / 2;
        if (v[m] == x) {
            return m;
        } else if (v[m] < x) {
            a = m + 1;
        } else {
            b = m - 1;
        }
    }
    return -1;
}

int buscarRecursivo(int x, int v[], int b, int a) {
    int m = a + (b - a) / 2;
    if (v[m] == x) {
        return m;
    }
    if (b < a) {
        return -1;
    }
    if (v[m] < x) {
        return buscarRecursivo(x, v, b, m + 1);
    }
    return buscarRecursivo(x, v, m - 1, a);
}
