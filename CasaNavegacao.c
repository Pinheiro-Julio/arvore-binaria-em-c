#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    char nome[50];
    struct No* esquerda;
    struct No* direita;
    struct No* pai;
} No;

No* criarNo(const char* nome, No* pai) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    strncpy(novo->nome, nome, sizeof(novo->nome)-1);
    novo->nome[sizeof(novo->nome)-1] = '\0';
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->pai = pai;
    return novo;
}

void liberar(No* raiz) {
    if (raiz == NULL) return;
    liberar(raiz->esquerda);
    liberar(raiz->direita);
    free(raiz);
}

// Constroi uma árvore de exemplo representando cômodos da casa
No* construirCasaExemplo() {
    No* hall = criarNo("Hall de Entrada", NULL);

    hall->esquerda = criarNo("Sala de Estar", hall);
    hall->direita  = criarNo("Cozinha", hall);

    hall->esquerda->esquerda = criarNo("Biblioteca", hall->esquerda);
    hall->esquerda->direita  = criarNo("Quarto", hall->esquerda);

    hall->direita->esquerda = criarNo("Área de Serviço", hall->direita);
    hall->direita->direita  = criarNo("Despensa", hall->direita); // nó folha

    // Torna "Biblioteca" um nó folha (sem saídas)
    // Torna "Quarto" um nó folha
    // Área de Serviço pode ter saída para um pequeno "Jardim" — aqui não adicionamos para manter alguns nós folhas

    return hall;
}

int ehFolha(No* n) {
    return n != NULL && n->esquerda == NULL && n->direita == NULL;
}

void navegarCasa(No* raiz) {
    No* atual = raiz;
    char entrada[100];

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (ehFolha(atual)) {
            printf("--> Este cômodo não tem saída. Fim da navegação.\n");
            break;
        }

        printf("Opções:\n");
        int opt = 0;
        int temEsq = atual->esquerda != NULL;
        int temDir = atual->direita != NULL;
        int temPai = atual->pai != NULL;

        if (temEsq) printf("  1 - Ir para: %s\n", atual->esquerda->nome);
        if (temDir) printf("  2 - Ir para: %s\n", atual->direita->nome);
        if (temPai) printf("  3 - Voltar para: %s\n", atual->pai->nome);
        printf("  0 - Sair\n");

        printf("Escolha uma opção: ");
        if (!fgets(entrada, sizeof(entrada), stdin)) break;
        if (sscanf(entrada, "%d", &opt) != 1) {
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        if (opt == 0) {
            printf("Saindo da navegação.\n");
            break;
        }

        if (opt == 1 && temEsq) {
            atual = atual->esquerda;
        } else if (opt == 2 && temDir) {
            atual = atual->direita;
        } else if (opt == 3 && temPai) {
            atual = atual->pai;
        } else {
            printf("Opção inválida para o cômodo atual. Tente novamente.\n");
        }
    }
}

int main() {
    No* casa = construirCasaExemplo();

    printf("Navegação interativa pela casa (árvore binária).\n");
    printf("Navegue até encontrar um cômodo sem saída (nó folha).\n");

    navegarCasa(casa);

    liberar(casa);
    return 0;
}
