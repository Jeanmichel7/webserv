/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/03/07 00:49:02 by jrasser          ###   ########.fr       */
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
						std::cout << "CLOSE2\n";
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

							if (reqIsChuncked(header_buffer) == false && sbuffer[event[i].ident].is_chunked == false && sbuffer[event[i].ident].buffer.size() == 0) {
								header_readed = server.reading_header(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start, header_buffer);
								if (req.check_header_buffer(header_buffer, server.config)) {
									sbuffer[event[i].ident].is_413 = true;
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
									continue;
								}
								for (unsigned long j = 0; j <  sbuffer[event[i].ident].readed; j++)
									sbuffer[event[i].ident].buffer.push_back(header_buffer[j]);
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
									sbuffer[event[i].ident].buffer.push_back('\r');
									sbuffer[event[i].ident].buffer.push_back('\n');
									sbuffer[event[i].ident].buffer.push_back('\r');
									sbuffer[event[i].ident].buffer.push_back('\n');
									sbuffer[event[i].ident].is_chunked = false;
								}
								else {
									for (unsigned long j = 0; j < sbuffer[event[i].ident].readed; j++)
										sbuffer[event[i].ident].buffer.push_back(chunck_buffer[j]);
								}
							}
							else
							{
								// check si il y a un body
								std::string header(header_buffer);
								std::string::size_type pos = header.find("Content-Length");
								if (header.size() > 0 && pos == std::string::npos)
								{
									cout << "FINISHED" << endl;
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
									break;
								}
								readed = server.reading(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start, buffer);
								for (unsigned long j = 0; j < readed; j++)
									sbuffer[event[i].ident].buffer.push_back(buffer[j]);

								if (req.isFinishedRequest(sbuffer[event[i].ident].buffer, sbuffer[event[i].ident].readed))
								{
									cout << "FINISHED" << endl;
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
								}
							}
							// display buffer total
							// cout << "BUFFER size : " << sbuffer[event[i].ident].buffer.size() << endl;
							// std::vector<char>::const_iterator start = sbuffer[event[i].ident].buffer.begin();
							// std::vector<char>::const_iterator end = sbuffer[event[i].ident].buffer.end();
							// cout << "BUFFER: '" << endl;
							// for (; start != end; start++)
							// 	std::cout << *start;
							// cout << "'" << endl;
						
						}
						else if (event[i].filter == EVFILT_WRITE)
						{
							if (sbuffer[event[i].ident].is_413) {
								std::stringstream reponse;
								std::fstream fd;
								cout << "on depasse le maxi body size server!!" << endl;
								
								char tmp_buff[4097];
								memset(tmp_buff, 0, 4097);
								size_t	o_read_p = 0;
								o_read_p = server.reading(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start, tmp_buff);

								if (o_read_p != 4096) {
									reponse << "HTTP/1.1 413 Payload Too Large\n";
									reponse << "Content-Type: text/plain\n";
									reponse << "Content-Length: 21\r\n\r\n";
									reponse << "Payload Too Large\r\n\r\n";
									write(event[i].ident, reponse.str().c_str(), reponse.str().size());
									sbuffer[event[i].ident].buffer.clear();
									sbuffer[event[i].ident].readed = 0;
									server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
									server.set_event(ke, event[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE);
									sbuffer[event[i].ident].is_413 = false;
								}
							}
							else
							{
								if (!server.writing(event[i].ident, sbuffer[event[i].ident].buffer, clients[event[i].ident]))
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
