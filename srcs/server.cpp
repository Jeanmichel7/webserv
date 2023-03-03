/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/03/03 11:09:20 by ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


bool reqIsChuncked(std::string buffer) {
	string str(buffer);
	std::string::size_type pos = str.find("\r\n");
	if (pos != std::string::npos)
		str.erase(pos + 2);
	// str.erase(str.find("\r\n") + 2);

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
	struct kevent event[MAX_REQUESTS];
	Request req;
	std::map<int, sockaddr_in> clients;
	int ke = kqueue();
	try
	{
		if (ke == -1)
			throw Settings::badCreation();
		server.build(ke);
		Sbuffer sbuffer[MAX_REQUESTS];
		std::memset(sbuffer, 0, sizeof(Sbuffer) * MAX_REQUESTS);
		while (1)
		{
			int nevents = kevent(ke, NULL, 0, event, MAX_REQUESTS, &server.check_request_timeout);
			if (nevents > 0)
			{
				for (int i = 0; i < nevents; i++)
				{
					std::string reponse_request;

					if (event[i].flags & EV_EOF || event[i].flags & EV_ERROR)
					{
						server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
						clients.erase(event[i].ident);
						close(event[i].ident);
						std::cout << "CLOSE\n";
					}
					else
					{
						if (clients.find(event[i].ident) == clients.end())
						{
							struct sockaddr_in client_addr;
							socklen_t client_addr_len = sizeof(client_addr);
							std::cout << "ACCEPT\n";
							int socket_client = accept(event[i].ident, (struct sockaddr *)&client_addr, &client_addr_len);
							clients[socket_client] = client_addr;
							server.set_event(ke, socket_client, EVFILT_READ, EV_ADD | EV_ENABLE);
						}
						else if (event[i].filter == EVFILT_READ)
						{
							char buffer[4097];
							size_t readed = 0;
							readed = server.reading(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start, buffer);			
							sbuffer[event[i].ident].buffer.insert(sbuffer[event[i].ident].buffer.end(), buffer, buffer + readed);
							if (req.isFinishedRequest(sbuffer[event[i].ident].buffer, sbuffer[event[i].ident].readed))
							{
								std::vector< char >::iterator start = sbuffer[event[i].ident].buffer.begin();
								std::vector< char >::iterator end = sbuffer[event[i].ident].buffer.end();
								cout << "BUFFER: " << endl;
								for (; start != end; start++)
									std::cout << *start;
								cout << endl;

								server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
								server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
							}
						}
						else if (event[i].filter == EVFILT_WRITE)
						{
							if (!server.writing(event[i].ident, sbuffer[event[i].ident].buffer, clients[event[i].ident], sbuffer[event[i].ident].readed))
							{
								sbuffer[event[i].ident].buffer.clear();
								sbuffer[event[i].ident].readed = 0;
								server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
								clients.erase(event[i].ident);
								close(event[i].ident);
								std::cout << "CLOSE\n";
							}
							else
							{
								sbuffer[event[i].ident].buffer.clear();
								sbuffer[event[i].ident].readed = 0;
								server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
								server.set_event(ke, event[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE);
							}
						}
					}
				}
			}
			else
				server.check_timeout(sbuffer, ke);
		}
	}
	catch (const std::exception &e) {
		std::cout << strerror(errno);
		std::cerr << std::endl
				<< e.what() << std::endl;
		
	}
}
