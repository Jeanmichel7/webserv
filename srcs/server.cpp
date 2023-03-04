/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/03/04 21:40:19 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

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
	if (server.checkArgs(argc, argv) == 1)
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
							char header_buffer[4097];
							memset(buffer, 0, 4097);
							memset(header_buffer, 0, 4097);
							size_t readed = 0;
							size_t header_readed = 0;

							if (reqIsChuncked(header_buffer) == false) {
								header_readed = server.reading_header(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start, header_buffer);
								for (unsigned long j = 0; j <  sbuffer[event[i].ident].readed; j++)
									sbuffer[event[i].ident].buffer.push_back(header_buffer[j]);

							}
							if (reqIsChuncked(header_buffer) == true)
								sbuffer[event[i].ident].is_chunked = true;
							// cout << "header_buffer : " << header_buffer << endl;
							// cout << "req is chunked : " << sbuffer[event[i].ident].is_chunked << endl;
							if (sbuffer[event[i].ident].is_chunked == true)
							{
								// cout << "On est dans les chuncks" << endl;
								
								string::size_type pos;
								char *useless = NULL;
								char *chunck_buffer = NULL;
								// bool is_last_chunck = false;
								sbuffer[event[i].ident].readed = 0;
								chunck_buffer = server.reading_chunck(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start, useless);
								cout << "chunck_buffer : '" << chunck_buffer << "'" << endl;
								
								if (strlen(chunck_buffer) == 0) {
									cout << "FINISHED chunck request" << endl;
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
									sbuffer[event[i].ident].buffer.push_back('\r');
									sbuffer[event[i].ident].buffer.push_back('\n');
									sbuffer[event[i].ident].is_chunked = false;
								}
								else {
									for (unsigned long j = 0; j < sbuffer[event[i].ident].readed; j++)
										sbuffer[event[i].ident].buffer.push_back(chunck_buffer[j]);
								}

								// display buffer total
								std::vector<char>::const_iterator start = sbuffer[event[i].ident].buffer.begin();
								std::vector<char>::const_iterator end = sbuffer[event[i].ident].buffer.end();
								cout << "BUFFER: '" << endl;
								for (; start != end; start++)
									std::cout << *start;
								cout << "'" << endl;
							}
							else
							{
								// check si il y a un body
								std::string header(header_buffer);
								std::string::size_type pos = header.find("Content-Length");
								if (pos == std::string::npos)
								{
									cout << "FINISHED" << endl;
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
									break;
								}
								
								readed = server.reading(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start, buffer);
								if (readed == 18446744073709551615)
								{
									cout << "ERROR: readed == 18446744073709551615" << endl;
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
									break;
								}
								cout << "readed : " << readed << endl;
								cout << "read[]sdfdsf" << sbuffer[event[i].ident].readed << endl;
								cout << "buffer avant : " << buffer << endl;

								// sbuffer[event[i].ident].buffer.insert(sbuffer[event[i].ident].buffer.end(), readed, *buffer );
								for (unsigned long j = 0; j < sbuffer[event[i].ident].readed; j++)
									sbuffer[event[i].ident].buffer.push_back(buffer[j]);

								if (req.isFinishedRequest(sbuffer[event[i].ident].buffer, sbuffer[event[i].ident].readed))
								{
									cout << "FINISHED" << endl;
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
								}
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
	catch (const std::exception &e)
	{
		std::cout << strerror(errno);
		std::cerr << std::endl
							<< e.what() << std::endl;
	}
}
