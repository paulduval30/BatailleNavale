/* Processus A */

#include <stdlib.h>

#include "bataillenavale.h"

int main(int argc, char *argv[]) {
    int mem_ID;
    void *ptr_mem_partagee;./

    if ((mem_ID = shmget(cle, sizeof(struct Game), 0444)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((ptr_mem_partagee = shmat(mem_ID, NULL, 0)) == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    struct Joueur joueur;
    if(strcmp(argv[1], "1") == 0)
    {
            joueur = ((struct Game *)ptr_mem_partagee)->j1;
    } else
    {
        joueur = ((struct Game *)ptr_mem_partagee)->j2;
    }

    printf("Affichage de la grille\n");
    printf("%s", afficherGrille(&joueur));

    return (0);
}
