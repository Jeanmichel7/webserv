/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/02 14:13:29 by lomasson         ###   ########.fr       */
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
	int				socket_server;
	struct kevent	change;
	struct kevent	event[1];
	char buffer[8000] = {0};
	Request req;
	
	memset(&server.interface , 0, sizeof(sockaddr_in));
	memset(&server.interfacee , 0, sizeof(sockaddr_in));
	config.selectServ();
	int ke = kqueue();
	try
	{
		socket_server = server.build(config, &change, "80", &server.interface);
		listen(socket_server, 10);
		kevent(ke, &change, 1, event, 1, NULL);
		socket_server = server.build(config, &change, "43", &server.interfacee);
		if (socket_server == -1 || ke == -1 || listen(socket_server, 10) == -1 || kevent(ke, &change, 1, event, 1, NULL) == -1)
			throw Settings::badCreation();
		// char test[550] = {0};
		// read(fd, &test, 550);
		while(1)
		{
			std::string reponse_request;
			int nevents = kevent(ke, NULL, 0, event, 1, NULL);
			if (nevents > 0)
			{
				int socket_client;
				// if (change.ident == 4)
				socket_client = accept( event->ident, (struct sockaddr *)&server.interface, (socklen_t *)&server.interface);
				// else
				// 	socket_client = accept( change.ident , (struct sockaddr *)&server.interfacee, (socklen_t *)&server.interfacee);
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
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}