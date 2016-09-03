//
//  main.c
//  simplekeeper
//
//  Created by Vitor Ribeiro Roriz on 10/9/15.
//  Copyright (c) 2015 Vitor Ribeiro Roriz. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "mxml.h"
#include "arvoreXML.h"




void addNode(mxml_node_t *data, int node, int var, int lowNode, int highNode);
void exportArvoreXml(treeNode *arvore, int tamanho);
void plantArvoreXml(treeNode *arvore, mxml_node_t *floresta,int iAvore, int tamanho);
void exportFlorestaXml(treeNode *florestaA[], int nArvores, int* tArvores);
void importNodeXML(mxml_node_t *node, treeNode *nodeStruct, int countNode);
void importArvoreXML(mxml_node_t *arvore, treeNode *arvoreStruct);
void importFlorestaXML(mxml_node_t *xml, treeNode *florestaStruct[]);
int  contaNodesXML(mxml_node_t *arvore);
int  contaArvoresXML(mxml_node_t *xml);
int  contaNodes2XML(mxml_node_t *xml, int *tArvores);
int  alocaFloresta(mxml_node_t *xml_recuperado, treeNode *floresta_r[]);






mxml_type_t type_cb(mxml_node_t *node);




//acrescenta um novo node a uma determinada arvore da estrutura XML
void addNode(mxml_node_t *arvoreP, int node, int var, int lowNode, int highNode)
{
    mxml_node_t *nodeP;   /* <node> */
    mxml_node_t *numNode;
    mxml_node_t *varP;  /* <group> */
    mxml_node_t *lowNodeP;
    mxml_node_t *highNodeP;


    nodeP = mxmlNewElement(arvoreP, "node");
    //mxmlNewText(nodeP, 0, "val = ");
    
    
    numNode = mxmlNewElement(nodeP, "numNode");
    mxmlNewInteger(numNode, node);
    
    varP = mxmlNewElement(nodeP, "var");
    //mxmlNewText(varP, 0, "val = ");
    mxmlNewInteger(varP, var);
    
    lowNodeP = mxmlNewElement(nodeP, "lowNode");
    //mxmlNewText(lowNodeP, 0, "val = ");
    mxmlNewInteger(lowNodeP, lowNode);
    
    highNodeP = mxmlNewElement(nodeP, "highNode");
    //mxmlNewText(highNodeP, 0, "val = ");
    mxmlNewInteger(highNodeP, highNode);



}



//armazena uma unica arvore em formato XML
void exportArvoreXml(treeNode *arvore, int tamanho)
{
    int i = 0;
    FILE *fp;
    
    mxml_node_t *xml;    /* <?xml ... ?> */
    mxml_node_t *arvoreP;   /* <data> */
    //mxml_node_t *florestaP;   /* <data> */
    
    
    xml = mxmlNewXML("1.0");
    //florestaP = mxmlNewElement(xml, "floresta");
    arvoreP = mxmlNewElement(xml, "arvore");

    

    for (i = 0 ; i < tamanho; i++)
    {
        addNode(arvoreP, arvore[i].node, arvore[i].var, arvore[i].lowNode, arvore[i].highNode);
    }
    
    //remove o \n depois de 132 colunas
    mxmlSetWrapMargin(0);
    
    fp = fopen("/Users/vitorroriz/Documents/PG1/ALTERNA/BuDDy/buddy22/src/simplegate/simplegate/arvoreteste.xml", "w");
    mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);
    fclose(fp);

}



//Adiciona uma nova arvore a floresta XML -> Usada por exportFlorestXml
void plantArvoreXml(treeNode *arvore, mxml_node_t *floresta, int iArvore, int tamanho)
{
    int i = 0;
 
    mxml_node_t *arvoreP;   /* <arvore> */
    mxml_node_t *numArvore;

    
    //florestaP = mxmlNewElement(xml, "floresta");
    arvoreP = mxmlNewElement(floresta, "arvore");
    //mxmlNewText(arvoreP, 0, "val = ");
    numArvore = mxmlNewElement(arvoreP, "numArvore");
    mxmlNewInteger(numArvore, iArvore);
    
    
    
    for (i = 0 ; i < tamanho; i++)
    {
        addNode(arvoreP, arvore[i].node, arvore[i].var, arvore[i].lowNode, arvore[i].highNode);
    }
    
    
    
    
}



//Armazena um conjunto de arvores (floresta) em XML
void exportFlorestaXml(treeNode *florestaA[], int nArvores, int* tArvores)
{
    int i = 0;
    FILE *fp;
    
    mxml_node_t *xml;    /* <?xml ... ?> */
    mxml_node_t *florestaP;   /* <data> */
    
    xml = mxmlNewXML("1.0");
    florestaP = mxmlNewElement(xml, "floresta");
    
        
    for (i = 0 ; i < nArvores; i++)
    {
     
        plantArvoreXml(florestaA[i], florestaP, i , tArvores[i]);
        
    }
    

    
    
    //remove o \n depois de 132 colunas
    mxmlSetWrapMargin(0);
    
    fp = fopen("/Users/vitorroriz/Documents/PG1/ALTERNA/BuDDy/buddy22/src/simplegate/simplegate/arvoreteste.xml", "w");
    mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);
    fclose(fp);

    

}




//importa um node completo de um node XML para um node treeNode -> usado por importArvoreXML
void importNodeXML(mxml_node_t *node, treeNode *nodeStruct, int countNode)
{
    int nNode;
    mxml_node_t *auxiliar;
    
    auxiliar = mxmlGetFirstChild(node);
    
    
    nNode = atoi(mxmlGetText(auxiliar, 0));
    nodeStruct[countNode].node = nNode;
    
    auxiliar = mxmlGetNextSibling(auxiliar);
    nodeStruct[countNode].var = atoi(mxmlGetText(auxiliar, 0));
    
    auxiliar = mxmlGetNextSibling(auxiliar);
    nodeStruct[countNode].lowNode = atoi(mxmlGetText(auxiliar, 0));
    
    auxiliar = mxmlGetNextSibling(auxiliar);
    nodeStruct[countNode].highNode = atoi(mxmlGetText(auxiliar, 0));
    
    

}

//utiliza importNodeXML para importar uma arvore completa de uma arvore XML para uma arvore vetor de treeNodes -> usado por importFlorestaXML
void importArvoreXML(mxml_node_t *arvore, treeNode *arvoreStruct)
{

    mxml_node_t *node;
    int i = 0;
    
    node = mxmlFindElement(arvore, arvore, "numArvore", NULL, NULL, MXML_DESCEND); //pega numArvore
    
    while ((node = mxmlFindElement(node, arvore, "node", NULL, NULL, MXML_NO_DESCEND)))
    {
        importNodeXML(node, arvoreStruct, i);
        i++;
    }

    

}


//utiliza importArvoreXML para recuperar uma floresta completa de uma floresta XML para um conjunto de vetores de treeNodes que representam todas as saidas do sistema
void importFlorestaXML(mxml_node_t *xml, treeNode *florestaStruct[])
{
    mxml_node_t *floresta;
    mxml_node_t *arvore;
    int i = 0;
    
    floresta = mxmlFindElement(xml, xml, "floresta", NULL, NULL, MXML_DESCEND); //pega floresta na estrutura XML
    arvore = mxmlGetFirstChild(floresta); //pega o primeiro filho de floresta -> primeira arvore
    
    //condicao de seguranca para do-while nao tentar acessar NULL pointer de estrutura vazia
    if (arvore == NULL)
    {
        return;
    }
 
    do
    {
        importArvoreXML(arvore, florestaStruct[i]);
        i++;
        
    } while ((arvore = mxmlFindElement(arvore, floresta, "arvore", NULL, NULL, MXML_NO_DESCEND)));
    


}





// -----------------------------------------------------------------------------------------------------


//utiliza importNodeXML para importar uma arvore completa de uma arvore XML para uma arvore vetor de treeNodes -> usado por importFlorestaXML
int contaNodesXML(mxml_node_t *arvore)
{
    mxml_node_t *node;
    int i = 0;
    
    node = mxmlFindElement(arvore, arvore, "numArvore", NULL, NULL, MXML_DESCEND); //pega numArvore
    
    while ((node = mxmlFindElement(node, arvore, "node", NULL, NULL, MXML_NO_DESCEND)))
    {
        i++;
    }
    
    return i;
}




//utiliza importArvoreXML para recuperar uma floresta completa de uma floresta XML para um conjunto de vetores de treeNodes que representam todas as saidas do sistema
int contaArvoresXML(mxml_node_t *xml)
{
    mxml_node_t *floresta;
    mxml_node_t *arvore;
    int i = 0;
    
    floresta = mxmlFindElement(xml, xml, "floresta", NULL, NULL, MXML_DESCEND); //pega floresta na estrutura XML
    arvore = mxmlGetFirstChild(floresta); //pega o primeiro filho de floresta -> primeira arvore
    
    //condicao de seguranca para do-while nao tentar acessar NULL pointer de estrutura vazia
    if (arvore == NULL)
    {
        return 0;
    }
    
    do
    {
        i++;
        
    } while ((arvore = mxmlFindElement(arvore, floresta, "arvore", NULL, NULL, MXML_NO_DESCEND)));
    
    
    return i;
}


//utiliza importArvoreXML para recuperar uma floresta completa de uma floresta XML para um conjunto de vetores de treeNodes que representam todas as saidas do sistema
int contaNodes2XML(mxml_node_t *xml, int *tArvores)
{
    mxml_node_t *floresta;
    mxml_node_t *arvore;
    int i = 0;
    int nArvores;
    
    nArvores = contaArvoresXML(xml);
    tArvores = (int *) malloc(nArvores*sizeof(int));
    
    
    floresta = mxmlFindElement(xml, xml, "floresta", NULL, NULL, MXML_DESCEND); //pega floresta na estrutura XML
    arvore = mxmlGetFirstChild(floresta); //pega o primeiro filho de floresta -> primeira arvore
    
    //condicao de seguranca para do-while nao tentar acessar NULL pointer de estrutura vazia
    if (arvore == NULL)
    {
        return 0;
    }
    
    do
    {
        tArvores[i] = contaNodesXML(arvore);
        i++;
        
    } while ((arvore = mxmlFindElement(arvore, floresta, "arvore", NULL, NULL, MXML_NO_DESCEND)));
    
    
    return i;
}



int alocaFloresta(mxml_node_t *xml_recuperado, treeNode *floresta_r[])
{

    int nArvores;
    int *tArvores;
    int i = 0;
    
    nArvores = contaArvoresXML(xml_recuperado);
    contaNodes2XML(xml_recuperado, tArvores);
    
    
    for (i = 0; i < nArvores; i++)
    {
        floresta_r[i] = (treeNode *) malloc(tArvores[i]*sizeof(treeNode));
                         
    }


    return nArvores;
}

// ######################################################################################################


//callback function para determinar data type de cada node na recuperacao de uma arvore XML
mxml_type_t type_cb(mxml_node_t *node)
{
    const char *type;
    
    /*
     * You can lookup attributes and/or use the
     * element name, hierarchy, etc...
     */
    
    type = mxmlElementGetAttr(node, "type");
    if (type == NULL)
        type = mxmlGetElement(node);
    
    if (!strcmp(type, "integer"))
        return (MXML_INTEGER);
    else if (!strcmp(type, "opaque"))
        return (MXML_OPAQUE);
    else if (!strcmp(type, "real"))
        return (MXML_REAL);
    else
        return (MXML_TEXT);
}









