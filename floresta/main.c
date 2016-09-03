//
//  arvoreXML.h
//  simplegate
//
//  Created by Vitor Ribeiro Roriz on 20/10/15.
//  Copyright (c) 2015 Vitor Ribeiro Roriz. All rights reserved.
//

#include <stdlib.h>
#include "bdd.h"
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "arvoreXML.h"
#include "mxml.h"
#include <string.h>

#define Nbuffer 3000


//funcoes auxiliares

void plantaArvoreC(int iArvore, int nArvore, treeNode arvore[], FILE *fp);
void plantaFlorestaC(treeNode *floresta[], int *tArvores, int nSaidas);
void alocaMemoria();
void inicializaArvores();
void inicializaBuffers();
void bdd2arvores();
void arvores2arvoresIndexadas();
void setaParametros(char *caminhoEspresso);
void alocaES();
void preencheES();
void formaBDD();
void criaFloresta();
void inicializaFloresta();

bdd * bddAnd(int **AUXentradas, int i);
bdd * bddOr(bdd **AUXsaidas, bdd **AUXentradas, int j);


//variaveis globais
int nEntradas, nSaidas, nExpressoes;
int **nTermos;
bdd *entradasBDD;
bdd *saidasBDD;
treeNode **arvores;
treeNode **arvoresIndexadas;
int **buffers;
int *jbff;
int *tArvores;
int **AUXentradas;
int **AUXsaidas;

int main(void)
{
    FILE *florestaPointer;
    char *caminhoEspresso = "/Users/vitorroriz/Documents/PG1/ALTERNA/S2X/plc_engine/plc_engine/espresso.txt";
    
    setaParametros(caminhoEspresso);
    
    //----
    alocaES();
    
    preencheES();
    
    formaBDD();
    //----
    
    //ALOCADOR DE MEMORIA
    alocaMemoria();
    
    inicializaFloresta();
    
    criaFloresta();


    //----
    
    //exportacao 1: arvoresIndexadas -> arquivo .C
    plantaFlorestaC(arvoresIndexadas, tArvores, nSaidas);
    


    //seta arquivo opcional (nao default) para arquivo XML a ser exportado
    if((florestaPointer = fopen("/Users/vitorroriz/Documents/PG1/ALTERNA/S2X/plc_engine/plc_engine/floresta.xml", "w")));
    else
        printf("ERRO: falha na abertura de arquivo para exportar estrutura XML\n");
    
    //exportacao 2: arvoresIndexadas -> arquivo XML
    exportFlorestaXml(arvoresIndexadas, nSaidas, tArvores, florestaPointer);

    printf("===================================\n");
    printf("   Floresta exportada com sucesso\n\n");
    printf("exportacao 1: formato .XML         \n");
    printf("exportacao 2: formato .C           \n");
    printf("===================================\n");
    
    bdd_printtable(saidasBDD[0]);
    bdd_printtable(saidasBDD[1]);
    bdd_printtable(saidasBDD[2]);

    
    
    //encerra estruturas BDD
    bdd_done();
    
    return 0;
}




//------------------------------------------------------------------------------------------------------------------------
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ DEFINICAO DE FUNCOES AUXILIARES @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//------------------------------------------------------------------------------------------------------------------------


bdd * bddOr(bdd **AUXsaidas, bdd **AUXentradas, int j)
{
    int i = 0;
    bdd aux;
    bdd *retorno;
    
  //inicializa com a primeira saida positiva da tabela verdade espresso
    while (!AUXsaidas[j][i])
    {
        i++;
    }
    
    aux = bdd_addref(*bddAnd(AUXentradas, i));
    
    for (i = i+1 ; i < nExpressoes ; i++)
    {
        if (AUXsaidas[j][i])
        {
            aux = bdd_addref(bdd_or(aux, bdd_addref(*bddAnd(AUXentradas, i))));
        }
    }
    
    retorno = &aux;
    
    return retorno;

}


bdd * bddAnd(int **AUXentradas, int i)
{
    int j = 0;
    bdd aux;
    bdd *retorno;
    
 
    
    if (AUXentradas[0][i])
    {
        aux = entradasBDD[0];
    }
    else
        aux = bdd_addref(bdd_not(entradasBDD[0]));
    
    for (j = 0 ;  j < nEntradas ; j++)
    {
        
        if (AUXentradas[j][i])
        {
            aux = bdd_addref(bdd_and(aux, entradasBDD[j]));
        }
        else
            aux = bdd_addref(bdd_and(aux, bdd_addref(bdd_not(entradasBDD[j]))));
        
    }
    

    
    retorno = &aux;
    
    return retorno;
}


void alocaMemoria()
{
    int i;
    
    
    arvores          = (treeNode **) malloc(nSaidas*sizeof(treeNode *));
    arvoresIndexadas = (treeNode **) malloc(nSaidas*sizeof(treeNode *));
    buffers          = (int **) malloc(nSaidas*sizeof(int *));
    jbff             = (int *)malloc(nSaidas*sizeof(int));
    tArvores         = (int *)malloc(nSaidas*sizeof(int));
    
    for(i = 0 ; i < nSaidas ; i++)
    {
        tArvores[i] = bdd_nodecount(saidasBDD[i])+2;
        arvores[i] = (treeNode*) malloc(tArvores[i]*sizeof(treeNode));
        buffers[i] = (int *) malloc((tArvores[i]-2)*sizeof(int));
        arvoresIndexadas[i] = (treeNode*) malloc(tArvores[i]*sizeof(treeNode));
        
    }
    
   // usleep(500000);
}



void inicializaArvores()
{
    int i;

    for (i = 0; i < nSaidas ; i++)
    {
        arvores[i][0].node     =  0;
        arvores[i][0].var      = -1;
        arvores[i][0].lowNode  =  0;
        arvores[i][0].highNode =  0;
        
        arvores[i][1].node     =  1;
        arvores[i][1].var      = -1;
        arvores[i][1].lowNode  =  1;
        arvores[i][1].highNode =  1;
        
        arvores[i][2].node     =  saidasBDD[i];
        arvores[i][2].var      =  bdd_var(saidasBDD[i]);
        arvores[i][2].lowNode  =  bdd_low(saidasBDD[i]);
        arvores[i][2].highNode =  bdd_high(saidasBDD[i]);
    }
    


}


void inicializaBuffers()
{
    int i;
    
    for (i = 0 ; i < nSaidas;  i++)
    {
        if ((verificaBuffer(buffers[i], bdd_low(saidasBDD[i]), tArvores[i])))
        {
            buffers[i][jbff[i]] = bdd_low(saidasBDD[i]);
            jbff[i]++;
        }
        if ((verificaBuffer(buffers[i], bdd_high(saidasBDD[i]), tArvores[i])))
        {
            buffers[i][jbff[i]] = bdd_high(saidasBDD[i]);
            jbff[i]++;
        }
    }
}


void bdd2arvores()
{

    int i, j = 0 ;
    
    for (j = 0 ; j < nSaidas ; j++)
    {
        
        for (i = 3 ; i < tArvores[j] ; i++)
        {
            
            arvores[j][i].node = buffers[j][i-3];
            arvores[j][i].var = bdd_var(arvores[j][i].node);
            arvores[j][i].lowNode = bdd_low(arvores[j][i].node);
            arvores[j][i].highNode = bdd_high(arvores[j][i].node);
            
            if ((verificaBuffer(buffers[j], bdd_low(arvores[j][i].node), tArvores[j])))
            {
                buffers[j][jbff[j]] = bdd_low(arvores[j][i].node);
                jbff[j]++;
            }
            if ((verificaBuffer(buffers[j], bdd_high(arvores[j][i].node), tArvores[j])))
            {
                buffers[j][jbff[j]] = bdd_high(arvores[j][i].node);
                jbff[j]++;
            }
            
            
        }
        
    }
    

}

void arvores2arvoresIndexadas()
{
    int i,j;
    
    for (j = 0; j < nSaidas; j++)
    {
        for (i = 0; i < tArvores[j]; i++)
        {
            arvoresIndexadas[j][i].node = indexSeeker(arvores[j][i].node, arvores[j], tArvores[j]);
            arvoresIndexadas[j][i].lowNode = indexSeeker(arvores[j][i].lowNode, arvores[j], tArvores[j]);
            arvoresIndexadas[j][i].highNode = indexSeeker(arvores[j][i].highNode, arvores[j], tArvores[j]);
            arvoresIndexadas[j][i].var = arvores[j][i].var;
        }
    }
    

}

void setaParametros(char *caminhoEspresso)
{

    FILE *espressoPointer;
    char espressoBuffer[Nbuffer];
    char *cp;
    char *auxEND;
    char *stringNE;
    char *stringNS;
    int stringL;
    
    
   if((espressoPointer = fopen(caminhoEspresso, "r")) == NULL)
       printf("ERRO: erro ao abrir arquivo espresso\n");
    
    fread(espressoBuffer, Nbuffer*sizeof(char), 1, espressoPointer);
    
    fclose(espressoPointer);
    
    cp     = strstr(espressoBuffer, ".i");
    
    cp = cp + 3;
    
    auxEND = strstr(cp,"\n");
    stringL =((int)(auxEND - cp))/sizeof(char);
    stringNE = malloc(stringL*sizeof(char));

    strncpy(stringNE, cp, stringL);
    
    
    nEntradas = atoi(stringNE);
   // nEntradas = (int) cp[0]-48;
    
    
    cp = strstr(espressoBuffer, ".o");
    cp = cp + 3;
    auxEND = strstr(cp,"\n");
    stringL =((int)(auxEND - cp))/sizeof(char);
    stringNS = malloc(stringL*sizeof(char));
    strncpy(stringNS, cp, stringL);
    nSaidas = atoi(stringNS);

    
    //nSaidas = (int) cp[0]-48;
    
    
    cp = auxEND + 1;
    nExpressoes = ((int) (strstr(espressoBuffer, ".e") - cp)/sizeof(char))/(nSaidas+nEntradas+2);

}


void alocaES()
{
    int i;
    
    
    AUXentradas = (int **) malloc(nEntradas*sizeof(int *));
    AUXsaidas   = (int **) malloc(nSaidas*sizeof(int *));
    entradasBDD = (int *) malloc(nEntradas*sizeof(bdd));
    saidasBDD  = (int *) malloc(nSaidas*sizeof(bdd));
    
    for (i = 0 ; i < nEntradas; i++)
    {
        AUXentradas[i] = (int *) malloc(nExpressoes*sizeof(int));
    }
    
    for (i = 0 ; i < nSaidas; i++)
    {
        AUXsaidas[i]   = (int *) malloc(nExpressoes*sizeof(int));
        
    }
    //usleep(500000);

    
}


void preencheES()
{
    int i,j;
    
    FILE *espressoPointer;
    char espressoBuffer[Nbuffer];
    char *cp;
    
    
    if((espressoPointer = fopen("/Users/vitorroriz/Documents/PG1/ALTERNA/S2X/plc_engine/plc_engine/espresso.txt", "r")) == NULL)
        printf("ERRO: erro ao abrir arquivo espresso\n");
    
    fread(espressoBuffer, Nbuffer*sizeof(char), 1, espressoPointer);
    
    fclose(espressoPointer);
    
    cp = strstr(espressoBuffer, ".o");
    cp = cp + 5;
    
    for(i = 0 ; i < nExpressoes ; i ++)
    {
        for (j = 0 ; j < nEntradas; j++)
        {
            AUXentradas[j][i] = ((int) cp[0]) - 48;
            cp++;
            
        }
        
        cp++;
        for (j = 0;  j < nSaidas; j++)
        {
            AUXsaidas[j][i] = ((int) cp[0] - 48);
            cp++;
        }
        cp++;
    }
    

}


void formaBDD()
{
    int i,j;
    
    
    bdd_init(1000000,100000);
    bdd_setvarnum(nEntradas);
    
    
    
    for (i = 0 ;  i < nEntradas ; i++)
    {
        entradasBDD[i] = bdd_ithvar(i);
    }
    
    
    for (j = 0 ; j < nSaidas; j++)
    {
        saidasBDD[j] = bdd_addref(*bddOr(AUXsaidas, AUXentradas, j));
    }
}

void criaFloresta()
{
    //dinamica da tabelarvore
    bdd2arvores();
    
    //Substitui NODES genericos por index na tabelArvore
    arvores2arvoresIndexadas();
}

void inicializaFloresta()
{
    //condicoes iniciais da tabelarvore
    inicializaArvores();
    
    //inicializa buffer
    inicializaBuffers();

}