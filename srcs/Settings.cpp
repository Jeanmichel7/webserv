/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:11:03 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/25 12:34:00 by lomasson         ###   ########.fr       */
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

int	Settings::execute_cgi(std::string path)
{
	(void)path;
	// int pip[2];
	// char **env;
	// pid_t pid = fork();
	// if (pid == 0)
	// {
	// 	int	fd = open("tmp/resultcgi.txt", O_RDWR | O_TRUNC);
	// 	dup2(STDOUT_FILENO, fd);
		
	// }
	// else
	// {
	// 	waitpid(pid, 0, NULL);
	// 	// execve();
	// }
	return (1);
}

std::string	Settings::get( void )
{
	char buffer[5000] = {0};
	std::string	reponse = "HTTP/1.1";
	int fd = open("http/index.html", O_RDONLY);
	if (fd < 0)
	{
		fd = open("http/404.html", O_RDONLY);
		if (fd < 0)
			fd = open("http/404.html", O_RDONLY);
		reponse.append(" 404 Not Found\n");
	}
	else
		reponse.append(" 200 OK\n");
	read(fd, &buffer, 5000);
	close(fd);
	reponse += Settings::date() + "server: WebServ\n" + "Last-Modified: \n" + "Content-Length: 500\n" + "Content-Type: text/html\n" + "Connection: keep-alive\n\n" + buffer;

	return (reponse);
}

Settings::Settings()
{
			
}
Settings::~Settings()
{
			
}