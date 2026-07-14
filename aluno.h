#ifndef ALUNO_H
#define ALUNO_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

typedef struct no No;

typedef struct celula{
    int num;
    char oper;
}Cel;

bool criarExpr(No** ppRaiz,char* expressao, int ini, int fim); 
void destroiExpr(No** ppRaiz);
bool avaliaExpr(No* pRaiz, int *res);

void  imprimeArvore(No* pRaiz);
bool lerLinha(char *linha);

#endif