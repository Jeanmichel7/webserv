/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/23 12:40:24 by ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


bool reqIsChuncked(std::string buffer) {
	string str(buffer);
	str.erase(str.find("\r\n") + 2);
	cout << "que des hexq : " << str << endl;

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
	std::vector<int> clients;
	int ke = kqueue();
	try
	{
		if (ke == -1)
			throw Settings::badCreation();
		server.build(ke);
		std::string sbuffer[1024];
		while (1)
		{
			// printf("------------------ Waiting new connection-------------------\n");
			int nevents = kevent(ke, NULL, 0, event, 1024, NULL);
			if (nevents > 0)
			{
				for (int i = 0; i < nevents; i++)
				{
					std::string reponse_request;

					if (event[i].flags & EV_EOF)
					{
						server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
						close(event[i].ident);
						clients.erase(clients.begin() + i);
					}
					else
					{
						if (std::find(clients.begin(), clients.end(), event[i].ident) == clients.end())
						{
							int socket_client = accept(event[i].ident, (struct sockaddr *)event[i].udata, (socklen_t *)event[i].udata);
							clients.push_back(socket_client);
							event[i].filter = 10;
							server.set_event(ke, socket_client, EVFILT_READ, EV_ADD | EV_ENABLE);
						}
						if (event[i].filter == EVFILT_READ)
						{
							std::string buffer;
							// cout << "is chunck : " << req.header.is_chuncked << endl;
							// focntion qui check si c'est chunk
							buffer = server.reading(event[i].ident, req);
							if (!reqIsChuncked(buffer))
							{
								if (buffer == "0\r\n\r\n")
								{
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
								} 
								else {
									string str(buffer);
									str.erase(0, str.find("\r\n") + 2);
									sbuffer[event[i].ident] += str;
									cout << sbuffer[event[i].ident] << endl;
								}

							}
							else
							{
								// req.splitRequest(buffer);
								// req.header.parseHeader();
								server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
								server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
								sbuffer[event[i].ident] += buffer;
							}

						}

						// que si la requete est en entier
						// if (!sbuffer[event[i].ident].empty())
						// {
						// 	server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
						// 	server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
						// }
					}
					if (event[i].filter == EVFILT_WRITE)
					{
						server.writing(event[i].ident, req, sbuffer[event[i].ident]);
						req.reset();
						sbuffer[event[i].ident] = "";
						server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
						server.set_event(ke, event[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE);
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
