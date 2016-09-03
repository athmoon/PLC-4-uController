//
//  arvoreXML.h
//  simplegate
//
//  Created by Vitor Ribeiro Roriz on 10/10/15.
//  Copyright (c) 2015 Vitor Ribeiro Roriz. All rights reserved.
//
#include "mxml.h"


#ifndef TREENODE
#define TREENODE

typedef struct treeNode
{
    int node;
    int var;
    int lowNode;
    int highNode;
} treeNode;

#endif



//estruturas auxiliares


void addNode(mxml_node_t *data, int node, int var, int lowNode, int highNode);
void exportArvoreXml(treeNode *arvore, int tamanho);
void plantArvoreXml(treeNode *arvore, mxml_node_t *floresta,int iAvore, int tamanho);
void exportFlorestaXml(treeNode *florestaA[], int nArvores, int* tArvores, FILE *fp);


void importNodeXML(mxml_node_t *node, treeNode *nodeStruct, int countNode);
void importArvoreXML(mxml_node_t *arvore, treeNode *arvoreStruct);
void importFlorestaXML(mxml_node_t *xml, treeNode *florestaStruct[]);

int contaArvoreXML(mxml_node_t *arvore);
int contaFlorestaXML(mxml_node_t *xml);


int  evaluateOutput2(treeNode *arvore, int* entradas);
int  verificaBuffer(int *a, int n, int tamanho);
int  indexSeeker(int node, treeNode* arvore, int n);

mxml_type_t type_cb(mxml_node_t *node);



