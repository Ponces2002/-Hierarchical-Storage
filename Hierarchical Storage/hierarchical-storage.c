/* Afonso Ponces de Carvalho ist199046 Projeto 2 de IAED */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stuff.h"

/* comando help printa todos os comandos e o q fazem*/
void cmd_help(){
    printf(HELP);
    printf(QUIT);
    printf(SET);
    printf(PRINT);
    printf(FIND);
    printf(LIST);
    printf(SEARCH);
    printf(DELETE);
}

/* funcao auxiliar que da free ao array e aos seus componentes */
void liberta_espaco_array(char **array, int *len){
    int i;

    for(i=0; i< *len; i++){
        free(array[i]);
    }
    free(array);
    free(len);
}

/* utiliza o array de palavras chave e cria um array de caminhos completos */
char** monta_caminhos(char **comps, char **paths, int *len){
    int i;

    paths = realloc(paths, sizeof(char*) * (*len));
    for(i=0; i < *len; i++){
        if(i==0){
            paths[i] = malloc(sizeof(char) * (strlen(comps[i]) + 2));
            strcpy(paths[i], Token);
            strcat(paths[i], comps[i]);
        }
        else{
            paths[i] = malloc(sizeof(char) * (strlen(paths[i-1]) + strlen(comps[i]) + 2));
            strcpy(paths[i], paths[i-1]);
            strcat(paths[i], Token);
            strcat(paths[i], comps[i]);
        }
    }
    for(i=0; i < *len; i++)
        free(comps[i]);
    free(comps);
    return paths;
}

/* divide o caminho num array de palavras chave */
char** estrutura_caminho(char *caminho, char **comps, char **paths, int *len){
    int size = 1, i=0;
    char *comp;

    comp = strtok(caminho,Token);
    while(comp != NULL){
        comps = realloc(comps, sizeof(char*) * size);
        comps[i] = malloc(sizeof(char) * (strlen(comp) +1));
        strcpy(comps[i],comp);
        comp = strtok(NULL, Token);
        i++;
        size++;
    }
    free(caminho);
    *len = i;
    paths = monta_caminhos(comps, paths, len);
    return paths;
}


/* funcao responsavel pela insercao e organizacao da base de dados do programa */
node insere_dados(char **paths, int *len, char *valor, node head, char **nomes){
    int i;
    node curr, aux, parent = NULL;

    if(head->caminho == NULL){
        head->caminho = (char*)malloc(strlen(paths[0])+1);
        strcpy(head->caminho, paths[0]);
        head->nome = (char*)malloc(strlen(nomes[0])+1);
        strcpy(head->nome, nomes[0]);
    }
    curr = head;
    for(i=0; i < *len; i++){
        aux = findNode_path(curr, paths[i]);
        if(i == 0 && *len > 1){
            if(aux == NULL){
                head = addNode_path(curr, paths[i], nomes[i]);
                aux = findNode_path(head, paths[i]);
                curr = aux->head;
            }
            else
                curr = aux->head;
        }
        if(i == 0 && *len == 1){
            if(aux == NULL)
                head = addNode_path_value(curr, paths[i], valor, nomes[i]);
            else{
                aux->valor = realloc(aux->valor, sizeof(char) * (strlen(valor) + 1));
                strcpy(aux->valor, valor);
            }
        }
        if(i == *len - 1 && i != 0){
            if(aux == NULL){
                if (curr == NULL){
                    curr = addNode_path_value(curr, paths[i], valor, nomes[i]);
                    aux = findNode_path(curr, paths[i]);
                    parent->head = aux;
                }
                else
                    curr = addNode_path_value(curr, paths[i], valor, nomes[i]);
            }
            else{
                aux->valor = realloc(aux->valor, sizeof(char) * (strlen(valor) + 1));
                strcpy(aux->valor, valor);
            }
        }
        if(i != 0 && i != *len - 1){
            if(aux == NULL){
                if(curr == NULL){                    
                    curr = addNode_path(curr, paths[i], nomes[i]);
                    aux = findNode_path(curr, paths[i]);
                    parent->head = aux;
                    curr = aux->head;
                }
                else{
                    curr = addNode_path(curr, paths[i], nomes[i]);
                    aux = findNode_path(curr, paths[i]);
                    curr = aux->head;
                }   
            }
            else
                curr = aux->head;
        }
        parent = aux;
    }
    for(i=0; i < *len; i++)       /* da free a toda a memoria que n e mais necessaria */
        free(paths[i]);
    liberta_espaco_array(nomes, len);
    free(paths);free(valor);
    return head;
}


/* cria um array que divie caminho num array de diretorios */
char** diretorios_nomes(char *caminho_aux){
    int size = 1, i=0;
    char *nome, **nomes;

    nomes = malloc(sizeof(char*));
    nome = strtok(caminho_aux, Token);
    while(nome != NULL){
        nomes = realloc(nomes, sizeof(char*) * size);
        nomes[i] = malloc(sizeof(char) * (strlen(nome) +1));
        strcpy(nomes[i],nome);
        nome = strtok(NULL, Token);
        i++;
        size++;
    }
    free(caminho_aux);
    return nomes;
}

/* comando set responsavel por criar e guardar todos os caminhos e valores */
node cmd_set(node head){
    char *caminho, *caminho_aux, *valor, **comps, **paths, *input, **nomes;
    int *len, i;

    len = malloc(sizeof(int));
    comps = malloc(sizeof(char*));
    paths = malloc(sizeof(char*));
    input = malloc(sizeof(char) * Max_input);
    scanf(" %[^\n]", input);
    for(i=0; input[i] != ' '; i++);
    input[i] = '\0';
    caminho = malloc(sizeof(char) * (strlen(input) + 1));
    caminho_aux = malloc(sizeof(char) * (strlen(input) + 1));
    strcpy(caminho, input);
    strcpy(caminho_aux, input);
    valor = malloc(sizeof(char) * (strlen(input + i + 1) + 1));
    strcpy(valor, input + i + 1);
    free(input);
    nomes = diretorios_nomes(caminho_aux);
    paths = estrutura_caminho(caminho, comps, paths, len);
    head = insere_dados(paths, len, valor, head, nomes);
    return head;
}

/* quando o input contem apenas um comando e uma informacao, obtem essa informacao */
char* get_Path(){
    char *input, *caminho;

    input = malloc(sizeof(char) * Max_input);
    scanf(" %[^\n]", input);
    caminho = malloc(sizeof(char) * (strlen(input) + 1));
    strcpy(caminho, input);
    free(input);
    return caminho;
}

/* navega pela estrutura e se encontrar a estrutura printa o valor associado, 
senao printa a mensagem de erro not found ou no data, se este nao tiver valor*/
int Find_Value(node head, char **paths, int *len){
    int i;
    node curr, aux;

    curr = head;
    for(i=0; i<*len; i++){
        aux = findNode_path(curr, paths[i]);
        if(i == *len - 1){
            if(aux == NULL){
                printf(NOT_FOUND); return 0;
            }
            else{
                if(aux->valor == NULL){
                    printf(NO_DATA); return 0;
                }
                else{
                    printf("%s\n", aux->valor); return 0;
                }
            }
        }
        else{
            if(aux == NULL){
                printf(NOT_FOUND); return 0;
            }
            else
                curr = aux->head;
        }
    }
    return 0;
}

/* comando find que printa um valor armazenado num certo caminho */
void cmd_find(node head){
    char *caminho, **comps, **paths;
    int *len;

    comps = malloc(sizeof(char*));
    paths = malloc(sizeof(char*));
    len = malloc(sizeof(int));
    caminho = get_Path();
    paths = estrutura_caminho(caminho, comps, paths, len);
    if(head->caminho != NULL){
        Find_Value(head, paths, len);
        liberta_espaco_array(paths, len);
    }
    else{
        printf(NOT_FOUND);
        liberta_espaco_array(paths, len);
    }
}

/* devolve uma estrutura caso exista, senao printa a mensagem de erro e devolve NULL*/
node Encontra_Dir(node head, char **paths, int *len){
    node curr = head, aux;
    int i;

    for(i=0; i<*len; i++){
        aux = findNode_path(curr, paths[i]);
        if(i == *len - 1){
            if(aux == NULL){
                printf(NOT_FOUND); return NULL;
            }
            else
                return aux;
        }
        else{
            if(aux == NULL){
                printf(NOT_FOUND); return NULL;
            }
            else
                curr = aux->head;
        }
    }
    return NULL;
}

/* cria um lista de nomes se o caminho pertencer a lista principal */
char **Forma_lista_Main(node Dir, char **dirs, int *size){
    int i=0, length = 1;
    node aux;

    for(aux = Dir; aux != NULL; aux=aux->next){
        dirs = realloc(dirs, sizeof(char*) * length);
        dirs[i]= (char*)malloc(sizeof(char*) * (strlen(aux->nome) + 1));
        strcpy(dirs[i], aux->nome);
        i++;
        length++;
    }
    *size = i;
    return dirs;
}

/* cria um lista de nomes se o caminho pertencer a uma sublista da base de dados */
char **Forma_lista(node Dir, char **dirs, int *size){
    int i=0, length = 1;
    node aux;

    for(aux = Dir->head; aux != NULL; aux=aux->next){
        dirs = realloc(dirs, sizeof(char*) * length);
        dirs[i]= (char*)malloc(sizeof(char*) * (strlen(aux->nome) + 1));
        strcpy(dirs[i], aux->nome);
        i++;
        length++;
    }
    *size = i;
    return dirs;
}

/* escolhe um pivo e executa a particao da lista de forma a ficar organizada alfabeticamente  */
int partition(char *a[], int l, int r) {
    int i = l - 1;
    int j = r;
    char *v = a[r], *aux;
    while (i < j) {
        while (strcmp(a[++i], v) > 0);
        while (strcmp(v, a[--j]) > 0)
            if (j == l)
                break;
        if (i < j){
            aux=a[i];
            a[i]=a[j];
            a[j]=aux;
        }
    }
    aux=a[i];
    a[i]=a[r];
    a[r]=aux;
    return i;
}

/* funaco recursiva responsavel pela ordenacao alfabetica de um conjunto de elementos  */
void quicksort(char *a[], int l, int r){
    int i;

    if (r <= l)
        return;
    i = partition(a, l, r);
    quicksort(a, l, i - 1);
    quicksort(a, i + 1, r);
}


/* funcao responsavel por dar print a todos os elementos de um array*/
void print_array(char **dirs, int *size){
    int i;

    for(i=0; i<*size; i++){
        printf("%s\n", dirs[i]);
    }
}

/* comando list que recebe um caminho e lista todos os nomes dos seus subcaminhos
por ordem alfabetica*/
int cmd_list(node head){
    char *caminho, **comps, **paths, **dirs;
    int *len, *size;
    node Dir;

    if(head->caminho == NULL)
        return 0;
    size = malloc(sizeof(int)); dirs = malloc(sizeof(char*));
    if(getchar() == '\n'){
        dirs = Forma_lista_Main(head, dirs, size);
        quicksort(dirs, 0, *size - 1);
        print_array(dirs, size);
    }
    else{
        comps = malloc(sizeof(char*)); paths = malloc(sizeof(char*));
        len = malloc(sizeof(int));
        caminho = get_Path();
        paths = estrutura_caminho(caminho, comps, paths, len);
        Dir = Encontra_Dir(head, paths, len);
        liberta_espaco_array(paths, len);
        if(Dir == NULL)return 0;
        dirs = Forma_lista(Dir, dirs, size);
        quicksort(dirs, 0, *size - 1);
        print_array(dirs, size);
    }
    liberta_espaco_array(dirs, size);
    return 0;
}

/* comando print printa todos os caminhos com valor associado por travessia pre-order */
/*int cmd_print(node head){
    
    if(head->caminho == NULL)
        return 0;
    while(head != NULL){
        if(head->valor != NULL)
            printf("%s %s\n", head->caminho, head->valor);
        if(head->head != NULL)
            cmd_print(head->head);
        head = head->next;
    }
    return 0;
}*/

void da_print(node head){
    
    if(head->valor != NULL){
    	printf("%s %s\n", head->caminho, head->valor);
    }
}

void cmd_print(node head){

	if(head == NULL) return ;
    cmd_print(head->next);
    cmd_print(head->head);
    da_print(head);
    return ;
}

/* encontra o primeiro caminho com valor associado igaul ao valor recebido
por travessia pre-order */
node search_valor(node head, char *valor){
    node aux;

    for(; head != NULL; head = head->next){
        if(head->valor != NULL && strcmp(head->valor, valor) == 0)
            return head;
        aux = search_valor(head->head, valor);
        if(aux != NULL)
            return aux;
    }
    return NULL;
}

/* comando search encontra o primeiro caminho com o valor recebido, se econtrar
printa o caminho, senao printa a mensagem de erro correspondente */
void cmd_search(node head){
    char *valor;
    node path;

    valor = get_Path();
    path = search_valor(head, valor);
    free(valor);
    if(path != NULL)
        printf("%s\n", path->caminho);
    else
        printf(NOT_FOUND);  
}

/* comando delete apaga e da free ao caminho e a todos os seus subcaminhos,
se delete vier sozinho apaga a base de dados toda */
node cmd_delete(node head){

    if(getchar() == '\n'){
        limpa_estrutura(head);
        head = inicilaiza_estrutura();
    }
    return head;
}

/* funcao main responavel por receber o comando inserido e direcionar para as 
devidas funcoes a executar, quando e invocado o comando quit, e libertada toda a 
memoria guardada pela estrutura */
int main(){
    char *cmd;
    node head = inicilaiza_estrutura();

    cmd = malloc(sizeof(char*) * 7);
    while(strcmp(cmd, C_Quit) != 0){
        scanf("%s", cmd);
        if(strcmp(cmd, C_Help) == 0)
            cmd_help();
        else if(strcmp(cmd, C_Set) == 0)
            head = cmd_set(head);
        else if(strcmp(cmd, C_Find) == 0)
            cmd_find(head);
        else if(strcmp(cmd, C_List) == 0)
            cmd_list(head);
        else if(strcmp(cmd, C_Print) == 0)
            cmd_print(head);
        else if(strcmp(cmd, C_Search) == 0)
            cmd_search(head);
        else if(strcmp(cmd, C_Delete) == 0)
            head = cmd_delete(head);
    }
    free(cmd);
    if(head->caminho == NULL)     /* da free a toda a memoria que nao e mais necessaria */
        free(head);
    else 
        limpa_estrutura(head);      
    return 0;
}


