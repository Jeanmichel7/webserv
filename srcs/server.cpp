/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/03/10 02:56:04 by lomasson         ###   ########.fr       */
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
	Settings	server;
	if (server.checkArgs(argc) == 1)
		return (1);
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

	struct kevent				event[MAX_REQUESTS];
	Request						req;
	std::map<int, sockaddr_in>	clients;
	int							ke = kqueue();
	try
	{
		if (ke == -1)
			throw Settings::badCreation();
		server.build(ke);
		std::map<int, Sbuffer>	sbuffer;
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
						close(event[i].ident);
						std::cout << "CLOSE\n";
					}
					else
					{
						if (clients.find(event[i].ident) == clients.end())
						{
							std::cout << "\nACCEPT\n";
							struct sockaddr_in client_addr;
							socklen_t client_addr_len = sizeof(client_addr);
							int socket_client = accept(event[i].ident, (struct sockaddr *)&client_addr, &client_addr_len);
							// int r = 1;
							// if (setsockopt(socket_client, SOL_SOCKET, SO_REUSEADDR, &r, sizeof(r)) < 0)
							// 	std::cout << "nope\n";
							clients[socket_client] = client_addr;
							server.set_event(ke, socket_client, EVFILT_READ, EV_ADD | EV_ENABLE);
							fcntl(socket_client, F_SETFL, O_NONBLOCK);
						}
						else if (event[i].filter == EVFILT_READ)
							server.reading_request(sbuffer[event[i].ident], server, ke, event[i].ident, req);
						else if (event[i].filter == EVFILT_WRITE)
						{
								
								// std::cout << sbuffer[event[i].ident]._buffer.size() << std::endl;
								if (sbuffer[event[i].ident].status_code == 413)
									server.gestion_413(sbuffer[event[i].ident], event[i].ident);
								if (!sbuffer[event[i].ident]._request_parsed)
									server.parseRequest(sbuffer[event[i].ident]);
								if (!sbuffer[event[i].ident]._body_ready)
									server.generate_body(sbuffer[event[i].ident], clients[event[i].ident]);
								if (!sbuffer[event[i].ident]._header_ready)
									server.generate_header(sbuffer[event[i].ident]);
								if (!sbuffer[event[i].ident]._header_sent && !sbuffer[event[i].ident]._response_sent)
									server.writeResponse(sbuffer[event[i].ident], event[i].ident);
								if (sbuffer[event[i].ident]._add_eof || (sbuffer[event[i].ident]._header_sent && sbuffer[event[i].ident]._response_sent))
								{
									sbuffer.erase(event[i].ident);
									std::cout << "CLOSE DE FD\n";
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
									clients.erase(event[i].ident);
									close(event[i].ident);
									continue ;
								}
								else
								{
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE);
									std::memset(&sbuffer[event[i].ident], 0, sizeof(sbuffer[event[i].ident]));
									sbuffer.erase(event[i].ident);
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

