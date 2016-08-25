//
//  main.c
//  plc_engine
//
//  Created by Vitor Ribeiro Roriz on 10/18/15.
//  Copyright (c) 2015 Vitor Ribeiro Roriz. All rights reserved.
//


#define numeroDeEntradas 6
#include "arvoreXML.h"


int entradas[numeroDeEntradas] = {1 , 0 , 1 , 0 , 1 , 0};

bool controleM, controleD, controleEME, controleEMD;

//para a thread de teste/teclado
pthread_t thread;
pthread_attr_t attr;
int rc;
void* keyHunter(void* data);
void executa_plc();
void criaThread();



//floresta_r -> recuperada de um arquivo XML na qual foi salva para exportacao/importacao
treeNode *floresta_r[3]; //vetor de ponteiros para as arvores indexadas recuperadas

mxml_node_t *xml_recuperado; //ponteiro para a abstracao do arquivo XML na biblioteca de tratamento (raiz da estrutura XML)





int main(int argc, const char * argv[])
{
    int nSaidas;
    
    FILE *fileRetriever; //ponteiro para abrir arquivo XML

    
    fileRetriever = fopen("/Users/vitorroriz/Documents/PG1/ALTERNA/S2X/plc_engine/plc_engine/arvoreteste2.xml", "r");
    xml_recuperado = mxmlLoadFile(NULL, fileRetriever, type_cb);
    fclose(fileRetriever);
    
    nSaidas = alocaFloresta(xml_recuperado, floresta_r); //calcula o numero de saidas e a quantidade de nodes de cada saida e aloca espaco em memoria para as estruturas de arvores que formam a floresta (sistema)
    
    importFlorestaXML(xml_recuperado, floresta_r); //recupera arvores para determinar saidas do sistema

    criaThread(); //criacao de uma segunda thread para leitura de teclado -> simulacao de alteracao entradas
    
    executa_plc(); //loop principal que le as entradas do sistema e calcula as saidas baseado nas arvores recuperadas
    

    
    return 0;
}



//---------- Auxiliares --------------------
//funcao de insercao de entradas para teste

void* keyHunter(void* data)
{
    char tecla;
    
    
    while (1)
    {
        
        scanf("%c",&tecla);
        
        
        switch (tecla) {
            case 'a':
                entradas[1] = 1;
                break;
            case 's':
                entradas[2] = 1;
                break;
            case 'd':
                entradas[5] = 1;
                break;
            case 'q':
                entradas[1] = 0;
                entradas[2] = 0;
            default:
                break;
        }
        
        
        
        usleep(1000*200);
        fflush(stdout);
    }
    return NULL;
}



//funcao para Loop Principal da major thread
void executa_plc()
{


    while(1)
    {
        usleep(500000);
        fflush(stdout);
        //printf("main...\n");
        
        printf("--------------------------------------------\n");
        printf("           Entradas          \n");
        printf(" P[%i] FCE[%i] FCD[%i] EME[%i] EMD[%i] B[%i]\n",entradas[0],entradas[1],entradas[2],entradas[3],entradas[4],entradas[5]);
        printf("\n");
        
        
        //        controleM = evaluateOutput(M, entradas);
        //        controleD = evaluateOutput(D, entradas);
        //        controleEMD = evaluateOutput(EMDs,entradas);
        
        
        //ATENCAO: Saidas do sistema sendo calculadas atraves da estrutura recuperada de um arquivo XML
        
        controleM   = evaluateOutput2(floresta_r[1], entradas);
        controleD   = evaluateOutput2(floresta_r[2], entradas);
        controleEMD = evaluateOutput2(floresta_r[0], entradas);
        
        
        controleEME = !controleEMD;
        
        
        
        entradas[0] = !controleM;
        
        if (controleM)
        {
            entradas[1] = 0;
            entradas[2] = 0;
        }
        
        entradas[3] = !controleD;
        entradas[4] = controleD;
        
        
        
        
        printf("             Saidas         \n");
        printf(" M[%i] D[%i] EME[%i] EMD[%i] \n",controleM,controleD,controleEME,controleEMD);
        printf("\n");
        
        if(controleM)
        {
            printf("Status do portao: movendo ");
            
            
            if (controleD)
                printf("para a direita\n");
            else if(!controleD)
                printf("para a esquerda\n");
            
        }
        else if(!controleM)
        {
            if (entradas[1])
                printf("Status do portao: parado e aberto...\n");
            else if (entradas[2])
                printf("Status do portao: parado e fechado...\n");
            else
                printf("Status do portao: parado entreaberto...\n");
        }
        
        entradas[5] = 0;
        
    }



}

void criaThread()
{
    // criacao da Thread para leitura de teclado
    rc = pthread_attr_init(&attr);
    assert(!rc);
    rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    assert(!rc);
    
    rc = pthread_create(&thread, &attr, &keyHunter, NULL);
    if (rc)
    {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

}
