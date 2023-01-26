/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:11:03 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/26 12:23:40 by ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Settings.hpp"

int	Settings::build()
{
	int	socket_fd;
	
	this->interface.sin_addr.s_addr = htonl(INADDR_ANY);
	this->interface.sin_port = htons(80);
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(socket_fd, (const struct sockaddr *)&interface, sizeof(interface)) == -1)
		return (-1);
	if (listen(socket_fd, 100000) == -1)
		return (-1);
	return (socket_fd);
}

std::string	Settings::date( void )
{
	time_t tmm = time(0);
	std::string rdate, s, tmp;
	std::stringstream str(asctime(localtime(&tmm)));

	getline(str, rdate, ' ');
	getline(str, tmp, ' ');
	getline(str, s, ' ');
	rdate += ", " + s + " " + tmp;
	getline(str, tmp, ' ');
	getline(str, s, '\n');
	rdate += " " + s + " " + tmp + " GMT";
	return (rdate);
}

// int	Settings::execute_cgi(std::string path)
// {
	
// }

// std::string	Settings::get(Config const& config) {

// }

// std::string	Settings::get(Config const& config)
// {
// 	std::string	reponse = "HTTP/1.1";
// 	int fd = open(config.getFile("srcs/site/test.html").c_str(), O_RDONLY);
// 	if (fd < 0)
// 	{
// 		fd = open(config.getError(404).c_str(), O_RDONLY);
// 		if (fd < 0)
// 			fd = open("srcs/error_pages/404.html", O_RDONLY);
// 		reponse.append(" 404 Not Found\n");
// 	}
// 	else
// 		reponse.append(" 200 OK\n");
// 	reponse += Settings::date() + "\nserver: WebServ";
	
// 	return (reponse);
// }

Settings::Settings()
{
			
}
Settings::~Settings()
{
			
}