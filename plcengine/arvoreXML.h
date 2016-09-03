//
//  arvoreXML.h
//  simplegate
//
//  Created by Vitor Ribeiro Roriz on 10/10/15.
//  Copyright (c) 2015 Vitor Ribeiro Roriz. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include "mxml.h"
#include <unistd.h>


//estruturas

typedef struct treeNode
{
    int node;
    int var;
    int lowNode;
    int highNode;
} treeNode;


//funcoes

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
int alocaFloresta(mxml_node_t *xml_recuperado, treeNode *floresta_r[]);


int evaluateOutput2(treeNode *arvore, int* entradas);
int verificaBuffer(int *a, int n, int tamanho);
int indexSeeker(int node, treeNode* arvore, int n);

mxml_type_t type_cb(mxml_node_t *node);



