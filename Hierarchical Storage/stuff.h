/* Afonso Ponces de Carvalho ist199046 Projeto 2 de IAED */

#ifndef STUFF_H
#define STUFF_H

/* define um input maximo possivel ao utilizador */
#define Max_input 65536

/* define o que deve ser printado quando e invocado o comando help */
#define HELP "help: Imprime os comandos disponíveis.\n"
#define QUIT "quit: Termina o programa.\n"
#define SET "set: Adiciona ou modifica o valor a armazenar.\n"
#define PRINT "print: Imprime todos os caminhos e valores.\n"
#define FIND "find: Imprime o valor armazenado.\n"
#define LIST "list: Lista todos os componentes imediatos de um sub-caminho.\n"
#define SEARCH "search: Procura o caminho dado um valor.\n"
#define DELETE "delete: Apaga um caminho e todos os subcaminhos.\n"

/* define todos os comandos que podem ser invocados pelo utilizador */
#define C_Help "help"
#define C_Quit "quit"
#define C_Set "set"
#define C_Print "print"
#define C_Find "find"
#define C_List "list"
#define C_Search "search"
#define C_Delete "delete"

/* define um token utilizado para a separaco do caminho em palavras chave */
#define Token "/"

/* define todas as mensagens de erro possiveis do programa */
#define NO_DATA "no data\n"
#define NOT_FOUND "not found\n"
#define NO_MEMORY "no memory\n"

/* estrutura base que compoe a base de dados do programa */
/* nome: parte do caminho
   caminho: caminho total
   valor: valor guardado num certo caminho
   head: sublista do node
   next: proximo node
*/
typedef struct NODE {
    char *nome;
    char *caminho;
    char *valor;
    struct NODE *head;
    struct NODE *next;
} *node;

/* prototipos de todas as funcoes definidas em estruturas.c */
node NEW_path_value(char *path, char *valor, char *nome);

node NEW_path(char *path, char *nome);

node addNode_path_value(node head, char *path, char *valor, char *nome);

node addNode_path(node head, char *path, char *nome);

node findNode_path(node head, char *path);

node inicilaiza_estrutura();

void visit(node head);

void limpa_estrutura(node head);

#endif