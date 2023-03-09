/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/03/09 17:11:33 by lomasson         ###   ########.fr       */
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
						std::cout << "CLOSE2\n";
					}
					else
					{
						if (clients.find(event[i].ident) == clients.end())
						{
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
						{
							char buffer[4097];
							char header_buffer[4097];
							memset(buffer, 0, 4097);
							memset(header_buffer, 0, 4097);
							size_t readed = 0;
							size_t header_readed = 0;

							if (!reqIsChuncked(header_buffer) && sbuffer[event[i].ident].is_chunked == false && sbuffer[event[i].ident]._buffer.size() == 0) {
								header_readed = server.reading_header(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start, header_buffer);
								if (req.check_header_buffer(header_buffer, server.config)) {
									sbuffer[event[i].ident].status_code = 413;
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
									continue;
								}
								for (unsigned long j = 0; j <  sbuffer[event[i].ident].readed; j++)
									sbuffer[event[i].ident]._buffer.push_back(header_buffer[j]);
							}
							if (reqIsChuncked(header_buffer) == true)
								sbuffer[event[i].ident].is_chunked = true;
							if (sbuffer[event[i].ident].is_chunked == true)
							{
								char *chunck_buffer = NULL;
								sbuffer[event[i].ident].readed = 0;
								chunck_buffer = server.reading_chunck(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start);
								
								if (strlen(chunck_buffer) == 0) {
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
									sbuffer[event[i].ident]._buffer.push_back('\r');
									sbuffer[event[i].ident]._buffer.push_back('\n');
									sbuffer[event[i].ident]._buffer.push_back('\r');
									sbuffer[event[i].ident]._buffer.push_back('\n');
									sbuffer[event[i].ident].is_chunked = false;
								}
								else
								{
									for (unsigned long j = 0; j < sbuffer[event[i].ident].readed; j++)
										sbuffer[event[i].ident]._buffer.push_back(chunck_buffer[j]);
								}
							}
							else
							{
								// check si il y a un body
								std::string header(header_buffer);
								std::string::size_type pos = header.find("Content-Length");
								if (header.size() > 0 && pos == std::string::npos)
								{
									cout << "FINISHED" << std::endl;
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
									break;
								}
								readed = server.reading(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start, buffer);
								for (unsigned long j = 0; j < readed; j++)
									sbuffer[event[i].ident]._buffer.push_back(buffer[j]);

								if (req.isFinishedRequest(sbuffer[event[i].ident]._buffer, sbuffer[event[i].ident].readed))
								{
									cout << "FINISHED" << std::endl;
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
								}
							}
						}
						else if (event[i].filter == EVFILT_WRITE)
						{
								std::cout << sbuffer[event[i].ident]._buffer.size() << std::endl;
								if (sbuffer[event[i].ident].status_code == 413)
									server.gestion_413(sbuffer[event[i].ident], event[i].ident);
								else if (!sbuffer[event[i].ident]._request_parsed)
									server.parseRequest(sbuffer[event[i].ident]);
								else if (!sbuffer[event[i].ident]._body_ready)
									server.generate_body(sbuffer[event[i].ident], clients[event[i].ident]);
								else if (!sbuffer[event[i].ident]._header_ready)
									server.generate_header(sbuffer[event[i].ident]);
								else if (!sbuffer[event[i].ident]._header_sent || !sbuffer[event[i].ident]._response_sent)
									server.writeResponse(sbuffer[event[i].ident], event[i].ident);
								else if (sbuffer[event[i].ident]._add_eof)
								{
									sbuffer.erase(event[i].ident);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
									clients.erase(event[i].ident);
									close(event[i].ident);
									continue ;
								}
								else
								{
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE);
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

