/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/08 22:34:33 by ydumaine         ###   ########.fr       */
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
	Settings server;
	int socket_server_a;
	int socket_server_b;

	struct kevent event;
	char buffer[8000] = {0};
	Request req;
	// ip a changer ici, dans la fonction build et dans le fichier de conf
	config.selectServ("127.0.0.1", "4241");
	int ke = kqueue();
	try
	{
		if (ke == -1)
			throw Settings::badCreation();
		socket_server_a = server.build("4242", ke);
		socket_server_b = server.build("4241", ke);
		while (1)
		{
			std::string reponse_request;
			int nevents = kevent(ke, NULL, 0, &event, 2, NULL);
			if (nevents > 0)
			{
				int socket_client = accept(event.ident, (struct sockaddr *)event.udata, (socklen_t *)event.udata);
				read(socket_client, buffer, 8000);
				// printf("%s\n", buffer);
				if (req.parseRequest(buffer))
					reponse_request = server.badRequest(config);
				else if (!config.selectServ(req.header.host_ip, req.header.port, req.header.host))
					reponse_request = server.badRequest(config);
				else if (req.method.isGet)
					reponse_request = server.get(config, req);
				else if (req.method.isPost || req.method.isDelete)
					reponse_request = server.post(config, req);
				else
					reponse_request = server.badRequest(config);
				req.printRequest();
				if (req.method.type == "STOP")
					throw std::exception();
				send(socket_client, reponse_request.c_str(), strlen(reponse_request.c_str()), 0);
				std::cout << std::endl
						  << "Response : " << reponse_request << std::endl;
				printf("------------------Hello message sent-------------------\n");
				close(socket_client);
				req.reset();
			}
		}
	}
	catch (const std::exception &e)
	{
		close(socket_server_b);
		close(socket_server_a);
		std::cout << strerror(errno);
		std::cerr << std::endl
				  << e.what() << std::endl;
	}
}