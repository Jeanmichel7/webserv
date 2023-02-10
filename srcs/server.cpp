/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/10 13:50:20 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int main(int argc, char **argv)
{
	Settings server;
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
	struct kevent 	event;
	Request 		req;
	int ke = kqueue();
	try
	{
		if (ke == -1)
			throw Settings::badCreation();
		server.build(ke);
		while (1)
		{
			std::string 	reponse_request;
			stringstream 	sbuffer;

			int nevents = kevent(ke, NULL, 0, &event, 2, NULL);
			if (nevents > 0)
			{
				int socket_client = accept(event.ident, (struct sockaddr *)event.udata, (socklen_t *)event.udata);

				string::size_type o_read = 0;
				o_read = read(socket_client, req.buffer, REQ_MAX_SIZE);
				sbuffer << req.buffer;
				if (o_read == REQ_MAX_SIZE) {
					while (o_read == REQ_MAX_SIZE) {
						req.resetBuffer();
						o_read = read(socket_client, req.buffer, REQ_MAX_SIZE);
						sbuffer << req.buffer;
					}
				}
				if (req.parseRequest(sbuffer.str()))
					reponse_request = server.badRequest();
				else if (!server.config.selectServ(req.header.host_ip, req.header.port, req.header.host))
					reponse_request = server.badRequest();
				else if (req.method.isGet)
					reponse_request = server.get(req);
				else if (req.method.isPost || req.method.isDelete)
					reponse_request = server.post(req);
				else
					reponse_request = server.badRequest();
				req.printRequest();
				send(socket_client, reponse_request.c_str(), strlen(reponse_request.c_str()), 0);
				std::cout << std::endl
						  << "Response : " << reponse_request << std::endl;
				printf("------------------ Waiting new connection-------------------\n");
				close(socket_client);
				req.reset();
			}
		}
	}
	catch (const std::exception &e)
	{
		std::cout << strerror(errno);
		std::cerr << std::endl
				  << e.what() << std::endl;
	}
}