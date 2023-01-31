/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:11:03 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/31 14:35:16 by ydumaine         ###   ########.fr       */
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
	rdate = "date: " + rdate + ", " + s + " " + tmp;
	getline(str, tmp, ' ');
	getline(str, s, '\n');
	rdate += " " + s + " " + tmp + " GMT\n";
	return (rdate);
}




// std::string	Settings::get(Config const& config) {

std::string	Settings::get(Config& config )
{
	char buffer[8000] = {0};
	std::string	reponse = "HTTP/1.1";
	std::stringstream n;
	std::string const *path = config.getFile("/bg");
	if (path == NULL)
		return (0);
	int fd = open((path->c_str()), O_RDONLY);
	if (fd < 0)
	{
		fd = open("http/404.html", O_RDONLY);
		if (fd < 0)
			fd = open("http/404.html", O_RDONLY);
		reponse.append(" 404 Not Found\n");
	}
	else
		reponse.append(" 200 OK\n");
	read(fd, &buffer, 4000);
	close(fd);
	n << strlen(buffer);
	reponse += Settings::date() + "server: " + *config.getName() + "\nLast-Modified: " + "\nContent-Length: " + n.str() + "\nContent-Type: text/html\n" + "Connection: keep-alive\n\n" + buffer;
	return (reponse);
}

std::string Settings::post( Config& config )
{
	std::string reponse;
	(void)config;
	return (reponse);
}

Settings::Settings()
{
			
}
Settings::~Settings()
{
			
}