/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/24 14:27:56 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/settings.hpp"
#include "includes/Config.hpp"


int main( void )
{
	// Config			config;
	settings		server;
	int				socket_server;
	int				timeout = 0;
	struct kevent	srv;
	

	int ke = kqueue();
	int fd = open("srcs/site/test.html", O_RDWR);
	try
	{
		socket_server = server.build();
		EV_SET(&srv, socket_server, EVFILT_READ | EVFILT_WRITE , EV_ADD | EV_ENABLE, 0, 0, NULL);
		if (socket_server == -1 || ke == -1 || listen(socket_server, 10) == -1 || kevent(ke, &srv, 1, NULL, 0, NULL) == -1)
			throw settings::badCreation();
		char test[550] = {0};
		read(fd, &test, 550);
		while(1)
		{
			
			char buffer[1024] = {0};
			struct kevent event;
			
			int nevents = kevent(ke, NULL, 0, &event, 1, NULL);
			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
			if (nevents == -1)
				printf("KO\n");
			else if (nevents > 0)
			{
				int socket_client = accept(socket_server, (struct sockaddr *)&server.interface, (socklen_t *)&server.interface);
				read(socket_client, buffer, 1024);
				
				printf("%s\n", buffer);
				close(socket_client);
				write(socket_client, test, 550);
				printf("------------------Hello message sent-------------------\n");
				EV_SET(&event, socket_client, EVFILT_WRITE , EV_ADD | EV_ENABLE, 0, 0, &timeout);
				if (kevent(ke, &event, 1, NULL, 0, NULL) == -1){}
			}
		}
	}
	catch(const std::exception& e)
	{
		close(fd);
		close(ke);
		std::cerr << e.what() << '\n';
	}
}


/* PLAN

Si GET:
	recuperer le lien vers la ressource voulu
	verifier son existence
	recuperer son contenu
	Si Cgi:
		execve et dup vert fichier tmp
	generer les headers
	assembler headers et contenu
	si la taille du contenu > 2 083 caractères split
	envoyer le ou les paquets
Si POST:
	recuperer la cible
	verifier l'existence du dossier
	upload avec le nom donne
Si DELETE:
	recuperer la cible
	verifier l'existence du fichier
	supprimer la cible




*/