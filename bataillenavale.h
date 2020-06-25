#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//#define cle 12305


/**
 * Structure qui gère le joueur et son terrain
 */
struct Joueur {
    //La ou on place les bateaux, contient des entier : 0 une case vide, > 0 la taille du bateau couvrant la case,
    // < 0 un bateau toucher
    int map[10][10];
    // La nom du joueur
    char nom[255];
    // Les bateaux a placer. Le tableau contient le nombre d'exemplaire de chaque bateau
    int bateaux[4];
};

struct Game {
    struct Joueur j1;
    struct Joueur j2;
};


/**
 * Fonction qui affiche la grille du joueur passé en paramètre
 * @param joueur le joueur
 */
char *afficherGrille(struct Joueur *joueur) {
    char *grille;
    grille = malloc(sizeof(char) * 250);
    strcat(grille, joueur->nom);
    strcat(grille, "\n\n   1 2 3 4 5 6 7 8 9 10");
    int i;
    int j;
    for (i = 0; i < 10; ++i) {
        strcat(grille, "\n");
        if (i + 1 < 10)
            sprintf(grille, "%s %d", grille, (i + 1));
        else
            sprintf(grille, "%s%d", grille, (i + 1));
        strcat(grille, "|");
        for (j = 0; j < 10; ++j) {
            if (joueur->map[i][j] == 0)
                strcat(grille, "~|");
            else if (joueur->map[i][j] > 0) {
                strcat(grille, "■|");
            } else {
                strcat(grille, "X|");
            }
        }
    }
    strcat(grille, "\n");
    return grille;
}

void envoyerMessage(char *message, int sock) {
    char *msg;
    char *toSend;
    toSend = malloc(sizeof(char) * 1024);
    msg = malloc(sizeof(char) * 1024);

    toSend[0] = '\0';
    scanf("%[^\n]%*c", msg);
    strcat(toSend, msg);

    while (write(sock, toSend, strlen(toSend)) == -1) {
        //printf("Can't write\n");
        //break;
    }
}

void envoyerGrille(struct Joueur *joueur, int socket) {
    char *message = malloc(sizeof(char) * 1024);
    char *valeur = malloc(sizeof(char) * 2);
    fflush(stdin);
    sprintf(message, "\n");
    int i;
    int j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            sprintf(valeur, "%d", joueur->map[i][j]);
            strcat(message, valeur);
        }
        strcat(message, "\n");
    }

    envoyerMessage(message, socket);
}

/**
 * Fonction qui permet de placer les bateau
 * @param joueur
 * @param size
 * @param col
 * @param ligne
 * @param direction
 * @return 0 si réussis -1 sinon
 */
int placerBateau(struct Joueur *joueur, int size, int col, int ligne, char *direction) {
    int i;
    if (col > 9 || col < 0 || ligne > 9 || ligne < 0)
        return -1;
    //direction = (char *)strupr(direction);
    if (strcmp(direction, "H") == 0) {
        if (col + size > 9)
            return -1;
        for (i = 0; i < size; ++i) {
            joueur->map[ligne][col + i] = size;
        }
    } else if (strcmp(direction, "V") == 0) {
        if (ligne + size > 9)
            return -1;
        for (i = 0; i < size; ++i) {
            joueur->map[ligne + i][col] = size;
        }
    } else
        return -1;
    return 0;
}

/**
 * Fonction qui demande l'emplacement ou placer un bateau
 * @param joueur
 */
void demanderPlacement(struct Joueur *joueur) {
    int ligne = 0;
    int colonne = 0;
    int i;
    int j;
    char direction[20];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < joueur->bateaux[i]; j++) {
            printf("%s", afficherGrille(joueur));

            printf("Placez votre bateau de largeur %d : \nLigne : \n", i + 2);
            scanf("%d", &ligne);
            printf("Colonne : \n");
            scanf("%d", &colonne);
            printf("Orientation : (H/V)\n");
            fflush(stdin);
            fflush(stdout);
            scanf("%s", direction);
            printf("Placement en %d %d %s bateau de taille %d\n", ligne, colonne, direction, (i + 2));
            if (placerBateau(joueur, i + 2, colonne - 1, ligne - 1, direction) != 0) {
                printf("ERREUR DE SAISIE\n");
                j--;
            }

        }
    }
}

void initGrille(struct Joueur *joueur) {

    int i;
    int j;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            joueur->map[i][j] = 0;
        }
    }
}

/**
 * Fonction qui initialise un joueur
 * @param joueur la structure a initialiser
 * @param nom le nom du joueur
 */
void init(struct Joueur *joueur, char nom[255]) {
    printf("Init grille J1\n");
    initGrille(joueur);

    printf("Init name J1\n");
    strcpy(joueur->nom, nom);
    printf("Init boats J1\n");
    joueur->bateaux[0] = 1;
    joueur->bateaux[1] = 2;
    joueur->bateaux[2] = 1;
    joueur->bateaux[3] = 1;

    demanderPlacement(joueur);
}


void initGame(struct Game *game) {
    printf("INIT J1\n");
    init(&game->j1, "J1");
    printf("INIT J2\n");
    initGrille(&game->j2);
}
