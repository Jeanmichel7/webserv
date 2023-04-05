/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/04/05 14:20:55 by ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Settings.hpp"

bool reqIsChuncked(std::string req)
{
	std::string::size_type pos = req.find("Transfer-Encoding: chunked");
	if (pos != std::string::npos)
		return (true);
	return (false);
}

int main(int argc, char **argv)
{
	Settings server;
	if (server.checkArgs(argc) == 1)
		return (1);
	try
	{
		if (argc == 1)
			server.config = Config("config/default.conf");
		else if (argv[1] != NULL)
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
		std::map<int, Sbuffer> sbuffer;
		std::cout << "\e[0;32m\e[1mWebServ$> Ready to accept request.\e[0m" << std::endl;
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
						sbuffer.erase(event[i].ident);
						std::cout << event[i].ident << ": Close fd" << std::endl;
						close(event[i].ident);
					}
					else
					{
						if (clients.find(event[i].ident) == clients.end())
						{
							struct sockaddr_in client_addr;
							socklen_t client_addr_len = sizeof(client_addr);
							int socket_client = accept(event[i].ident, (struct sockaddr *)&client_addr, &client_addr_len);
							int option = 1;
							if (setsockopt(socket_client, SOL_SOCKET, SO_NOSIGPIPE, &option, sizeof(option)) < 0)
							{
								std::cout << "Erreur lors de la configuration de SO_NOSIGPIPE" << std::endl;
							}
							std::cout << socket_client << ": Accept Connexion " << std::endl;
							// int r = 1;
							// if (setsockopt(socket_client, SOL_SOCKET, SO_REUSEADDR, &r, sizeof(r)) < 0)
							// 	std::cout << "nope\n";
							clients[socket_client] = client_addr;
							server.set_event(ke, socket_client, EVFILT_READ, EV_ADD | EV_ENABLE);
							fcntl(socket_client, F_SETFL, O_NONBLOCK);
						}
						else if (event[i].filter == EVFILT_READ)
						{
							std::cout << event[i].ident << ": reading request " << std::endl;
							sbuffer[event[i].ident]._status = REQUEST_BEING_RECEIVED;
							server.reading_request(sbuffer[event[i].ident], server, ke, event[i].ident, req);
						}
						else if (event[i].filter == EVFILT_WRITE)
						{
							if (sbuffer[event[i].ident].status_code == 413)
							{
								std::cout << event[i].ident << ": 413" << std::endl;
								server.gestion_413(sbuffer[event[i].ident], event[i].ident);
							}
							if (sbuffer[event[i].ident]._status == REQUEST_RECEIVED)
							{
								std::cout << event[i].ident << ": parse request " << std::endl;
								if (server.parseRequest(sbuffer[event[i].ident]))
									return 1;
							}
							if (sbuffer[event[i].ident]._status == REQUEST_PARSED || sbuffer[event[i].ident]._status == CGI_PROCESS_LAUNCHED)
							{
								std::cout << event[i].ident << ": Generate body " << std::endl;
								server.generate_body(sbuffer[event[i].ident], clients[event[i].ident]);
							}
							if (sbuffer[event[i].ident]._status == BODY_GENERATED)
							{
								std::cout << event[i].ident << ": Generate Header " << std::endl;
								server.generate_header(sbuffer[event[i].ident]);
							}
							if (sbuffer[event[i].ident]._status == HEADER_GENERATED || sbuffer[event[i].ident]._status == HEADER_SENT)
							{
								std::cout << event[i].ident << ": writeResponse" << std::endl;
								server.writeResponse(sbuffer[event[i].ident], event[i].ident);
							}
							if ((sbuffer[event[i].ident]._add_eof && sbuffer[event[i].ident]._status == BODY_SENT) || (sbuffer[event[i].ident]._status == SOCKET_ERROR))
							{
								std::cout << event[i].ident << ": close FD" << std::endl;
								sbuffer[event[i].ident].clean();
								// sbuffer.erase(event[i].ident);
								server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
								clients.erase(event[i].ident);
								close(event[i].ident);
								server.check_timeout(sbuffer, ke, clients);
								continue;
							}
							else if (sbuffer[event[i].ident]._status == BODY_SENT)
							{
								std::cout << event[i].ident << ": clear client, keep connexion open" << std::endl;
								server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
								server.set_event(ke, event[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE);
								// std::memset(&sbuffer[event[i].ident], 0, sizeof(sbuffer[event[i].ident]));
								sbuffer[event[i].ident].clean();
								server.check_timeout(sbuffer, ke, clients);
								// sbuffer.erase(event[i].ident);
							}
						}
					}
				}
			}
			else
				server.check_timeout(sbuffer, ke, clients);
		}
	}
	catch (const std::exception &e)
	{
		std::cout << strerror(errno);
		std::cerr << std::endl
							<< e.what() << std::endl;
	}
}
