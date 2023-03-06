/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/03/06 23:06:02 by ydumaine         ###   ########.fr       */
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
							// cout << "header_buffer : '" << header_buffer << "'" << endl;
							if (reqIsChuncked(header_buffer) == true)
								sbuffer[event[i].ident].is_chunked = true;
							// cout << "header_buffer : " << header_buffer << endl;
							// cout << "req is chunked : " << sbuffer[event[i].ident].is_chunked << endl;
							if (sbuffer[event[i].ident].is_chunked == true)
							{
								// cout << "On est dans les chuncks" << endl;
								char *chunck_buffer = NULL;
								// bool is_last_chunck = false;
								sbuffer[event[i].ident].readed = 0;
								chunck_buffer = server.reading_chunck(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start);
								// cout << "chunck_buffer : '" << chunck_buffer << "'" << endl;
								
								if (strlen(chunck_buffer) == 0) {
									// cout << "FINISHED chunck request" << endl;
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
								// sbuffer[event[i].ident].readed = 0;
								readed = server.reading(event[i].ident, sbuffer[event[i].ident].readed, sbuffer[event[i].ident].time_start, buffer);
								// if (readed == 18446744073709551615)
								// {
								// 	cout << "ERROR: readed == 18446744073709551615" << endl;
								// 	server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
								// 	server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
								// 	break;
								// }
								cout << "readed : " << readed << endl;
								cout << "read[]sdfdsf" << sbuffer[event[i].ident].readed << endl;
								cout << "buffer avant : '" << buffer << "'" << endl;

								// sbuffer[event[i].ident].buffer.insert(sbuffer[event[i].ident].buffer.end(), readed, *buffer );
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
							cout << "BUFFER size : " << sbuffer[event[i].ident].buffer.size() << endl;
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

								server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);

								reponse << "HTTP/1.1 413 Payload Too Large\n";
								reponse << "Content-Type: text/plain\n";
								// reponse << server.date();
								// reponse << "server: " << server.config.getName() << "\n";
								// reponse << "Connection: close\n";
								reponse << "Content-Length: 21\r\n\r\n";
								reponse << "Payload Too Large\r\n\r\n";
								// char tmp_buff[4096];
								// while (recv(event[i].ident, tmp_buff, 4096, 0) > 0)
								// 	;

								// usleep(1000);
								std::cout << "\nreponse : '" << reponse.str() << "'" << std::endl;
								usleep(1000000);
								write(event[i].ident, reponse.str().c_str(), reponse.str().size());
								sbuffer[event[i].ident].buffer.clear();
								sbuffer[event[i].ident].readed = 0;
								server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_DELETE);
								clients.erase(event[i].ident);
								close(event[i].ident);
								sbuffer[event[i].ident].is_413 = false;
								// usleep(1000);
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
