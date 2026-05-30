/*
 * ============================================================
 *  Desafio Código da Ilha – Edição Free Fire
 *  Nível: NOVATO – Inventário Básico
 * ============================================================
 *  Funcionalidades:
 *    - Adicionar itens à mochila (nome, tipo, quantidade)
 *    - Remover itens pelo nome
 *    - Listar todos os itens cadastrados
 *
 *  Conceitos utilizados:
 *    - struct para representar um item
 *    - Vetor estático com capacidade para até 10 itens
 *    - Menu interativo com switch e do-while
 *    - Leitura via scanf / exibição via printf
 * ============================================================
 */

#include <stdio.h>
#include <string.h>

/* ── Constantes ── */
#define CAPACIDADE_MAXIMA 10  /* Número máximo de itens na mochila */
#define TAM_NOME         50   /* Tamanho máximo do nome do item    */
#define TAM_TIPO         30   /* Tamanho máximo do tipo do item    */

/* ── Estrutura que representa um item da mochila ── */
typedef struct {
    char nome[TAM_NOME];   /* Ex.: "AK47", "Kit Médico"   */
    char tipo[TAM_TIPO];   /* Ex.: "Arma", "Cura", "Munição" */
    int  quantidade;       /* Quantidade disponível        */
} Item;

/* ── Variáveis globais da mochila ── */
Item mochila[CAPACIDADE_MAXIMA]; /* Vetor estático de itens */
int  totalItens = 0;             /* Quantidade de itens atualmente armazenados */

/* ── Protótipos das funções ── */
void adicionarItem(void);
void removerItem(void);
void listarItens(void);
void exibirMenu(void);

/* ============================================================
 *  Função principal
 * ============================================================ */
int main(void) {
    int opcao;

    printf("╔══════════════════════════════════════════╗\n");
    printf("   🔫 FREE FIRE – MOCHILA DE SOBREVIVÊNCIA \n");
    printf("          Nível Novato                     \n");
    printf("╚══════════════════════════════════════════╝\n\n");

    do {
        exibirMenu();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: adicionarItem(); break;
            case 2: removerItem();   break;
            case 3: listarItens();   break;
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
    printf("│  0. Sair                    │\n");
    printf("└─────────────────────────────┘\n");
}

/* ============================================================
 *  Adiciona um novo item à mochila
 *  - Verifica se a mochila está cheia antes de inserir
 *  - Verifica se o item já existe pelo nome
 * ============================================================ */
void adicionarItem(void) {
    int i;
    char nomeTemp[TAM_NOME];

    /* Verifica capacidade */
    if (totalItens >= CAPACIDADE_MAXIMA) {
        printf("\n⚠️  Mochila cheia! Máximo de %d itens.\n", CAPACIDADE_MAXIMA);
        return;
    }

    printf("\n── Adicionar Item ──\n");
    printf("Nome do item : ");
    scanf(" %[^\n]", nomeTemp); /* Lê string com espaços */

    /* Verifica duplicidade */
    for (i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeTemp) == 0) {
            printf("⚠️  Item '%s' já está na mochila!\n", nomeTemp);
            return;
        }
    }

    /* Preenche o novo item */
    strcpy(mochila[totalItens].nome, nomeTemp);

    printf("Tipo do item : ");
    scanf(" %[^\n]", mochila[totalItens].tipo);

    printf("Quantidade   : ");
    scanf("%d", &mochila[totalItens].quantidade);

    /* Valida quantidade */
    if (mochila[totalItens].quantidade <= 0) {
        printf("⚠️  Quantidade deve ser maior que zero.\n");
        return;
    }

    totalItens++;
    printf("✅ Item '%s' adicionado com sucesso!\n", nomeTemp);
}

/* ============================================================
 *  Remove um item da mochila pelo nome
 *  - Busca linear (sem busca especializada, nível Novato)
 *  - Preenche o "buraco" deslocando os itens seguintes
 * ============================================================ */
void removerItem(void) {
    int  i, j;
    int  encontrado = 0;
    char nomeBusca[TAM_NOME];

    if (totalItens == 0) {
        printf("\n⚠️  Mochila vazia! Nada para remover.\n");
        return;
    }

    printf("\n── Remover Item ──\n");
    printf("Nome do item a remover: ");
    scanf(" %[^\n]", nomeBusca);

    for (i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = 1;

            /* Desloca itens à esquerda para fechar o espaço */
            for (j = i; j < totalItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }

            totalItens--;
            printf("✅ Item '%s' removido com sucesso!\n", nomeBusca);
            break;
        }
    }

    if (!encontrado) {
        printf("❌ Item '%s' não encontrado na mochila.\n", nomeBusca);
    }
}

/* ============================================================
 *  Lista todos os itens cadastrados na mochila em formato
 *  de tabela
 * ============================================================ */
void listarItens(void) {
    int i;

    printf("\n📦 MOCHILA – %d/%d iten(s)\n", totalItens, CAPACIDADE_MAXIMA);

    if (totalItens == 0) {
        printf("   (vazia)\n");
        return;
    }

    /* Cabeçalho da tabela */
    printf("┌──────────────────────┬─────────────────┬────────────┐\n");
    printf("│ %-20s │ %-15s │ %-10s │\n", "Nome", "Tipo", "Quantidade");
    printf("├──────────────────────┼─────────────────┼────────────┤\n");

    /* Linhas de dados */
    for (i = 0; i < totalItens; i++) {
        printf("│ %-20s │ %-15s │ %-10d │\n",
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade);
    }

    printf("└──────────────────────┴─────────────────┴────────────┘\n");
}
