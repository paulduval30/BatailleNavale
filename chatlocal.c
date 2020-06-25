#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char ** argv)
{
	char * pseudo = argv[1];
	int port1 = atoi(argv[2]);
	int port2 = atoi(argv[3]);

	int pid = fork();
		if(pid == 0)
		{
			struct sockaddr_in sock_host;
			int sock;
	
			/* ouverture d'une socket */
			sock = socket(AF_INET, SOCK_STREAM, 0);

			/* on cre l'adresse de la machine distante */
			memset(& sock_host, '\0', sizeof(sock_host));
			sock_host.sin_family = AF_INET;
			sock_host.sin_port = htons(port1);
			inet_aton("127.0.0.1", & sock_host.sin_addr);

			/* on demande un connection sur l'adresse distante */
			while(connect(sock, (struct sockaddr *) & sock_host, sizeof(sock_host)) == -1){}

			char * msg;
			char * toSend;
			toSend = malloc(sizeof(char) * 1024);
			msg = malloc(sizeof(char) * 1024);
	
			do {
				toSend[0] = '\0';
				printf("%s>", pseudo);
				scanf("%[^\n]%*c", msg);
				strcat(toSend, pseudo);
				strcat(toSend, " : ");
				strcat(toSend, msg);
				/* on envoi le message sur le serveur */
				if(write(sock, toSend, strlen(toSend)) == -1);
				{
					//printf("Can't write\n");
					//break;
				}
			}while(strcmp(msg, "/q"));
	
			/* on ferme la socket */
			close(sock);
			exit(0);
		}
		else
		{
			struct sockaddr_in my_adr;
			struct sockaddr_in serveur_adr;
			char buf[1234];
			int sz_buf;
			socklen_t sz_sock_serveur;
			int sock, sock_distante;
			/* ouverture d'une socket */
			sock = socket(AF_INET, SOCK_STREAM, 0);

			/* cration d'une adresse locale */
			memset(&my_adr,
			'\0'
			, sizeof(my_adr));

			my_adr.sin_family = AF_INET;
			my_adr.sin_port = htons(port2);
			my_adr.sin_addr.s_addr = INADDR_ANY;
			/* association de l'adresse locale a la socket */
			bind(sock, (struct sockaddr *) & my_adr, sizeof(struct sockaddr_in));
			/* attente des clients */
			listen(sock, 1);
			sz_sock_serveur = sizeof(serveur_adr);
			/* recupere les donnes du client */
		
			sock_distante = accept(sock, (struct sockaddr *) & serveur_adr,& sz_sock_serveur);
			printf("Utilisateur connecté ! ...\n");
			while(1) {

			/* lecture de la chaine */
			if((sz_buf = read(sock_distante, buf, 1234)) == 0)
			{
				printf("Can't Read");
				break;
			}
			printf("%d", sz_buf);
			buf[sz_buf] = '\0';
			printf("%s\n", buf);

			}
			close(sock_distante);
			close(sock);
		}
	return 0;
}
