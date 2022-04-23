/* Afonso Ponces de Carvalho ist199046 Projeto 2 de IAED */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stuff.h"

/* este ficheiro contem todas as funcoes necessarias para modificar e encontar 
elementos da base de dados onde sao guardados os dados do utilizador*/

/* cria uma nova estrutura com um caminho, nome e valor*/
node NEW_path_value(char *path, char *valor, char *nome){
    node n;

    n = (node)malloc(sizeof(struct NODE));
    n->valor = (char*)malloc(sizeof(char) * (strlen(valor) + 1));
    n->caminho = (char*)malloc(sizeof(char) * (strlen(path) + 1));
    n->nome = (char*)malloc(sizeof(char) * (strlen(nome) + 1));
    strcpy(n->valor, valor);
    strcpy(n->caminho, path);
    strcpy(n->nome, nome);
    n->head = NULL;
    n->next = NULL;
    return n;
}


/* cria uma nova estrutura com um caminho e um nome*/
node NEW_path(char *path, char *nome){
    node n;

    n = (node)malloc(sizeof(struct NODE));
    n->caminho = (char*)malloc(sizeof(char) * (strlen(path) + 1));
    n->nome = (char*)malloc(sizeof(char) * (strlen(nome) + 1));
    strcpy(n->caminho, path);
    strcpy(n->nome, nome);
    n->head = NULL;
    n->next = NULL;
    n->valor = NULL;
    return n;
}

/* adiciona a estrutura criada com caminho, nome e valor a base de dados*/
node addNode_path_value(node head, char *path, char *valor, char *nome){
    node x;

    if(head == NULL)
        return NEW_path_value(path, valor, nome);
    for(x=head; x->next != NULL; x = x->next)
        ;
    x->next = NEW_path_value(path, valor, nome);
    return head;
}

/* adiciona a estrutura criada com caminho e nome a base de dados*/
node addNode_path(node head, char *path, char *nome){
    node x;

    if(head == NULL)
        return NEW_path(path, nome);
    for(x=head; x->next != NULL; x = x->next)
        ;
    x->next = NEW_path(path, nome);
    return head;
}
/* encontra uma certa estrutura dado o seu caminho na base de dados*/
node findNode_path(node head, char *path){
    node aux;

    for(aux = head;aux!= NULL; aux = aux->next){
        if(strcmp(aux->caminho, path)==0)
            return aux;
    }
    return NULL;
}

/* inicializa a baze de dados onde vao ser guardados todos os caminhos*/
node inicilaiza_estrutura(){
    node head;

    head = (node)malloc(sizeof(struct NODE));
    head->valor = NULL;
    head->next = NULL;
    head->head = NULL;
    head->caminho = NULL;
    return head;
}

/* visita uma estrutura da base de dados e apaga a dando free a todos os componentes */
void visit(node head){
    
    free(head->nome);
    free(head->caminho);
    free(head->valor);
    free(head);
}

/* apaga toda a estrutura criada, dando free aos seus componentes */
void limpa_estrutura(node head){

    if(head == NULL) return ;
    limpa_estrutura(head->head);
    limpa_estrutura(head->next);
    visit(head);
}
