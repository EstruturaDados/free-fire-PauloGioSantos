/*
 * ============================================================
 *  Desafio Código da Ilha – Edição Free Fire
 *  Nível: Mestre – Torre de Fuga
 * ============================================================
 *  Missão:
 *    A última safe zone está se fechando. Para escapar da ilha,
 *    o jogador deve montar uma torre de resgate organizando os
 *    componentes na ordem certa e com a prioridade correta.
 *
 *  Funcionalidades:
 *    - Cadastro de até 20 componentes (nome, tipo, prioridade)
 *    - Bubble Sort por nome (string)
 *    - Insertion Sort por tipo (string)
 *    - Selection Sort por prioridade (int)
 *    - Medição de comparações e tempo de execução (clock())
 *    - Busca binária por nome (exige ordenação prévia por nome)
 *    - Confirmação visual do componente-chave da torre
 *
 *  Conceitos utilizados:
 *    - struct Componente
 *    - Bubble Sort, Insertion Sort, Selection Sort
 *    - Binary Search
 *    - clock() para medição de tempo
 *    - fgets() para entrada segura de strings
 *    - Ponteiros para funções (medirTempo)
 *    - Modularização em funções separadas
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ── Constantes ── */
#define MAX_COMPONENTES   20   /* Limite máximo de componentes na torre   */
#define TAM_NOME          30   /* Tamanho máximo do nome do componente    */
#define TAM_TIPO          20   /* Tamanho máximo do tipo do componente    */
#define PRIORIDADE_MIN     1   /* Menor prioridade válida                 */
#define PRIORIDADE_MAX    10   /* Maior prioridade válida                 */

/* ── Estrutura principal ── */
/*
 * Componente: representa uma peça necessária para montar a torre de fuga.
 *   nome      – identificador único do componente (ex: "chip central")
 *   tipo      – categoria funcional (ex: "controle", "propulsão")
 *   prioridade – ordem de importância na montagem (1 = baixa, 10 = crítica)
 */
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int  prioridade;
} Componente;

/* ── Variáveis globais ── */
Componente torre[MAX_COMPONENTES];   /* Vetor de componentes cadastrados  */
int totalComponentes = 0;            /* Quantidade atual de componentes   */

/*
 * Flag de estado: indica se o vetor está ordenado por nome.
 * A busca binária exige esse pré-requisito.
 * É definida como int (0/1) para manter compatibilidade com C89/C90.
 */
int ordenadoPorNome = 0;

/* ── Protótipos das funções ── */
void   cadastrarComponente(void);
void   mostrarComponentes(Componente v[], int n);
void   bubbleSortNome(Componente v[], int n);
void   insertionSortTipo(Componente v[], int n);
void   selectionSortPrioridade(Componente v[], int n);
int    buscaBinariaPorNome(Componente v[], int n, char nome[]);
void   medirTempo(void (*algoritmo)(Componente[], int),
                  Componente v[], int n);
void   executarOrdenacao(int criterio);
void   executarBuscaBinaria(void);
void   exibirMenu(void);
void   exibirMenuOrdenacao(void);
void   limparBuffer(void);

/* ============================================================
 *  Função principal
 * ============================================================ */
int main(void) {
    int opcao;

    printf("╔══════════════════════════════════════════════╗\n");
    printf("  🔫 FREE FIRE – TORRE DE FUGA  |  Nível Mestre \n");
    printf("     A safe zone está fechando… ative a torre!   \n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    do {
        exibirMenu();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer(); /* descarta o '\n' residual do scanf */

        switch (opcao) {
            case 1: cadastrarComponente();    break;
            case 2: mostrarComponentes(torre, totalComponentes); break;
            case 3: executarOrdenacao(1);     break;  /* Bubble  – nome      */
            case 4: executarOrdenacao(2);     break;  /* Insertion – tipo    */
            case 5: executarOrdenacao(3);     break;  /* Selection – prio    */
            case 6: executarBuscaBinaria();   break;
            case 0:
                printf("\n🏝️  Torre ativada! Boa sorte na fuga, sobrevivente!\n\n");
                break;
            default:
                printf("\n⚠️  Opção inválida! Tente novamente.\n\n");
        }

    } while (opcao != 0);

    return 0;
}

/* ============================================================
 *  limparBuffer – descarta caracteres pendentes no stdin.
 *  Necessário após scanf("%d") para que o próximo fgets
 *  não leia o '\n' residual.
 * ============================================================ */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ============================================================
 *  exibirMenu – mostra o menu principal com estado atual
 * ============================================================ */
void exibirMenu(void) {
    printf("\n┌──────────────────────────────────────────┐\n");
    printf("│             MENU PRINCIPAL               │\n");
    printf("├──────────────────────────────────────────┤\n");
    printf("│  1. Cadastrar componente                 │\n");
    printf("│  2. Listar componentes                   │\n");
    printf("│  3. Ordenar por nome   (Bubble Sort)     │\n");
    printf("│  4. Ordenar por tipo   (Insertion Sort)  │\n");
    printf("│  5. Ordenar por prio.  (Selection Sort)  │\n");
    printf("│  6. Buscar componente-chave (Binária)    │\n");
    printf("│  0. Sair / Ativar torre                  │\n");
    printf("└──────────────────────────────────────────┘\n");
    printf("  Componentes: %d/%d", totalComponentes, MAX_COMPONENTES);
    if (ordenadoPorNome)
        printf("  |  🔤 Ordenado por nome – busca binária disponível\n");
    else
        printf("  |  ⚠️  Ordene por nome para usar busca binária\n");
}

/* ============================================================
 *  cadastrarComponente – lê nome, tipo e prioridade via fgets.
 *  Usa fgets para captura segura de strings com espaços.
 *  Valida a prioridade no intervalo [PRIORIDADE_MIN, PRIORIDADE_MAX].
 * ============================================================ */
void cadastrarComponente(void) {
    char buffer[TAM_NOME];
    int  pri;
    int  i;

    if (totalComponentes >= MAX_COMPONENTES) {
        printf("\n⚠️  Torre cheia! Máximo de %d componentes.\n", MAX_COMPONENTES);
        return;
    }

    printf("\n── Cadastrar Componente ──\n");

    /* ── Leitura do nome ── */
    printf("Nome do componente: ");
    fgets(buffer, TAM_NOME, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; /* remove '\n' do fgets */

    /* Verifica duplicata pelo nome */
    for (i = 0; i < totalComponentes; i++) {
        if (strcmp(torre[i].nome, buffer) == 0) {
            printf("⚠️  Componente '%s' já cadastrado!\n", buffer);
            return;
        }
    }
    strcpy(torre[totalComponentes].nome, buffer);

    /* ── Leitura do tipo ── */
    printf("Tipo (ex: controle, propulsão, suporte): ");
    fgets(torre[totalComponentes].tipo, TAM_TIPO, stdin);
    torre[totalComponentes].tipo[strcspn(torre[totalComponentes].tipo, "\n")] = '\0';

    /* ── Leitura e validação da prioridade ── */
    do {
        printf("Prioridade (%d–%d): ", PRIORIDADE_MIN, PRIORIDADE_MAX);
        scanf("%d", &pri);
        limparBuffer();
        if (pri < PRIORIDADE_MIN || pri > PRIORIDADE_MAX)
            printf("⚠️  Valor inválido! Digite entre %d e %d.\n",
                   PRIORIDADE_MIN, PRIORIDADE_MAX);
    } while (pri < PRIORIDADE_MIN || pri > PRIORIDADE_MAX);

    torre[totalComponentes].prioridade = pri;
    totalComponentes++;

    /*
     * Qualquer novo cadastro invalida a ordenação por nome,
     * impedindo busca binária sobre um vetor desatualizado.
     */
    ordenadoPorNome = 0;

    printf("✅ Componente '%s' adicionado à torre!\n", buffer);
}

/* ============================================================
 *  mostrarComponentes – exibe o vetor formatado em tabela.
 *  Parâmetros:
 *    v – vetor de componentes (pode ser uma cópia temporária)
 *    n – número de elementos a exibir
 * ============================================================ */
void mostrarComponentes(Componente v[], int n) {
    int i;

    printf("\n📦 COMPONENTES DA TORRE – %d/%d\n", n, MAX_COMPONENTES);

    if (n == 0) {
        printf("   (nenhum componente cadastrado)\n");
        return;
    }

    printf("┌──────────────────────────────┬──────────────────────┬────────────┐\n");
    printf("│ %-28s │ %-20s │ %-10s │\n", "Nome", "Tipo", "Prioridade");
    printf("├──────────────────────────────┼──────────────────────┼────────────┤\n");
    for (i = 0; i < n; i++) {
        printf("│ %-28s │ %-20s │ %-10d │\n",
               v[i].nome, v[i].tipo, v[i].prioridade);
    }
    printf("└──────────────────────────────┴──────────────────────┴────────────┘\n");
}

/* ============================================================
 *  bubbleSortNome – ordena o vetor por nome em ordem crescente.
 *
 *  Algoritmo Bubble Sort:
 *    Percorre o vetor repetidamente comparando pares adjacentes.
 *    Troca os elementos fora de ordem e "borbulha" o maior
 *    para o final a cada passagem.
 *    Usa flag 'trocou' para encerrar cedo se já estiver ordenado.
 *
 *  Parâmetros:
 *    v – vetor de componentes
 *    n – número de elementos
 *
 *  Retorno implícito: atualiza a variável global 'comparacoesBubble'
 *  por meio do ponteiro passado em medirTempo.
 *
 *  Complexidade: O(n²) pior caso, O(n) melhor caso (já ordenado)
 * ============================================================ */

/* Variável global para compartilhar contagem com medirTempo */
static int contadorComparacoes = 0;

void bubbleSortNome(Componente v[], int n) {
    int       i, j, trocou;
    Componente temp;

    contadorComparacoes = 0;

    for (i = 0; i < n - 1; i++) {
        trocou = 0; /* otimização: encerra se nenhuma troca ocorrer */

        for (j = 0; j < n - 1 - i; j++) {
            contadorComparacoes++;

            /* Compara nomes lexicograficamente */
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                /* Troca os componentes de posição */
                temp     = v[j];
                v[j]     = v[j + 1];
                v[j + 1] = temp;
                trocou   = 1;
            }
        }

        if (!trocou) break; /* vetor já ordenado: encerra antecipadamente */
    }
}

/* ============================================================
 *  insertionSortTipo – ordena o vetor por tipo em ordem crescente.
 *
 *  Algoritmo Insertion Sort:
 *    Para cada elemento a partir do segundo, guarda-o como 'chave'
 *    e desloca os elementos anteriores maiores que ele uma posição
 *    à direita, inserindo a chave na posição correta.
 *
 *  Parâmetros:
 *    v – vetor de componentes
 *    n – número de elementos
 *
 *  Complexidade: O(n²) pior caso, O(n) melhor caso (já ordenado)
 * ============================================================ */
void insertionSortTipo(Componente v[], int n) {
    int        i, j;
    Componente chave;

    contadorComparacoes = 0;

    for (i = 1; i < n; i++) {
        chave = v[i]; /* salva o elemento atual */
        j = i - 1;

        /* Desloca elementos com tipo maior que 'chave' para a direita */
        while (j >= 0) {
            contadorComparacoes++;

            if (strcmp(v[j].tipo, chave.tipo) > 0) {
                v[j + 1] = v[j]; /* abre espaço */
                j--;
            } else {
                break; /* posição de inserção encontrada */
            }
        }

        v[j + 1] = chave; /* insere na posição correta */
    }
}

/* ============================================================
 *  selectionSortPrioridade – ordena por prioridade decrescente.
 *  (maior prioridade = componente mais crítico aparece primeiro)
 *
 *  Algoritmo Selection Sort:
 *    A cada passagem, encontra o elemento com maior prioridade
 *    no subvetor ainda não ordenado e o coloca na posição correta.
 *
 *  Parâmetros:
 *    v – vetor de componentes
 *    n – número de elementos
 *
 *  Complexidade: O(n²) em todos os casos
 * ============================================================ */
void selectionSortPrioridade(Componente v[], int n) {
    int        i, j, idxMax;
    Componente temp;

    contadorComparacoes = 0;

    for (i = 0; i < n - 1; i++) {
        idxMax = i; /* assume que o maior está na posição i */

        /* Encontra o maior no subvetor v[i+1..n-1] */
        for (j = i + 1; j < n; j++) {
            contadorComparacoes++;

            if (v[j].prioridade > v[idxMax].prioridade)
                idxMax = j;
        }

        /* Troca o maior encontrado com v[i] */
        if (idxMax != i) {
            temp       = v[i];
            v[i]       = v[idxMax];
            v[idxMax]  = temp;
        }
    }
}

/* ============================================================
 *  medirTempo – executa um algoritmo de ordenação e mede:
 *    1. Número de comparações (via variável global contadorComparacoes)
 *    2. Tempo de CPU gasto (via clock())
 *
 *  Parâmetros:
 *    algoritmo – ponteiro para a função de ordenação
 *    v         – vetor de componentes a ordenar
 *    n         – número de elementos
 *
 *  Observação: clock() retorna ticks de CPU acumulados.
 *  A diferença entre dois valores dividida por CLOCKS_PER_SEC
 *  dá o tempo em segundos. Para vetores pequenos o valor pode
 *  ser 0.000000s; isso é normal — o algoritmo simplesmente
 *  termina antes do próximo tick do sistema operacional.
 * ============================================================ */
void medirTempo(void (*algoritmo)(Componente[], int),
                Componente v[], int n) {
    clock_t inicio, fim;
    double  tempoSegundos;

    contadorComparacoes = 0;      /* zera antes de executar */

    inicio = clock();             /* marca início (ticks de CPU) */
    algoritmo(v, n);              /* executa o algoritmo         */
    fim    = clock();             /* marca fim                   */

    tempoSegundos = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("   📊 Comparações realizadas : %d\n", contadorComparacoes);
    printf("   ⏱️  Tempo de execução      : %.6f segundos\n", tempoSegundos);
}

/* ============================================================
 *  executarOrdenacao – orquestra a escolha do algoritmo,
 *  chama medirTempo e exibe o resultado.
 *
 *  Parâmetro:
 *    criterio – 1 = Bubble/nome, 2 = Insertion/tipo, 3 = Selection/prio
 * ============================================================ */
void executarOrdenacao(int criterio) {
    if (totalComponentes < 2) {
        printf("\n⚠️  São necessários pelo menos 2 componentes para ordenar.\n");
        return;
    }

    switch (criterio) {
        case 1:
            printf("\n── Bubble Sort por Nome ──\n");
            medirTempo(bubbleSortNome, torre, totalComponentes);
            /* Após ordenação por nome, a busca binária fica disponível */
            ordenadoPorNome = 1;
            break;

        case 2:
            printf("\n── Insertion Sort por Tipo ──\n");
            medirTempo(insertionSortTipo, torre, totalComponentes);
            /* Ordenação por tipo invalida a busca binária por nome */
            ordenadoPorNome = 0;
            break;

        case 3:
            printf("\n── Selection Sort por Prioridade ──\n");
            medirTempo(selectionSortPrioridade, torre, totalComponentes);
            /* Ordenação por prioridade invalida a busca binária por nome */
            ordenadoPorNome = 0;
            break;

        default:
            printf("⚠️  Critério inválido!\n");
            return;
    }

    printf("\n✅ Ordenação concluída!\n");
    mostrarComponentes(torre, totalComponentes);
}

/* ============================================================
 *  buscaBinariaPorNome – localiza um componente pelo nome.
 *  PRÉ-REQUISITO: o vetor deve estar ordenado por nome.
 *
 *  Algoritmo:
 *    Mantém índices 'inicio' e 'fim' do segmento de busca.
 *    Calcula o meio a cada iteração e compara com o alvo.
 *    Descarta metade do vetor a cada passo.
 *
 *  Parâmetros:
 *    v    – vetor de componentes (ordenado por nome)
 *    n    – número de elementos
 *    nome – string alvo (componente-chave a localizar)
 *
 *  Retorna: índice do componente encontrado, ou -1 se ausente.
 *
 *  Complexidade: O(log n)
 * ============================================================ */
int buscaBinariaPorNome(Componente v[], int n, char nome[]) {
    int inicio = 0;
    int fim    = n - 1;
    int meio;
    int cmp;
    int comparacoes = 0;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        comparacoes++;

        cmp = strcmp(v[meio].nome, nome);

        if (cmp == 0) {
            /* Componente-chave encontrado! Exibe as comparações usadas */
            printf("   📊 Comparações na busca binária: %d\n", comparacoes);
            return meio;
        } else if (cmp < 0) {
            inicio = meio + 1; /* busca na metade direita */
        } else {
            fim    = meio - 1; /* busca na metade esquerda */
        }
    }

    printf("   📊 Comparações na busca binária: %d\n", comparacoes);
    return -1; /* componente não encontrado */
}

/* ============================================================
 *  executarBuscaBinaria – interface para a busca binária.
 *  Solicita o nome do componente-chave e exibe o resultado,
 *  confirmando visualmente sua presença na torre.
 * ============================================================ */
void executarBuscaBinaria(void) {
    char  nomeBusca[TAM_NOME];
    int   indice;

    if (totalComponentes == 0) {
        printf("\n⚠️  Nenhum componente cadastrado!\n");
        return;
    }

    /* Valida o pré-requisito da busca binária */
    if (!ordenadoPorNome) {
        printf("\n⚠️  A busca binária exige ordenação prévia por nome.\n");
        printf("   Use a opção 3 (Bubble Sort por nome) primeiro.\n");
        return;
    }

    printf("\n── Busca Binária – Componente-Chave ──\n");
    printf("Nome do componente-chave: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0'; /* remove '\n' */

    indice = buscaBinariaPorNome(torre, totalComponentes, nomeBusca);

    if (indice != -1) {
        /* ── Confirmação visual: componente-chave encontrado ── */
        printf("\n🔑 COMPONENTE-CHAVE LOCALIZADO! Torre pode ser ativada!\n");
        printf("┌──────────────────────────────────────────┐\n");
        printf("│  🔩 Nome      : %-24s│\n", torre[indice].nome);
        printf("│  🏷️  Tipo      : %-24s│\n", torre[indice].tipo);
        printf("│  ⭐ Prioridade: %-24d│\n", torre[indice].prioridade);
        printf("│  📍 Posição   : %-24d│\n", indice);
        printf("└──────────────────────────────────────────┘\n");
        printf("✅ Componente-chave confirmado! Iniciando montagem final...\n");
    } else {
        printf("\n❌ Componente-chave '%s' não encontrado.\n", nomeBusca);
        printf("   A torre não pode ser ativada sem este componente!\n");
    }
}

/* ============================================================
 *  FIM DO PROGRAMA
 * ============================================================ */