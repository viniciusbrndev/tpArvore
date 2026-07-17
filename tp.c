#include "aluno.h"



int main(){
    int n, res;
    scanf("%d ", &n);
    char linha[200];
    if(n <= 0)
        return 0;
    No *pRaiz = NULL;
    for(int i = 0; i < n; i++){
        if(!lerLinha(linha, sizeof(linha)))
            continue;
        int fim = strlen(linha) -1;
        if(!criarExpr(&pRaiz, linha, 0, fim))
            continue;
        if(!avaliaExpr(pRaiz,&res)){
            destroiExpr(&pRaiz);
            continue;
        }
        imprimeArvore(pRaiz);
        destroiExpr(&pRaiz);
    }


}