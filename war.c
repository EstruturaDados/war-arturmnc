#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define QT_TERRITORIOS 5

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_NOME];
    int tropas;
} Territorio;

Territorio* inicializarTerritorios();
void cadastrarTerritorios(Territorio *t);
void exibirMapa(const Territorio *t);
void atacar(Territorio *t);
int simularDado();
void conquistar(Territorio *atacante, Territorio *defensor);
int gerarMissao();
void verificarMissao(const Territorio *t, int missao);

int main() {
    srand(time(NULL));

    Territorio *mapa = inicializarTerritorios();
    int missao = gerarMissao();
    int opcao;

    printf("\n=== DESAFIO WAR ===\n");
    printf("Sua missão é: ");

    if (missao == 1)
        printf("Destruir o exército VERDE\n");
    else
        printf("Conquistar 3 territórios\n");

    do {
        printf("\n=== MENU ===\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missão\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                atacar(mapa);
                break;
            case 2:
                verificarMissao(mapa, missao);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    free(mapa);
    return 0;
}

Territorio* inicializarTerritorios() {
    Territorio *t = calloc(QT_TERRITORIOS, sizeof(Territorio));

    printf("\n=== Cadastro dos Territórios ===\n");
    for (int i = 0; i < QT_TERRITORIOS; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", t[i].nome);

        printf("Cor do exército: ");
        scanf(" %[^\n]", t[i].cor);

        printf("Número de tropas: ");
        scanf("%d", &t[i].tropas);
    }

    return t;
}

void exibirMapa(const Territorio *t) {
    printf("\n=== MAPA ATUAL ===\n");
    for (int i = 0; i < QT_TERRITORIOS; i++) {
        printf("%d) %-10s | Cor: %-10s | Tropas: %d\n",
               i + 1, t[i].nome, t[i].cor, t[i].tropas);
    }
}

void atacar(Territorio *t) {
    int a, d;

    exibirMapa(t);

    printf("\nEscolha o território ATACANTE (1-5): ");
    scanf("%d", &a);
    printf("Escolha o território DEFENSOR (1-5): ");
    scanf("%d", &d);

    a--; d--;

    if (a == d || a < 0 || d < 0 || a >= QT_TERRITORIOS || d >= QT_TERRITORIOS) {
        printf("Ataque inválido.\n");
        return;
    }

    if (t[a].tropas <= 1) {
        printf("O atacante não tem tropas suficientes.\n");
        return;
    }

    int dadoA = simularDado();
    int dadoD = simularDado();

    printf("\nDado atacante: %d", dadoA);
    printf("\nDado defensor: %d\n", dadoD);

    if (dadoA >= dadoD) {
        printf("Atacante venceu! Defensor perde 1 tropa.\n");
        t[d].tropas--;

        if (t[d].tropas <= 0) {
            printf("Território conquistado!\n");
            conquistar(&t[a], &t[d]);
        }

    } else {
        printf("Defensor venceu! Atacante perde 1 tropa.\n");
        t[a].tropas--;
    }

    exibirMapa(t);
}

int simularDado() {
    return (rand() % 6) + 1;
}

void conquistar(Territorio *atacante, Territorio *defensor) {
    strcpy(defensor->cor, atacante->cor);
    defensor->tropas = 1;
    atacante->tropas -= 1;
}

int gerarMissao() {
    return (rand() % 2) + 1;
}

void verificarMissao(const Territorio *t, int missao) {
    if (missao == 1) {
        for (int i = 0; i < QT_TERRITORIOS; i++) {
            if (strcasecmp(t[i].cor, "verde") == 0 && t[i].tropas > 0) {
                printf("Missão não concluída.\n");
                return;
            }
        }
        printf("Missão concluída! Exército verde destruído!\n");

    } else {
        int cont = 1;

        for (int i = 1; i < QT_TERRITORIOS; i++) {
            if (strcasecmp(t[i].cor, t[0].cor) == 0)
                cont++;
        }

        if (cont >= 3)
            printf("Missão concluída! 3 territórios dominados.\n");
        else
            printf("Missão não concluída. Domínio atual: %d.\n", cont);
    }
}
