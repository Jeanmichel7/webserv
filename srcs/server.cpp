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
	try 	{
		server.config = Config(argv[1]);
	}
	catch (std::runtime_error &e) {
		std::cout << e.what() << std::endl;
		return (0);
	}
	catch (std::exception &e) 	{
		return (0);
	}


	struct kevent 							event[MAX_REQUESTS];
	Request 										req;
	std::map<int, sockaddr_in> 	clients;
	int 												ke = kqueue();
	try {
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
							// if (reqIsChuncked(buffer) == true)
							// 	sbuffer[event[i].ident].is_chunked = true;
							// if (sbuffer[event[i].ident].is_chunked = true) {
							// 	// get body
							// 	string chunck(buffer);
							// 	std::string::size_type pos = chunck.find("Transfer-Encoding: chunked");
							// 	if (pos != std::string::npos) {
							// 		pos = chunck.find("\r\n\r\n");
							// 		if (pos != std::string::npos) {
							// 			if ((pos = chunck.find("\r\n")) != string::npos)
							// 			{
							// 				string str_hexa = chunck.substr(0, pos);
							// 				chunck.erase(0, pos + 2);

							// 				cout << "str_hexa : " << str_hexa << endl;

							// 				string::size_type x;
							// 				std::stringstream ss;
							// 				std::stringstream ssbody;
							// 				ss << std::hex << str_hexa;
							// 				ss >> x;
							// 				long size = static_cast<string::size_type>(x);
							// 				cout << "size : " << size << endl;

							// 				for (long int i = 0; i < size; i++) {
							// 					sbuffer[event[i].ident].buffer.push_back(chunck[i]);
							// 				}
							// 			}
							// 		}
							// 		else {
							// 			cerr << "ERROR: no body in chunck ?!" << endl;
							// 		}
							// 	} 
							// 	else {
							// 		if ((pos = chunck.find("\r\n")) != string::npos) {
							// 			string str_hexa = chunck.substr(0, pos);
							// 			chunck.erase(0, pos + 2);

							// 			cout << "str_hexa : " << str_hexa << endl;

							// 			string::size_type x;
							// 			std::stringstream ss;
							// 			std::stringstream ssbody;
							// 			ss << std::hex << str_hexa;
							// 			ss >> x;
							// 			long size = static_cast<string::size_type>(x);
							// 			cout << "size : " << size << endl;

							// 			if (size == 0) {
							// 				// end of chunck
							// 				server.set_event(ke, event[i].ident, EVFILT_READ, EV_DELETE);
							// 				server.set_event(ke, event[i].ident, EVFILT_WRITE, EV_ADD);
							// 			}
							// 			else {
							// 				for (long int i = 0; i < size; i++) {
							// 					sbuffer[event[i].ident].buffer.push_back(chunck[i]);
							// 				}
							// 			}
							// 		}
							// 	}
							// }
							// else
								sbuffer[event[i].ident].buffer.insert(sbuffer[event[i].ident].buffer.end(), buffer, buffer + readed);
							if (event[i].filter == EVFILT_READ && req.isFinishedRequest(sbuffer[event[i].ident].buffer, sbuffer[event[i].ident].readed))
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
