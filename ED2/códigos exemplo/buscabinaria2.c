#include<stdio.h>
#include<stdlib.h>
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
    if (m == x) {
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

int main(){
    int v[] = {1,2,3,4,5,6,7,8,9,10};
    int b = 9, x = 8;
    int d = buscar(x, v, b, 0); 
    printf("%d\n", v[d]);
    int e = buscarRecursivo(x, v, b, 0);
    printf("%d", v[e]);
    return 0;
}