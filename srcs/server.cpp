/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/03 13:29:52 by ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int main( void )
{
	Config config;
	try 
	{
	config = Config("srcs/conf.conf");
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
	Settings		server;
	int				socket_server_a;
	int				socket_server_b;
	struct kevent	change;
	struct sockaddr_in serv_addr1, serv_addr2;
	struct kevent	event[5] = {0};
	char buffer[8000] = {0};
	Request req;
	
	memset(&serv_addr1 , 0, sizeof(sockaddr_in));
	memset(&serv_addr2 , 0, sizeof(sockaddr_in));
	config.selectServ();
	//std::cout << "valeur du body :" << CGI::execute_cgi("nique ta mere", "/nolife/", "test.sh", config) << std::endl;
	Methods test;
	test = config.getMethod("/bg");
	std::cout << test.isget << std::endl;
	int ke = kqueue();
	try
	{
		socket_server_a = server.build(config, &change, "4242", &serv_addr1);
		kevent(ke, &change, 1, event, 1, NULL);
		socket_server_b = server.build(config, &change, "80", &serv_addr2);
		if (socket_server_a == -1 || socket_server_b == -1 || ke == -1 || kevent(ke, &change, 1, event, 1, NULL) == -1)
			throw Settings::badCreation();
		// char test[550] = {0};
		// read(fd, &test, 550);
		while(1)
		{
			std::string reponse_request;
			int nevents = kevent(ke, NULL, 0, event, 1, NULL);
			for(int i = 0; i < 2; i++)
			{
				if (nevents > 0)
				{
					int socket_client;
					if (event->ident == socket_server_a)
						socket_client = accept( socket_server_a, (struct sockaddr *)&serv_addr1, (socklen_t *)&serv_addr1);
					else if (event->ident == socket_server_b)
						socket_client = accept( socket_server_b , (struct sockaddr *)&serv_addr2, (socklen_t *)&serv_addr2);
					else
						break ;
					read(socket_client, buffer, 8000);
					printf("%s\n", buffer);
					if (req.parseRequest(buffer))
						reponse_request = server.badRequest(config);
					else if (strncmp(buffer, "GET", 3) == 0)
						reponse_request = server.get(config, req);
					else if (strncmp(buffer, "POST", 4) == 0 || strncmp(buffer, "DELETE", 6) == 0)
						reponse_request = server.post(config, req);
					else
						reponse_request = server.badRequest(config);
					send(socket_client, reponse_request.c_str(), strlen(reponse_request.c_str()),0);
					std::cout << std::endl << reponse_request << std::endl;
					printf("------------------Hello message sent-------------------\n");
					// EV_SET(event, socket_client, EVFILT_READ | EVFILT_WRITE , EV_ADD | EV_ENABLE, 0, 0, &timeout);
					// kevent(ke, event, 1, NULL, 0, NULL);
					close(socket_client);
				}
			}

			// req.printRequest(req);
		}
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}