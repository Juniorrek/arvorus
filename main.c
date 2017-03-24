#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define LARGURA_TELA 1024
#define ALTURA_TELA 768

//GLOBALIS
SDL_Window *janela;
SDL_Surface *superficie, *resultado, *superficieTexto;
SDL_Rect objeto;
SDL_Event evento;
TTF_Font *fonte;
SDL_Color corTexto={255, 255, 255};
char preordem[100], inordem[100], posordem[100];

typedef struct NodoArvore{
    char info[10];
    struct NodoArvore *dir, *esq;
}ArvBin;

typedef struct Bloco{
    char dado[100];
    char infi[100];
    float resultado;
    struct NodoArvore *arvore;
    struct Bloco *prox;
}Nodo;

typedef struct Bloco2{
    char dado;
    struct Bloco2 *prox;
}Nodopilha;

typedef struct Bloco3{
    char dado[10];
    struct Bloco3 *prox;
}Nodopilhas;

typedef struct Bloco4{
    struct NodoArvore *arv;
    struct Bloco4 *prox;
}pilhaEnds;


ArvBin * Cria_Nodoa(){
    ArvBin *p;

    p = (ArvBin *) malloc(sizeof(ArvBin));
    if(!p){
        printf("Problema de alocação");
        exit(0);
    }
    return p;
}

void SetColor(int ForgC)
 {
 WORD wColor;

  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;

                       //We use csbi for the wAttributes word.
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
                 //Mask out all but the background attribute, and add in the forgournd color
      wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
      SetConsoleTextAttribute(hStdOut, wColor);
 }
 return;
}

void inicializa_lista(Nodo **N){//inicializa a lista{
    *N = NULL;
}

Nodo * Cria_Nodol(){ //aloca memória para o nodo
    Nodo *p;

    p = (Nodo *) malloc(sizeof(Nodo));
    if(!p){
        printf("Problema de alocação");
        exit(0);
    }
    return p;
}

Nodopilha * Cria_Nodop(){ //aloca memória para o nodo
    Nodopilha *p;

    p = (Nodopilha *) malloc(sizeof(Nodopilha));
    if(!p){
        printf("Problema de alocação");
        exit(0);
    }
    return p;
}

void pushx(Nodopilha **N, char dado){
    Nodopilha *novo, *auxp;

    novo = Cria_Nodop();
    novo->dado = dado;
    novo->prox = *N;
    *N = novo;

    SetColor(2);
    printf("foi empilhado operador %c \n", novo->dado);
    SetColor(15);

    auxp = *N;
    while(auxp != NULL){
        printf("%c \n", novo->dado);
        auxp = auxp->prox;
    }
}

void push(Nodopilha **N, char dado){
    Nodopilha *novo;

    novo = Cria_Nodop();
    novo->dado = dado;
    novo->prox = *N;
    *N = novo;
}

void pushs(Nodopilhas **N, char *dado){
    Nodopilhas *novo, *auxp;

    novo = (Nodopilhas *) malloc(sizeof(Nodopilhas));
    strcpy(novo->dado, dado);
    novo->prox = *N;
    *N = novo;

    SetColor(2);
    printf("foi empilhado operando: ");
    puts(dado);
    SetColor(15);

    auxp = *N;
    while(auxp != NULL){
        puts(auxp->dado);
        auxp = auxp->prox;
    }
}

void pushe(pilhaEnds **N, ArvBin *dado){
    pilhaEnds *novo;

    novo = (pilhaEnds *) malloc(sizeof(pilhaEnds));
    novo->arv=dado;
    novo->prox = *N;
    *N = novo;

    SetColor(2);
    printf("foi empilhado %p e seu operador ", novo->arv);
    puts(novo->arv->info);
    SetColor(15);

    /*auxp = *N;
    while(auxp != NULL){
        printf("%p\n", auxp->arv);
        auxp = auxp->prox;
    }*/
}

pilhaEnds *pope(pilhaEnds **N){
    pilhaEnds *aux, *aux2;

    if(*N == NULL) return *N; //verifica se a lista está vazia
    else{
        aux = (*N)->prox;
        aux2=*N;
        *N=aux;
    }
    SetColor(4);
    printf("%p foi desempilhado\n", aux2->arv);
    SetColor(15);

    /*aux2 = *N;
    while(aux2 != NULL){
        printf("%p\n", aux2->arv);
        aux2 = aux2->prox;
    }*/

    return aux2;
}

int pops(Nodopilhas **N){
    Nodopilhas *aux, *auxp2;
    int ret;

    if(*N == NULL) return 0; //verifica se a lista está vazia
    else{
        aux = (*N)->prox;
        auxp2=*N;
        ret = (int) strtol((*N)->dado, (char **)NULL, 10);
        free(*N);
        *N = aux;
    }
    SetColor(4);
    printf("%d foi desempilhado\n", ret);
    SetColor(15);
    auxp2 = *N;
    while(auxp2 != NULL){
        puts(auxp2->dado);
        auxp2 = auxp2->prox;
    }
    return ret;
}

float pops2(Nodopilhas **N){
    Nodopilhas *aux, *auxp2;
    float ret;

    if(*N == NULL) return 0; //verifica se a lista está vazia
    else{
        aux = (*N)->prox;
        auxp2=*N;
        //ret = (float) strtol((*N)->dado, (char **)NULL, 10);
        ret = (float) atof((*N)->dado);
        free(*N);
        *N = aux;
    }
    SetColor(4);
    printf("%.2f foi desempilhado\n", ret);
    SetColor(15);
    auxp2 = *N;
    while(auxp2 != NULL){
        puts(auxp2->dado);
        auxp2 = auxp2->prox;
    }
    return ret;
}

int pop(Nodopilha **N){
    Nodopilha *aux;

    if(*N == NULL) return 0; //verifica se a lista está vazia
    else{
        aux = (*N)->prox;
        free(*N);
        *N = aux;
    }

    return 1;
}

char popx(Nodopilha **N){
    Nodopilha *aux, *aux3;
    char aux2;

    if(*N == NULL) return '0'; //verifica se a lista está vazia
    else{
        aux = (*N)->prox;
        aux2=(*N)->dado;
        free(*N);
        *N = aux;
    }
    SetColor(4);
    printf("%c foi desempilhado\n", aux2);
    SetColor(15);
    aux3 = *N;
    while(aux3 != NULL){
        printf("%c\n", aux3->dado);
        aux3 = aux3->prox;
    }

    return aux2;
}

void imprime_lista_ecandeada(Nodo *N){
    Nodo *aux;

    if(N == NULL) printf("\n A lista está vazia!!");
    else{
        for(aux = N; aux != NULL; aux = aux->prox) puts(aux->dado);
    }
}

void insere_fim_lista(Nodo **N, char *dado, char *infi){
    Nodo *novo, * aux;

    novo = Cria_Nodol( );
    strcpy(novo->dado, dado);
    strcpy(novo->infi, infi);
    novo->prox = NULL;
    if(*N == NULL) *N = novo;
    else{
        aux = *N;
        while(aux->prox != NULL) aux = aux->prox;
        aux->prox = novo;
    }
}

int prio(char operador){
    switch(operador){
    case '(':
        return 1;
    case '+':
    case '-':
    case '~':
        return 2;
    case '*':
    case '/':
        return 3;
    case '^':
        return 4;
    }
    return 0;
}

int enumero(char caractere){
    if((caractere == '0') ||
       (caractere == '1') ||
       (caractere == '2') ||
       (caractere == '3') ||
       (caractere == '4') ||
       (caractere == '5') ||
       (caractere == '6') ||
       (caractere == '7') ||
       (caractere == '8') ||
       (caractere == '9')) return 1;
    return 0;
}

char *posfixa(char *infixa){
    Nodopilha *pilha = NULL;
    char *saida = malloc(100), dads;
    int i, j;// k;

    strcpy(saida, "\0");
//8+(-9) | 8 9~+
    for(i=0; i<strlen(infixa); i++){
        switch(infixa[i]){
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            j = strlen(saida);
            saida[j] = ' ';
            saida[j+1] = '\0';

            if((infixa[i] == '-') && ((i == 0) || (enumero(infixa[i-1]) == 0))) dads = '~';
            else dads = infixa[i];
            while((pilha != NULL) && (prio(pilha->dado) >= prio(infixa[i]))){
                j = strlen(saida);
                saida[j] = pilha->dado;
                saida[j+1] = '\0';

                pop(&pilha);
            }
            push(&pilha, dads);

            break;
        case '(':
            push(&pilha, infixa[i]);
            break;
        case ')':
            while(pilha->dado != '('){
                j = strlen(saida);
                saida[j] = pilha->dado;
                saida[j+1] = '\0';

                pop(&pilha);
            }
            pop(&pilha);
            break;
        default:
            j = strlen(saida);
            saida[j] = infixa[i];
            saida[j+1] = '\0';
            break;
        }
    }
    while(pilha != NULL){
        j = strlen(saida);
        saida[j] = pilha->dado;
        saida[j+1] = '\0';

        pop(&pilha);
    }

    return saida;
}

void imprime_preordem(ArvBin *arv){
    if(!arv) return;
    puts(arv->info);
    strcat(preordem, arv->info);
    strcat(preordem, ", \0");
    imprime_preordem(arv->esq);
    imprime_preordem(arv->dir);
}

void imprime_inordem(ArvBin *arv){
    if(!arv) return;
    imprime_inordem(arv->esq);
    puts(arv->info);
    strcat(inordem, arv->info);
    strcat(inordem, ", \0");
    imprime_inordem(arv->dir);
}

void imprime_posordem(ArvBin *arv){
    if(!arv) return;
    imprime_posordem(arv->esq);
    imprime_posordem(arv->dir);
    puts(arv->info);
    strcat(posordem, arv->info);
    strcat(posordem, ", \0");
}

int itensPilha(Nodopilhas **pilha){
    Nodopilhas *aux;
    int i=0;

    aux=*pilha;
    while(aux != NULL){
        i++;
        aux=aux->prox;
    }
    return i;
}

void limparArvores(ArvBin *arv){
    if(arv == NULL) return;
    limparArvores(arv->esq);
    limparArvores(arv->dir);
    printf("limpando o:");
    puts(arv->info);
    free(arv);
    //system("pause");
}

char *pops3(Nodopilhas **N){
    Nodopilhas *aux, *auxp2;
    char *ret = malloc(10);

    if(*N == NULL) return 0; //verifica se a lista está vazia
    else{
        aux = (*N)->prox;
        auxp2=*N;
        strcpy(ret, (*N)->dado);
        free(*N);
        *N = aux;
    }
    SetColor(4);
    printf("foi desempilhado\n");
    puts(ret);
    SetColor(15);
    auxp2 = *N;
    while(auxp2 != NULL){
        puts(auxp2->dado);
        auxp2 = auxp2->prox;
    }

    return ret;
}

int voltando=0;

ArvBin *insere_no_arvore(ArvBin *pai, ArvBin *filho, char *dado){
    if(filho != NULL){
        if((filho->dir != NULL) && (filho->esq != NULL)){
            if((enumero(filho->dir->info[0]) == 1) && (enumero(filho->esq->info[0]) == 1)){
                voltando=1;
                return NULL;
            }
        }else if((filho->dir != NULL) && (enumero(filho->dir->info[0]) == 1) && (filho->info[0] == '~')){
            voltando=1;
            return NULL;
        }
    }

    if(filho == NULL){
        filho = Cria_Nodoa();
        filho->dir = NULL;
        filho->esq = NULL;
        strcpy(filho->info, dado);
        if(pai == NULL) return filho;// inserção do primeiro nodo
        if((pai->dir != NULL) && (enumero(pai->dir->info[0]) == 1) && (voltando == 0)) pai->esq = filho;//verifica se insere no direita ou esquerda da sub-árvore
        else{
            if(pai->dir == NULL) pai->dir = filho;
            else pai->esq = filho;
        }
        //voltando = 0;
        return filho;
    }
    if(voltando == 0){
        if((filho->dir != NULL) && (enumero(filho->dir->info[0]) == 1)) insere_no_arvore(filho, filho->esq, dado);
        else insere_no_arvore(filho, filho->dir, dado);
    }
    if(voltando == 1){
        if(filho->info[0] == '~'){
            voltando=1;
            return NULL;
        }

        if((filho->esq != NULL) && (enumero(filho->esq->info[0]) == 1)) return NULL;
        else{
            voltando = 0;
            insere_no_arvore(filho, filho->esq, dado);
        }
    }

    return filho;
}

/*void imprime_posordem(ArvBin *arv){
    if(!arv) return;
    imprime_posordem(arv->esq);
    imprime_posordem(arv->dir);
    puts(arv->info);
    strcat(posordem, arv->info);
    strcat(posordem, ", \0");
}*/

Nodopilhas *pilhaCalculator;
float op1, op2, resultado2;
char resultadoString[10];

void calculator3000(ArvBin *arv){
    if(!arv) return;
    calculator3000(arv->esq);
    calculator3000(arv->dir);
    if((arv->info[0] == '+') || (arv->info[0] == '-') || (arv->info[0] == '*') || (arv->info[0] == '/')){
        op2 = pops2(&pilhaCalculator);
        op1 = pops2(&pilhaCalculator);

        if(arv->info[0] == '+') resultado2 = op1+op2;
        if(arv->info[0] == '-') resultado2 = op1-op2;
        if(arv->info[0] == '*') resultado2 = op1*op2;
        if(arv->info[0] == '/') resultado2 = op1/op2;

        sprintf(resultadoString, "%.2f", resultado2);

        pushs(&pilhaCalculator, resultadoString);
    }else if(arv->info[0] == '~'){
        op1 = pops2(&pilhaCalculator);
        resultado2 = 0-op1;

        sprintf(resultadoString, "%.2f", resultado2);

        pushs(&pilhaCalculator, resultadoString);
    }else pushs(&pilhaCalculator, arv->info);
}

void criaArvores(Nodo **Arvorus){
    Nodo *aux=*Arvorus;
    Nodopilhas *pilha=NULL;
    ArvBin *arvore=NULL;
    char polonesa[100], nodoAinfo[10];
    int i, j=0, itensPilha=0;

    strcpy(polonesa, aux->dado);
    puts(polonesa);
    for(i=0; i<strlen(polonesa); i++){//EMPILHA TUDO DA POLONESA
        if(enumero(polonesa[i]) == 1){
            nodoAinfo[j] = polonesa[i];
            j++;
        }else if((polonesa[i] == '+') || (polonesa[i] == '-') || (polonesa[i] == '*') || (polonesa[i] == '/') || (polonesa[i] == '~') || (polonesa[i] == ' ')){
            if(j != 0){
                nodoAinfo[j] = '\0';
                pushs(&pilha, nodoAinfo);//EMPILHA O NUUMERO ATÉ AGR
                itensPilha++;
                j=0;
            }

            if(polonesa[i] != ' '){
                nodoAinfo[0] = polonesa[i];
                nodoAinfo[1] = '\0';
                pushs(&pilha, nodoAinfo);//EMPILHA O OPERADOR
                itensPilha++;
            }
        }
    }
    i=0;
    while(i < itensPilha){
        strcpy(nodoAinfo, pops3(&pilha));
        arvore = insere_no_arvore(arvore, arvore, nodoAinfo);

        i++;
    }
    printf("---ARVORE PREORDEM---\n");
    imprime_preordem(arvore);
    printf("---ARVORE INORDEM---\n");
    imprime_inordem(arvore);
    printf("---ARVORE POSORDEM---\n");
    imprime_posordem(arvore);
    aux->arvore=arvore;
    calculator3000(arvore);
    aux->resultado=pops2(&pilhaCalculator);
    limparArvores(arvore);
}

void resolvedortreismiu(Nodo **Arvorus){//resolve
    Nodo *aux;
    int sair=0, voltar=0;
    char string[10], stringona[100];
    float resultino;

    aux = *Arvorus;
    puts(aux->dado);

    while((aux != NULL) && (voltar==0)){
        SDL_BlitScaled(resultado, NULL, superficie, NULL);

        preordem[0]=' ';
        preordem[1]='\0';
        inordem[0]=' ';
        inordem[1]='\0';
        posordem[0]=' ';
        posordem[1]='\0';

        criaArvores(&aux);
        resultino = aux->resultado;
        printf("\nRESULTADO FINAL: %.2f , DA EXPRESSAO:",resultino);
        puts(aux->infi);

        strcpy(stringona, "RESULTADO FINAL: ");
        sprintf(string, "%.2f", resultino);
        strcat(stringona, string);
        strcat(stringona, " , DA EXPRESSAO:");
        strcat(stringona, aux->infi);

        fonte=TTF_OpenFont("media/Amatic-Bold.ttf", 62);
        superficieTexto=TTF_RenderText_Solid(fonte, stringona, corTexto);
        objeto.x=100;
        objeto.y=100;
        objeto.w=superficieTexto->w;
        objeto.h=superficieTexto->h;
        SDL_BlitScaled(superficieTexto, NULL, superficie, &objeto);
        //----------
        strcpy(stringona, "PREORDEM:");
        strcat(stringona, preordem);
        superficieTexto=TTF_RenderText_Solid(fonte, stringona, corTexto);
        objeto.x=100;
        objeto.y=200;
        objeto.w=superficieTexto->w;
        objeto.h=superficieTexto->h;
        SDL_BlitScaled(superficieTexto, NULL, superficie, &objeto);

        strcpy(stringona, "INORDEM:");
        strcat(stringona, inordem);
        superficieTexto=TTF_RenderText_Solid(fonte, stringona, corTexto);
        objeto.x=100;
        objeto.y=300;
        objeto.w=superficieTexto->w;
        objeto.h=superficieTexto->h;
        SDL_BlitScaled(superficieTexto, NULL, superficie, &objeto);

        strcpy(stringona, "POSORDEM:");
        strcat(stringona, posordem);
        superficieTexto=TTF_RenderText_Solid(fonte, stringona, corTexto);
        objeto.x=100;
        objeto.y=400;
        objeto.w=superficieTexto->w;
        objeto.h=superficieTexto->h;
        SDL_BlitScaled(superficieTexto, NULL, superficie, &objeto);

        SDL_UpdateWindowSurface(janela);

        do{
            while(SDL_PollEvent(&evento)!=0){
                if(evento.type==SDL_QUIT) voltar=1;
                else if(evento.type==SDL_KEYDOWN){
                    switch( evento.key.keysym.sym ){
                    case SDLK_KP_ENTER:
                    case SDLK_RETURN:
                        sair=1;
                        break;
                    }
                }
            }
        }while((sair == 0) && (voltar==0));//sair é só do loop atual, voltar ele sai do progama

        sair=0;
        aux = aux->prox;
    }
}

int expressaoValida(char *expressao){
    int i, operadores=0, operandos=0, num=0, abre=0, fecha=0;

    for(i=0; i<strlen(expressao); i++){//Percorre a expressao
        if((expressao[i] != '+') &&
           (expressao[i] != '-') &&
           (expressao[i] != '*') &&
           (expressao[i] != '/') &&
           (expressao[i] != '(') &&
           (expressao[i] != ')') &&
           (expressao[i] != '0') &&
           (expressao[i] != '1') &&
           (expressao[i] != '2') &&
           (expressao[i] != '3') &&
           (expressao[i] != '4') &&
           (expressao[i] != '5') &&
           (expressao[i] != '6') &&
           (expressao[i] != '7') &&
           (expressao[i] != '8') &&
           (expressao[i] != '9')) return 0;//invalida, achou outra coisa

        if((expressao[i] == '0') || (expressao[i] == '1') || (expressao[i] == '2') || (expressao[i] == '3') || (expressao[i] == '4') || (expressao[i] == '5') || (expressao[i] == '6') || (expressao[i] == '7') || (expressao[i] == '8') || (expressao[i] == '9'))
            num=1;
        else if(expressao[i] == '(') abre++;
        else if(expressao[i] == ')') fecha++;
        else if((expressao[i] == '+') || (expressao[i] == '-') || (expressao[i] == '*') || (expressao[i] == '/')){
            operadores++;
            if(num == 1){
                operandos++;
                num=0;
            }
        }
    }
    operandos++;
    //if(operandos-1 != operadores) return 0; bug com negaçao pq negaçao empata
    if(operandos < operadores) return 0;

    if(abre != fecha) return 0;//tem '(' e falta ')'
    return 1;
}

int main(int argc, char *argv[]){
    Nodo *lista = NULL, *aux;
    char expressao[100];
    int sair=0, opcaoInserir=3, escrevendo=0, i, um=0, resolvidis=0;
    SDL_Surface *inserirExpressao, *seta1, *seta2, *intro;
    Mix_Chunk *somIntro;
    Mix_Chunk *musica;


    SetColor(15);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);//Inicia funçoes de video do SDL
	janela=SDL_CreateWindow("Arvorus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARGURA_TELA, ALTURA_TELA, 0);//Cria a janela
    superficie=SDL_GetWindowSurface(janela);//superficie-janela
    IMG_Init(IMG_INIT_PNG);//Inicia função para importar png
    TTF_Init();
    fonte=TTF_OpenFont("media/Amatic-Bold.ttf", 38);
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 );//inicializa mixer
    intro=IMG_Load("media/intro.png");//importa o png pra superficie
    inserirExpressao=IMG_Load("media/inserirExpressao.png");
    resultado=IMG_Load("media/resultado.png");
    seta1=IMG_Load("media/seta1.png");
    seta2=IMG_Load("media/seta2.png");
    somIntro=Mix_LoadWAV("media/intro.wav");
    musica=Mix_LoadWAV("media/musica.ogg");
    expressao[0]=' ';
    expressao[1]='\0';

    SDL_Delay(1000);
    SDL_BlitScaled(intro, NULL, superficie, NULL);
    SDL_UpdateWindowSurface(janela);
    Mix_PlayChannel( -1, somIntro, 0 );
    SDL_Delay(2000);

    SDL_StartTextInput();
    escrevendo=1;
    if(um==0) um=1;//pra poder limpar dps
    //Mix_PlayMusic(musica, -1);
    Mix_PlayChannel( -1, musica, -1 );//infinite loops
    do{//inserir
        SDL_BlitScaled(inserirExpressao, NULL, superficie, NULL);

        //IMPRESSAO DA SETA
        objeto.w=155;//tamanho da seta
        objeto.h=75;
        if(opcaoInserir==3){
            objeto.x=110;
            objeto.y=275;
            SDL_BlitScaled(seta1, NULL, superficie, &objeto);
            objeto.x=760;
            SDL_BlitScaled(seta2, NULL, superficie, &objeto);
        }else if(opcaoInserir==2){
            objeto.x=250;
            objeto.y=350;
            SDL_BlitScaled(seta1, NULL, superficie, &objeto);
            objeto.x=620;
            SDL_BlitScaled(seta2, NULL, superficie, &objeto);
        }else{
            objeto.x=250;
            objeto.y=420;
            SDL_BlitScaled(seta1, NULL, superficie, &objeto);
            objeto.x=620;
            SDL_BlitScaled(seta2, NULL, superficie, &objeto);
        }

        //EVENTOS DO TECLADIS
        while( SDL_PollEvent( &evento ) != 0 ){
            if( evento.type == SDL_QUIT ) sair = 1;
            else if(escrevendo==0){
                if( evento.type == SDL_KEYDOWN ){
                    switch( evento.key.keysym.sym ){
                    case SDLK_UP:
                        if(opcaoInserir+1<4)
                            opcaoInserir++;

                        if(opcaoInserir == 3){
                            SDL_StartTextInput();
                            escrevendo=1;
                            if(um==0) um=1;//pra poder limpar dps
                        }
                        break;
                    case SDLK_DOWN:
                        if(opcaoInserir-1>0)
                            opcaoInserir--;
                        break;
                    case SDLK_KP_ENTER:
                    case SDLK_RETURN:
                        /*if(opcaoInserir == 3){//comecar/voltar escrever
                            SDL_StartTextInput();
                            escrevendo=1;
                            if(um==0) um=1;//pra poder limpar dps
                        }else */if(opcaoInserir==2){//inserir na lista
                            if(expressaoValida(expressao) == 1){
                                insere_fim_lista(&lista, posfixa(expressao), expressao);
                                expressao[0]=' ';
                                expressao[1]='\0';
                                printf("-------------------\n");
                                imprime_lista_ecandeada(lista);
                            }
                        }else{//resolver
                            if(lista != NULL){
                                resolvedortreismiu(&lista);
                                resolvidis=1;
                                sair=1;
                            }
                        }
                        break;
                    }
                }
            }else if(escrevendo==1){
                if( evento.type == SDL_KEYDOWN ){
                    if( evento.key.keysym.sym == SDLK_BACKSPACE && strlen(expressao) > 0 ){//APAGAR
                        i=0;
                        while(i<strlen(expressao)) i++;
                        i--;

                        if(i==0) expressao[i]=' ';
                        else expressao[i]='\0';
                    }else if(evento.key.keysym.sym==SDLK_DOWN){//if((evento.key.keysym.sym==SDLK_KP_ENTER) || (evento.key.keysym.sym==SDLK_RETURN)){parar de escrever
                        SDL_StopTextInput();
                        escrevendo=0;
                        opcaoInserir--;
                    }
                }else if( evento.type == SDL_TEXTINPUT ){
                    if(strlen(expressao)<99){
                        if(expressao[0]==' ') strcpy(expressao, evento.text.text);
                        else strcat(expressao, evento.text.text);
                    }
                }
            }
        }

        if(sair == 0){
        superficieTexto=TTF_RenderText_Solid(fonte, expressao, corTexto);
        objeto.x=280;
        objeto.y=290;
        objeto.w=superficieTexto->w;
        objeto.h=superficieTexto->h;
        SDL_BlitScaled(superficieTexto, NULL, superficie, &objeto);

        SDL_UpdateWindowSurface(janela);}
    }while(sair == 0);

    //FREE NOS BAGÜIO
    if(resolvidis == 1){
        while(lista != NULL){//percore todas as expressoes limpando
            aux = lista;
            lista = lista->prox;
            //limparArvores(aux->arvore);//onlione, bugson
            free(aux);
        }
    }

    SDL_FreeSurface(intro);//free nas surfaces
    SDL_FreeSurface(inserirExpressao);
    SDL_FreeSurface(resultado);
    SDL_FreeSurface(seta1);
    SDL_FreeSurface(seta2);

    if(um==1) SDL_FreeSurface(superficieTexto);
    Mix_FreeChunk(somIntro);
    Mix_FreeChunk(musica);
    TTF_CloseFont(fonte);
	SDL_DestroyWindow(janela);//free na janela e na surface dela
	TTF_Quit();
	IMG_Quit();
    Mix_Quit();
    SDL_Quit();

    return 0;
}


