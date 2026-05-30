/*
 * ============================================================
 *  Desafio Código da Ilha – Edição Free Fire
 *  Nível: AVENTUREIRO – Mochila com Busca Sequencial
 * ============================================================
 *  Novidades em relação ao Nível Novato:
 *    - Busca sequencial por nome (com flag de controle)
 *    - Nova opção no menu: "Buscar item por nome"
 *    - Exibição detalhada do item encontrado
 *    - Mensagem de erro amigável caso não encontrado
 *
 *  Conceitos utilizados:
 *    - struct, vetor estático (10 itens)
 *    - Busca sequencial + strcmp para comparação de strings
 *    - Flag (int) para indicar se o item foi encontrado
 *    - Menu com switch / do-while
 * ============================================================
 */

#include <stdio.h>
#include <string.h>

/* ── Constantes ── */
#define CAPACIDADE_MAXIMA 10   /* Limite de itens na mochila */
#define TAM_NOME         50    /* Tamanho máximo do nome     */
#define TAM_TIPO         30    /* Tamanho máximo do tipo     */

/* ── Estrutura que representa um item da mochila ── */
typedef struct {
    char nome[TAM_NOME];   /* Identificador do item  */
    char tipo[TAM_TIPO];   /* Categoria do item      */
    int  quantidade;       /* Quantidade disponível  */
} Item;

/* ── Variáveis globais da mochila ── */
Item mochila[CAPACIDADE_MAXIMA];
int  totalItens = 0;

/* ── Protótipos das funções ── */
void adicionarItem(void);
void removerItem(void);
void listarItens(void);
void buscarItem(void);        /* ← NOVIDADE do Nível Aventureiro */
int  buscarIndicePorNome(const char *nome); /* função auxiliar de busca */
void exibirMenu(void);

/* ============================================================
 *  Função principal
 * ============================================================ */
int main(void) {
    int opcao;

    printf("╔══════════════════════════════════════════╗\n");
    printf("║   🔫 FREE FIRE – MOCHILA DE SOBREVIVÊNCIA ║\n");
    printf("║        Nível Aventureiro                 ║\n");
    printf("╚══════════════════════════════════════════╝\n\n");

    do {
        exibirMenu();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: adicionarItem(); break;
            case 2: removerItem();   break;
            case 3: listarItens();   break;
            case 4: buscarItem();    break;   /* ← Nova opção */
            case 0: printf("\n🏝️  Boa sorte na ilha, sobrevivente!\n\n"); break;
            default: printf("\n⚠️  Opção inválida! Tente novamente.\n\n");
        }

    } while (opcao != 0);

    return 0;
}

/* ============================================================
 *  Exibe o menu de opções no terminal
 * ============================================================ */
void exibirMenu(void) {
    printf("\n┌─────────────────────────────┐\n");
    printf("│         MENU PRINCIPAL      │\n");
    printf("├─────────────────────────────┤\n");
    printf("│  1. Adicionar item          │\n");
    printf("│  2. Remover item            │\n");
    printf("│  3. Listar itens            │\n");
    printf("│  4. Buscar item por nome    │\n");   /* ← Nova opção */
    printf("│  0. Sair                    │\n");
    printf("└─────────────────────────────┘\n");
}

/* ============================================================
 *  Busca sequencial: retorna o índice do item cujo nome
 *  coincide com 'nome', ou -1 se não encontrado.
 *
 *  A busca percorre o vetor da posição 0 até totalItens-1.
 *  Ao encontrar a correspondência (strcmp == 0), retorna
 *  imediatamente (short-circuit).
 *
 *  Complexidade: O(n) no pior caso.
 * ============================================================ */
int buscarIndicePorNome(const char *nome) {
    int i;
    for (i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i;   /* Achou: retorna índice */
        }
    }
    return -1;          /* Não encontrado */
}

/* ============================================================
 *  Exibe detalhes completos de um item buscado pelo nome
 *  ── FUNCIONALIDADE NOVA do Nível Aventureiro ──
 * ============================================================ */
void buscarItem(void) {
    char nomeBusca[TAM_NOME];
    int  indice;

    if (totalItens == 0) {
        printf("\n⚠️  Mochila vazia! Nenhum item para buscar.\n");
        return;
    }

    printf("\n── Buscar Item ──\n");
    printf("Nome do item: ");
    scanf(" %[^\n]", nomeBusca);

    indice = buscarIndicePorNome(nomeBusca);

    if (indice != -1) {
        /* Item encontrado: exibe ficha completa */
        printf("\n🔍 Item encontrado!\n");
        printf("┌───────────────────────────────┐\n");
        printf("│  Nome      : %-17s│\n", mochila[indice].nome);
        printf("│  Tipo      : %-17s│\n", mochila[indice].tipo);
        printf("│  Quantidade: %-17d│\n", mochila[indice].quantidade);
        printf("└───────────────────────────────┘\n");
    } else {
        /* Item não encontrado: mensagem amigável */
        printf("\n❌ Item '%s' não encontrado na mochila.\n", nomeBusca);
        printf("   Verifique o nome e tente novamente.\n");
    }
}

/* ============================================================
 *  Adiciona um novo item à mochila.
 *  Reutiliza buscarIndicePorNome() para checar duplicidade.
 * ============================================================ */
void adicionarItem(void) {
    char nomeTemp[TAM_NOME];

    if (totalItens >= CAPACIDADE_MAXIMA) {
        printf("\n⚠️  Mochila cheia! Máximo de %d itens.\n", CAPACIDADE_MAXIMA);
        return;
    }

    printf("\n── Adicionar Item ──\n");
    printf("Nome do item : ");
    scanf(" %[^\n]", nomeTemp);

    /* Verifica duplicidade com a busca sequencial */
    if (buscarIndicePorNome(nomeTemp) != -1) {
        printf("⚠️  Item '%s' já existe na mochila!\n", nomeTemp);
        return;
    }

    strcpy(mochila[totalItens].nome, nomeTemp);

    printf("Tipo do item : ");
    scanf(" %[^\n]", mochila[totalItens].tipo);

    printf("Quantidade   : ");
    scanf("%d", &mochila[totalItens].quantidade);

    if (mochila[totalItens].quantidade <= 0) {
        printf("⚠️  Quantidade deve ser maior que zero.\n");
        return;
    }

    totalItens++;
    printf("✅ Item '%s' adicionado com sucesso!\n", nomeTemp);
}

/* ============================================================
 *  Remove um item da mochila pelo nome.
 *  Usa buscarIndicePorNome() para localizar o índice.
 * ============================================================ */
void removerItem(void) {
    int  j, indice;
    char nomeBusca[TAM_NOME];

    if (totalItens == 0) {
        printf("\n⚠️  Mochila vazia! Nada para remover.\n");
        return;
    }

    printf("\n── Remover Item ──\n");
    printf("Nome do item a remover: ");
    scanf(" %[^\n]", nomeBusca);

    indice = buscarIndicePorNome(nomeBusca);

    if (indice != -1) {
        /* Desloca os itens seguintes para preencher o espaço */
        for (j = indice; j < totalItens - 1; j++) {
            mochila[j] = mochila[j + 1];
        }
        totalItens--;
        printf("✅ Item '%s' removido com sucesso!\n", nomeBusca);
    } else {
        printf("❌ Item '%s' não encontrado na mochila.\n", nomeBusca);
    }
}

/* ============================================================
 *  Lista todos os itens da mochila em formato de tabela
 * ============================================================ */
void listarItens(void) {
    int i;

    printf("\n📦 MOCHILA – %d/%d iten(s)\n", totalItens, CAPACIDADE_MAXIMA);

    if (totalItens == 0) {
        printf("   (vazia)\n");
        return;
    }

    printf("┌──────────────────────┬─────────────────┬────────────┐\n");
    printf("│ %-20s │ %-15s │ %-10s │\n", "Nome", "Tipo", "Quantidade");
    printf("├──────────────────────┼─────────────────┼────────────┤\n");

    for (i = 0; i < totalItens; i++) {
        printf("│ %-20s │ %-15s │ %-10d │\n",
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade);
    }

    printf("└──────────────────────┴─────────────────┴────────────┘\n");
}
