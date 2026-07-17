#include "aluno.h"


struct no{
    Cel dado;
    No* esq;
    No* dir;
};

bool ehOperador(char* str, int pos);
int encontrarOperador(char *expressao,int ini ,int fim);
void removeParenteses(char *str, int *ini, int *fim);
No* criaFolha(int num);
No* criaOperador(char op);
void limparBuffer();

bool ehOperador(char* str, int pos){
    if(pos == 0)
        return false;
    if(str[pos -1] == '*' || str[pos -1] == '/' || str[pos -1] == '+' || str[pos -1] == '-')
        return false;
    if(str[pos -1] == '(')
        return false;
    return true;
}

int encontrarOperador(char *expressao,int ini ,int fim){
    if(!expressao)
        return -1;
    int nivel = 0;
    for(int i = fim; i >= ini; i--){
        if(expressao[i] == ')')
            nivel++;
        else if(expressao[i] == '(')
            nivel--;
        if(nivel != 0)
            continue;
        // procura o operador de menor precedência para colocaar na arvore
        if(expressao[i] == '+') 
            return i;
        else if(expressao[i] == '-'){
            if(ehOperador(expressao, i))
                return i;
            else continue;
        }
    }
    nivel = 0;
    for(int i = fim; i >= ini; i--){
        if(expressao[i] == ')')
            nivel++;
        else if(expressao[i] == '(')
            nivel--;
        if(nivel != 0)
            continue;
        // procura o operador de menor precedência para colocaar na arvore
        if(expressao[i] == '*' || expressao[i] == '/')
            return i;
    }
    return -1;
}
void removeParenteses(char *str, int *ini, int *fim){
    if(!str)
        return;
    int nivel = 0;
    int i;
    while(str[*ini] == '(' && str[*fim] == ')' && *ini < *fim){
        nivel = 0;
        for(i = *ini; i <= *fim; i++){
            if(str[i] == '(')
                nivel++;
            else if(str[i] == ')'){
                nivel--;
            }
            //quer dizer que o parenteses fechou
            if(nivel == 0){
                //marca onde o parenteses fechou
                break;
            } 
        }
        if(!nivel && i == *fim){
            (*ini)++;
            (*fim)--;
        }
    }
}
No* criaFolha(int num){
    No *aux = (No*)malloc(sizeof(No)); 
    if(aux){
        aux->dado.num = num;
        aux->dado.oper = '\0';
        
        aux->dir = NULL;
        aux->esq = NULL;
    }
    else aux = NULL;
    return aux;
}
No* criaOperador(char op){
    No *aux = (No*)malloc(sizeof(No)); 
    if(aux){
        aux->dado.num = INT_MAX;
        aux->dado.oper = op;
        
        aux->dir = NULL;
        aux->esq = NULL;
    }
    else aux = NULL;
    return aux;
}
int convertStrToNum(char *exp, int ini, int fim){
    char temp[20];
    int i = 0;
    while(i + ini <= fim){
        temp[i] = exp[ini +i];
        i++;
    }
    temp[i] = '\0';
    return atoi(temp);
}
bool criarExpr(No **ppRaiz, char *expressao, int ini, int fim){
    if(ppRaiz == NULL || *ppRaiz != NULL || expressao == NULL || ini > fim)
        return false;
    //testes
    printf("\nSubexpressao: ");
    for(int i = ini; i <= fim; i++)
    putchar(expressao[i]);
    printf("\n");

    removeParenteses(expressao, &ini, &fim);
    //testes
    printf("Depois remove: ");
    for(int i = ini; i <= fim; i++)
        putchar(expressao[i]);
    printf("\n");



    int pos = encontrarOperador(expressao, ini, fim);

    printf("Operador = %d", pos);
    if(pos != -1)
        printf(" (%c)", expressao[pos]);
    printf("\n");

    if(pos == -1){

        // expressão começa com um menos
        if(expressao[ini] == '-'){

            // -123
            if(isdigit(expressao[ini+1])){
                int num = convertStrToNum(expressao, ini, fim);
                *ppRaiz = criaFolha(num);
                return true;
            }

            // -(...)
            *ppRaiz = criaOperador('-');

            (*ppRaiz)->esq = criaFolha(0);

            criarExpr(&(*ppRaiz)->dir,
                    expressao,
                    ini + 1,
                    fim);

            return true;
        }

        // número positivo
        int num = convertStrToNum(expressao, ini, fim);
        *ppRaiz = criaFolha(num);
        return true;
    }

    *ppRaiz = criaOperador(expressao[pos]);

    criarExpr(&(*ppRaiz)->esq,expressao,ini,pos - 1);
    criarExpr(&(*ppRaiz)->dir,expressao,pos + 1,fim);

    return true;
}
void  imprimeArvore(No* pRaiz){
    if(!pRaiz)
        return;
    imprimeArvore(pRaiz->dir);
    if(pRaiz->dado.num != INT_MAX)
        printf("%d ", pRaiz->dado.num);
    else
        printf("%c ", pRaiz->dado.oper);
    imprimeArvore(pRaiz->esq);
}
bool avaliaExpr(No* pRaiz, int *res){
    if(pRaiz){
        char operador = pRaiz->dado.oper;
        if(operador != '\0'){
            int esq, dir;
            if(!avaliaExpr(pRaiz->esq, &esq))//se aconteceu divisão por zero
                return false;
            if(!avaliaExpr(pRaiz->dir, &dir))
                return false;

            if(operador == '+')
                *res = esq + dir;
            else if(operador == '-')
                *res = esq - dir;
            else if(operador == '*')
                *res = esq * dir;
            else if(operador == '/'){
                if(dir == 0)
                    return false;
                *res = esq / dir; 
            }
        }
        else
            *res = pRaiz->dado.num;
        return true;
    }
    return false;
}
void destroiExpr(No** ppRaiz){
    if(!(*ppRaiz))
        return;
    destroiExpr(&(*ppRaiz)->esq);
    destroiExpr(&(*ppRaiz)->dir);
    free(*ppRaiz);
}
void limparBuffer(){
    char c;
    while((c = getchar()) != '\n' && c != EOF);
}
bool lerLinha(char *linha,int tam ){
    if(!linha)
        return false;
    fgets(linha,tam, stdin);
    int n = strlen(linha);
    if(linha[n-1] == '\n')
        linha[n-1] = '\0';
    else{
        limparBuffer();
        return false;
    }
    return true;
}
void removeEspacos(char *str){
    int i = 0;
    while(str)
}