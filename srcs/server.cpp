/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/15 14:47:40 by lomasson         ###   ########.fr       */
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
		std::string 	sbuffer;
		while (1)
		{
			std::string 	reponse_request;
			

			// printf("------------------ Waiting new connection-------------------\n");
			int nevents = kevent(ke, NULL, 0, event, 1024, NULL);
			if (nevents > 0)
			{
				for(int i = 0; i < nevents; i++)
				{
					if (std::find(clients.begin(), clients.end(), event[i].ident) == clients.end())
					{
						int socket_client = accept(event[0].ident, (struct sockaddr *)event[0].udata, (socklen_t *)event[0].udata);
						clients.push_back(socket_client);
						event[i].flags = 1;
						struct kevent changeEvent;
						fcntl(socket_client, F_SETFL, fcntl(socket_client, F_GETFL, 0) | O_NONBLOCK);
						EV_SET(&changeEvent, socket_client, -1, EV_ADD, 0, 0, nullptr);
						if (kevent(ke, &changeEvent, 1, nullptr, 0, nullptr) == -1)
						{
						  std::cerr << "Could not add client socket to kqueue" << std::endl;
						}
					}
					if (event[i].flags & 1)
					{
						sbuffer = server.reading(clients[i], req);
						if (!sbuffer.empty())
							event[i].flags = 2;
						else
						{
							struct kevent changeEvent;
							EV_SET(&changeEvent, clients[i], 0, EV_DELETE, 0, 0, nullptr);
							kevent(ke, &changeEvent, 1, 0, 0, 0);
							close(clients[i]);
							clients.erase(clients.begin() + i);
						}
					}
					if (event[i].flags & 2)
					{
						server.writing(clients[i], req, sbuffer);
						req.reset();
						event[i].flags = 1;
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