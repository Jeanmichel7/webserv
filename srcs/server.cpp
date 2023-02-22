/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/21 19:40:02 by ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int main(int argc, char **argv)
{
	Settings server;
	if (argc < 2)
	{
		std::cout << RED << "WebServ$> Bad argument: please enter the path of the configuration file." << DEF << std::endl;
		return (1);
	}
	if (argc > 3)
	{
		std::cout << RED << "WebServ$> Bad argument: please enter only the path of the configuration file." << DEF << std::endl;
		return (1);
	}
	try
	{
		server.config = Config(argv[1]);
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
	struct kevent 	event[1024];
	Request 		req;
	std::vector<int>	clients;
	int ke = kqueue();
	try
	{
		if (ke == -1)
			throw Settings::badCreation();
		server.build(ke);
		// build socket and configure the ke structure. 
		// CHANGEMENT | RAJOUT PAR YANN 
		--(server.config);
		while (1)
		{
			// printf("------------------ Waiting new connection-------------------\n");
			int nevents = kevent(ke, NULL, 0, event, 1024, NULL);
			// changeList is a buffer of struct event and is filled with events detected by kevent, if an event occurs in a file descriptor the kernel create a structure kevent and put it in the buffer. 
			if (nevents > 0)
			{
				for(int i = 0; i < nevents; i++)
				{
					std::string 	reponse_request;
					std::string 	sbuffer;
					if (event[i].flags & EV_EOF)
					{
						// If a EOF event is detected, it means that the client has closed the connection.
						struct kevent changeEvent;
						EV_SET(&changeEvent, clients[i], 0, EV_DELETE, 0, 0, nullptr);
						// We set the struct changeEvent for delete the event in the kqueue when passe the struct to kevent system function .
						// if no eventlist are passed to kevent, kevent will wait and keep events for the next call with eventlist.
						// we need to retriev regularly the events in the kqueue for avoid overflow event in the kqueue.
						kevent(ke, &changeEvent, 1, 0, 0, 0);
						// kevent use flag to know what to do with the events.
						// we pass the struct changeEvent to kevent system function for delete the event in the kqueue.
						close(clients[i]);
						// we close the socket fd of the client.
						clients.erase(clients.begin() + i);
						// we erase the socket fd of the client in the vector clients.
					}
					else
					{
						if (std::find(clients.begin(), clients.end(), event[i].ident) == clients.end())
						{
							// si event[i].ident n'est pas dans le vector clients c'est que c'est forcement un nouveau client
							// donc on cree un nouveau socket fd a partir de event[i].ident car si ce n'est pas un clien c'est forcement un serveur.
							int socket_client = accept(event[0].ident, (struct sockaddr *)event[0].udata, (socklen_t *)event[0].udata);
							// accept the connection of the client and return the socket fd of the client in fuction of wich server is consern.
							clients.push_back(socket_client);
							server.set_event(ke, socket_client, EVFILT_READ, EV_ADD | EV_ENABLE);
							// creer et ajout un nouveau event pour le nouveau client .
							event[i].filter = EVFILT_READ;
							// A QUOI CA SERT ? 
						}
						if (event[i].filter == EVFILT_READ)
						{
							sbuffer = server.reading(clients[i], req);
							// on lit le contenue du buffer 	
							if (!sbuffer.empty())
							{
								event[i].filter = EVFILT_WRITE;
								// POURQUOI clients[i] et pas event[i].ident ?
								server.set_event(ke, clients[i], EVFILT_READ, EV_DELETE);
								// On supprime l'event de lecture du client car on a fini de lire le contenue du buffer.
								server.set_event(ke, clients[i], EVFILT_WRITE, EV_ADD | EV_ENABLE);
								// On ajoute un nouvel event d'ecriture pour le client car on a fini de lire le contenue du buffer.
							}
							// {
							// 	struct kevent changeEvent;
							// 	EV_SET(&changeEvent, clients[i], 0, EV_DELETE, 0, 0, nullptr);
							// 	kevent(ke, &changeEvent, 1, 0, 0, 0);
							// 	close(clients[i]);
							// 	clients.erase(clients.begin() + i);
							// }
						}
						if (event[i].filter == EVFILT_WRITE)
						{
							server.writing(clients[i], req, sbuffer);
							req.reset();
							server.set_event(ke, clients[i], EVFILT_WRITE, EV_DELETE);
              				server.set_event(ke, clients[i], EVFILT_READ, EV_ADD | EV_ENABLE);
						}
					}
				}
			}
		}
	}
	catch (const std::exception &e)
	{
		std::cout << strerror(errno);
		std::cerr << std::endl
				  << e.what() << std::endl;
	}
}
