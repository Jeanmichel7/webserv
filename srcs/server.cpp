/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/17 17:19:46 by lomasson         ###   ########.fr       */
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


	int fd, ret, ke, j = 0;
	struct kevent 	event[1024];
	struct kevent 	tevent;
	Request 					req;
	std::vector<int>	clients;
	try
	{
		ke = kqueue();
		if (ke == -1)
			throw Settings::badCreation();
		server.build(ke);
		while (1)
		{
			// printf("------------------ Waiting new connection-------------------\n");
			int nevents = kevent(ke, NULL, 0, event, 1024, NULL);

			// EV_SET(&event,	fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, NOTE_WRITE, 0,	NULL);
			// ret = kevent(ke, &event, 1, NULL, 0, NULL);
			// if (ret == -1)
			// 	throw Settings::badCreation();
			// if (event.flags & EV_ERROR)
			// 	throw Settings::badCreation();
			cout << "nb events: " << nevents << endl;
			if (nevents > 0)
			{
				for(int i = 0; i < nevents; i++)
				{

					std::string 	reponse_request;
					std::string 	sbuffer;
					if (event[i].flags & EV_EOF)
					{
						// cout << "fin de l'event" << endl;
						struct kevent changeEvent;
						EV_SET(&changeEvent, clients[i], 0, EV_DELETE, 0, 0, nullptr);
						kevent(ke, &changeEvent, 1, 0, 0, 0);
						close(clients[i]);
						clients.erase(clients.begin() + i);
					}
					else
					{
						if (std::find(clients.begin(), clients.end(), event[i].ident) == clients.end())
						{
							int socket_client = accept(event[i].ident, (struct sockaddr *)event[i].udata, (socklen_t *)event[i].udata);
							clients.push_back(socket_client);
							server.set_event(ke, socket_client, EVFILT_READ, EV_ADD | EV_ENABLE);
							event[i].filter = EVFILT_READ;
						}
						if (event[i].filter == EVFILT_READ)
						{
							sbuffer = server.reading(event[i].ident, req);
							if (!sbuffer.empty())
							{
								event[i].filter = EVFILT_WRITE;
								// cout << "c'est apres ca " << endl;
								server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
								server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD | EV_ENABLE);
							}
							else {
								cerr << "BUFFER EMPTY\n";
							}
						}
						if (event[i].filter == EVFILT_WRITE)
						{
							// cout << "j'ecris\n" << endl;
							server.writing(event[i].ident, req, sbuffer);
							req.reset();
							server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
			  			server.set_event(ke, event[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE);
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
