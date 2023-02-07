/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/07 12:16:10 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int main(int argc, char **argv)
{
	Config config;
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
	config = Config(argv[1]);
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
	// int				socket_server_a;
	int				socket_server_b;
	struct kevent	change;
	struct kevent	event;
	char buffer[8000] = {0};
	Request req;
	
	
	config.selectServ();
	int ke = kqueue();
try
{
		if (ke == -1)
			throw Settings::badCreation();
		// socket_server_a = server.build(config, &change, "80", ke);
		socket_server_b = server.build(config, &change, "80", ke);
		while(1)
		{
			std::string reponse_request;
			int nevents = kevent(ke, NULL, 0, &event, 2, NULL);
			if (nevents > 0)
			{
				int socket_client = accept( event.ident, (struct sockaddr *)event.udata, (socklen_t *)event.udata);
				read(socket_client, buffer, 8000);
				// printf("%s\n", buffer);
				if (req.parseRequest(buffer))
					reponse_request = server.badRequest(config);
				if (strncmp(buffer, "GET", 3) == 0)
					reponse_request = server.get(config, req);
				else if (strncmp(buffer, "POST", 4) == 0 || strncmp(buffer, "DELETE", 6) == 0)
					reponse_request = server.post(config, req);
				else
					reponse_request = server.badRequest(config);
				req.printRequest();
				if(req.method.type == "STOP")
					throw std::exception();
				send(socket_client, reponse_request.c_str(), strlen(reponse_request.c_str()),0);
				std::cout << std::endl << "Response : "<< reponse_request << std::endl;
				printf("------------------Hello message sent-------------------\n");
				close(socket_client);
				req.reset();
			}
		}
	}
	catch (const std::exception &e) {
		std::cout << strerror(errno);
		std::cerr << std::endl << e.what() << std::endl;
	}
}