/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/30 11:49:26 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
// #include "Settings.hpp"
#include "Config.hpp"

int	ft_strlen(char *str)
{
	int i = 0;
	while(str[i])
		i++;
	return (i);
}

int main( void )
{
	Config config;
	try 
	{
	config = Config("srcs/conf.conf");
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	catch (std::exception &e)
	{
		return (0);
	}
	Settings		server;
	int				socket_server;
	int				timeout = 0;
	struct kevent	change;
	struct kevent	event;
	char buffer[8000] = {0};

	memset(&server, 0, sizeof(server));
	config.selectServ();
	std::cout << "file:" <<*config.getFile("/bg") << std::endl;
	int fd = open("http/index.html", O_RDWR);
	int ke = kqueue();
	try
	{
		socket_server = server.build();
		EV_SET(&change, socket_server, EVFILT_READ , EV_ADD | EV_ENABLE, 0, 0, 0);
		if (socket_server == -1 || ke == -1 || listen(socket_server, 2) == -1 || kevent(ke, &change, 1, &event, 1, NULL) == -1)
			throw Settings::badCreation();
		char test[550] = {0};
		read(fd, &test, 550);
		while(1)
		{
			
			std::string reponse_request;
			int nevents = kevent(ke, NULL, 0, &event, 1, NULL);
			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
			if (nevents == -1)
				printf("KO\n");
			else if (nevents > 0)
			{
				int socket_client = accept(socket_server, (struct sockaddr *)&server.interface, (socklen_t *)&server.interface);
				read(socket_client, buffer, 8000);
				if (strncmp(buffer, "GET", 3) == 0)
					reponse_request = server.get(config);
				else if (strncmp(buffer, "POST", 4) == 0)
					reponse_request = server.post(config);
				// else if (strncmp(buffer, "DELETE", 6) == 0)
				// 	reponse_request = server.del(config);
				send(socket_client, reponse_request.c_str(), strlen(reponse_request.c_str()),0);
				std::cout << reponse_request << std::endl;
				printf("------------------Hello message sent-------------------\n");
				EV_SET(&event, socket_client, EVFILT_READ | EVFILT_WRITE , EV_ADD | EV_ENABLE, 0, 0, &timeout);
				if (kevent(ke, &event, 1, NULL, 0, NULL) == -1){}
				close(socket_client);
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