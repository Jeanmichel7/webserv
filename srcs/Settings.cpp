/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:11:03 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/31 11:20:47 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Settings.hpp"

int	Settings::build()
{
	int	socket_fd;
	
	this->interface.sin_addr.s_addr = htonl(INADDR_ANY);
	this->interface.sin_port = htons(80);
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(socket_fd, reinterpret_cast<const sockaddr *>(&(interface)), (socklen_t)sizeof(interface)) == -1)
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

std::string	Settings::get( Config const& config )
{
	std::string buffer;
	std::string	reponse = "HTTP/1.1";
	std::stringstream n;
	std::fstream fd;
	std::string tmp;
	
	fd.open(config.getFile("/bg")->c_str(), std::fstream::in);
	if (!fd.is_open())
	{
		fd.open(config.getError(404)->c_str(), O_RDONLY);
		if (!fd.is_open())
			fd.open("http/404.html", O_RDONLY);
		reponse.append(" 404 Not Found\n");
	}
	else
		reponse.append(" 200 OK\n");
	reponse += Settings::date();
	reponse += "server: " + *config.getName() + "\n";
	reponse += "Last-Modified: \n";
	while(getline(fd, tmp))
		buffer += tmp + "\n";
	n << strlen(buffer.c_str());
	reponse += "Content-Length: " + n.str() + "\n";
	reponse += "Content-Type: text/html\n";
	reponse += "Connection: keep-alive\n";
	reponse += "\n" + buffer;
	fd.close();
	return (reponse);
}

std::string Settings::post( Config const& config )
{
	std::string			bodytest = "mybody";
	std::string			reponse = "HTTP/1.1";
	std::stringstream	n;
	std::fstream		fd;
	std::string			tmp;

	fd.open(config.getFile("/bg")->c_str(), std::fstream::in);
	if (!fd.is_open())
	{
		fd.open(config.getError(404)->c_str(), O_RDONLY);
		if (!fd.is_open())
			fd.open("http/404.html", O_RDONLY);
		reponse.append(" 404 Not Found\n");
	}
	else
	{
		std::string retour_cgi = "Status: 500";
		if (!retour_cgi.c_str())
			reponse.append(" 204 No Content\n");
		else if (strcmp(retour_cgi.c_str(), "Status: 500") == 0)
			reponse.append("500 Internal Server Error\n");
	}
	reponse += Settings::date();
	reponse += "server: " + *config.getName() + "\n";
	reponse += "Content-Length: 0\n";
	reponse += "Content-Type: text/html\n";
	reponse += "Connection: keep-alive\n\n";
	fd.close();
	return (reponse);
}

std::string Settings::del( Config const& config )
{
	(void)config;
	// std::string reponse;
	// char buffer[8000] = {0};
	std::string	reponse = "HTTP/1.1";
	// if ()
	// std::stringstream n;
	// if (fd < 0)
	// {
	// 	fd = open("http/404.html", O_RDONLY);
	// 	if (fd < 0)
	// 		fd = open("http/404.html", O_RDONLY);
	// 	reponse.append(" 404 Not Found\n");
	// }
	// else
	// {
	// 	reponse.append(" 200 OK\n");
	// }
	// close(fd);
	// n << strlen(buffer);
	// reponse += Settings::date();
	// reponse += "server: " + *config.getName() + "\n";
	// reponse += "Last-Modified: \n";
	// reponse += "Content-Length: " + n.str() + "\n";
	// reponse += "Content-Type: text/html\n";
	// reponse += "Connection: keep-alive\n\n";
	// reponse += buffer;
	return (reponse);
}

Settings::Settings()
{
			
}
Settings::~Settings()
{
			
}