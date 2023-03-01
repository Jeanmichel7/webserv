/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/03/01 10:38:50 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


bool reqIsChuncked(std::string buffer) {
	string str(buffer);
	str.erase(str.find("\r\n") + 2);

	for(size_t i = 0; i < str.length(); i++) {
		if (!isxdigit(str[i]))
			return 1;
	}
	return 0;
}

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
	struct kevent event[1024];
	Request req;
	std::map<int, sockaddr_in> clients;
	int ke = kqueue();
	try
	{
		if (ke == -1)
			throw Settings::badCreation();
		server.build(ke);
		std::string sbuffer[1024];
		while (1)
		{
			int nevents = kevent(ke, NULL, 0, event, 1024, NULL);
			if (nevents > 0)
			{
				for (int i = 0; i < nevents; i++)
				{
					std::string reponse_request;

					if (event[i].flags & EV_EOF)
					{
						server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
						clients.erase(event[i].ident);
						close(event[i].ident);
						std::cout << std::endl << "Client disconnected \n" << std::endl;
						std::cout << "CLOSE\n";
					}
					else
					{
						if (clients.find(event[i].ident) == clients.end())
						{
							std::cout << std::endl << "New client connected \n" << std::endl;
							struct sockaddr_in client_addr;
							socklen_t client_addr_len = sizeof(client_addr);
							std::cout << "ACCEPT\n";
							int socket_client = accept(event[i].ident, (struct sockaddr *)&client_addr, &client_addr_len);
							clients[socket_client] = client_addr;
							server.set_event(ke, socket_client, EVFILT_READ, EV_ADD | EV_ENABLE);
						}
						else if (event[i].filter == EVFILT_READ)
						{
							std::string buffer;
							buffer = server.reading(event[i].ident);
							if (!reqIsChuncked(buffer))
							{
								if (buffer == "0\r\n\r\n")
								{
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
								} 
								else
									buffer.erase(0, buffer.find("\r\n") + 2);
							}
							sbuffer[event[i].ident] += buffer;
							// cout << "BUFFER: '" << sbuffer[event[i].ident] << "'" << endl;
							// if (yd::ends_with_rn(sbuffer[event[i].ident]))
							if (req.isFinishedRequest(sbuffer[event[i].ident]))
							{
								server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
								server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
							}
						}
					}
					if (event[i].filter == EVFILT_WRITE)
					{
						server.writing(event[i].ident, sbuffer[event[i].ident], clients[event[i].ident]);
						sbuffer[event[i].ident] = "";
						server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
						server.set_event(ke, event[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE);
					}
				}
			}
		}
	}
	catch (const std::exception &e) {
		std::cout << strerror(errno);
		std::cerr << std::endl
				<< e.what() << std::endl;
	}
}
