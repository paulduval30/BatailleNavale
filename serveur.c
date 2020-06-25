/* Processus A */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#define cle 12300

int msqid;
struct msgbuf_EXD {
    long mtype;
    char mtext[1024];
} msgp;

int main() {
    /* Réouvre la file de message */
    char message[20];
    while(strcmp(message, "/q") != 0)
    {
        msqid = msgget(cle, 0);
        /* Lit le message */
        msgrcv(msqid, &msgp, 19, 12, 0);

        if(strcmp(msgp.mtext, "") == 0)
            continue;
        else{
            strcpy(message, msgp.mtext);
            strcpy(msgp.mtext, "");
        }
        /* Détruit la MSQ */
        msgctl(msqid, IPC_RMID, NULL);
        /* Affiche le message */
        printf("Message : \"%s\"\n", message);
    }
    return (0);
}

