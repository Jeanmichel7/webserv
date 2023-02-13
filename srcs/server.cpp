/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/13 14:55:25 by lomasson         ###   ########.fr       */
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
		while (1)
		{
			std::string 	reponse_request;
			std::string 	sbuffer;

			printf("------------------ Waiting new connection-------------------\n");
			int nevents = kevent(ke, NULL, 0, event, 2, NULL);
			if (nevents > 0)
			{
				for(int i = 0; i < nevents; i++)
				{
					if (std::find(clients.begin(), clients.end(), event[i].ident) == clients.end())
					{
						std::cout << "ACCEPT" << std::endl;
						int socket_client = accept(event[0].ident, (struct sockaddr *)event[0].udata, (socklen_t *)event[0].udata);
						clients.push_back(socket_client);
						// event[i].flags = EVFILT_READ;
						struct kevent changeEvent;
						EV_SET(&changeEvent, socket_client, EVFILT_READ, EV_ADD, 0, 0, nullptr);
						if (kevent(ke, &changeEvent, 1, nullptr, 0, nullptr) == -1)
						{
						  std::cerr << "Could not add client socket to kqueue" << std::endl;
						}
					}
					if (event[i].flags & EVFILT_READ)
					{
						std::cout << "READING" << std::endl;
						sbuffer = server.reading(clients[i], req);
						if (sbuffer.empty())
						{
							close(clients[i]);
							event[i].flags = 0;
							clients.erase(clients.begin() + i);
						}
						else
							event[i].flags = EVFILT_WRITE;
					}
					if (event[i].flags & EVFILT_WRITE)
					{
						std::cout << "WRITING" << std::endl;
						server.writing(clients[i], req, sbuffer);
						event[i].flags = EVFILT_READ;
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