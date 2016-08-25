//
//  main.c
//  plc_engine
//
//  Created by Vitor Ribeiro Roriz on 10/18/15.
//  Copyright (c) 2015 Vitor Ribeiro Roriz. All rights reserved.
//

#include "arvoreXML.h"


//------------------------------------------------------------------------------------------------------------------------
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ DEFINICAO DE FUNCOES AUXILIARES @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//------------------------------------------------------------------------------------------------------------------------




// ---------------------------------------------------------------
int evaluateOutput2(treeNode *arvore, int* entradas)
{
    int currentVar, nextPath, nextBDD;
    
    //primeiro node eh a raiz
    nextBDD = arvore[2].node;
    currentVar = arvore[2].var;
    nextPath = entradas[currentVar];
    

    while ((nextBDD != 0) && (nextBDD != 1) )
    
    {
        currentVar = arvore[nextBDD].var;
        nextPath = entradas[currentVar];
        
        if (nextPath)
            nextBDD = arvore[nextBDD].highNode;
        else
            nextBDD = arvore[nextBDD].lowNode;
        
    }

    return nextBDD;
}


// ---------------------------------------------------------------

int verificaBuffer(int *a, int n, int tamanho)
{
    int k;
    
    if ((n == 0) || (n ==1 ))
    {
        return 0;
    }
    
    for (k = 0; k < tamanho; k++)
    {
        if (a[k]== n)
        {
            return 0;
        }
    }

    return 1;
}

// ---------------------------------------------------------------

int indexSeeker(int node, treeNode* arvore, int n)
{
    int i;
    
    
    if (node==0)
        return 0;
    if (node==1)
        return 1;
    

    for (i = 0; i < n+2; i++)
    {

        if (arvore[i].node == node)
        {
            return i;
        }
        
    }
    
    return -1;
    
}



void plantaArvoreC(int iArvore, int nArvore, treeNode arvore[], FILE *fp)
{
    int i;
    
    

    
    fprintf(fp,"treeNode arvoreC%i[%i] = {\n", iArvore, nArvore);
    
    for (i = 0; i < nArvore -1; i++)
    {
        
        fprintf(fp," {%i,%i,%i,%i},\n",arvore[i].node, arvore[i].var, arvore[i].lowNode, arvore[i].highNode);
    }
    fprintf(fp," {%i,%i,%i,%i}\n",arvore[i].node, arvore[i].var, arvore[i].lowNode, arvore[i].highNode);
    fprintf(fp,"};\n");
    
    
}


void plantaFlorestaC(treeNode *floresta[], int *tArvores, int nsaidas)
{
    int i;
    int n = nsaidas;
    
    FILE *fp;
    
    if((fp= fopen("/Users/vitorroriz/Documents/PG1/ALTERNA/S2X/plc_engine/plc_engine/estufa.c", "w")));
    else
        printf("ERRO: falha na abertura de arquivo para exportar estrutura XML\n");
    
    fprintf(fp,"#include \"arvoreXML.h\"\n\n");
    

    for (i = 0; i < n ; i++)
    {
        
        plantaArvoreC(i, tArvores[i], floresta[i], fp);
    }
    
    
    fprintf(fp,"treeNode *florestaC[%i] = {\n",n);

    
    for (i = 0; i < n-1 ; i++)
    {
        //fprintf(fp,"florestaC[%i] = arvoreC%i;\n",i,i);
        fprintf(fp,"arvoreC%i,\n",i);


    }
        fprintf(fp,"arvoreC%i};\n",i);
    
    fclose(fp);
    
    
}

// ====================== Funcoes Mestras ========================================



