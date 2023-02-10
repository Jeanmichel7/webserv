/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:11:03 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/10 15:52:20 by ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Settings.hpp"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void	Settings::build(int ke)
{
	static struct kevent	change;
	int						socket_fd;
	struct addrinfo			serv_addr, *res;

	this->list_of_serv_socket = new int[this->config.getServNumb()];
	this->config.selectFirstServ();
	for(unsigned int i = 0; i < this->config.getServNumb(); i++)
	{
		std::memset(&serv_addr , 0, sizeof(serv_addr));
		serv_addr.ai_family = AF_INET;
		serv_addr.ai_socktype = SOCK_STREAM;
		std::cout << std::endl;
		std::cout << "Ip: " << this->config.getIp() << std::endl;
		std::cout << "Port: " << this->config.getPort() << std::endl;
		std::cout << std::endl;
		int status = getaddrinfo(this->config.getIp().c_str(), this->config.getPort().c_str(), &serv_addr, &res);
		if (status != 0)
			throw Settings::badCreation();
		socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socket_fd == -1)
    	    throw Settings::badCreation();
		int bind_status = bind(socket_fd, res->ai_addr, res->ai_addrlen);
		if (bind_status == -1)
			throw Settings::badCreation();
		freeaddrinfo(res);
		if (listen(socket_fd, 10) == -1)
			throw Settings::badCreation();
		EV_SET(&change, socket_fd, EVFILT_READ , EV_ADD, 0, 0, &serv_addr);
		if (kevent(ke, &change, 1, NULL, 0, NULL) == -1)
				throw Settings::badCreation();
		this->list_of_serv_socket[i] = socket_fd;
		++config;
	}
}

std::string Settings::date(void)
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
	rdate += " " + s + " " + tmp + " GMT";
	return (rdate);
}

std::string Settings::get(Request const &req)
{
	std::string buffer;
	std::string reponse = "HTTP/1.1";
	std::stringstream n;
	std::fstream fd;
	std::string tmp;

	if (this->config.getFile(req.method.path) == NULL)
		reponse.append(" 404 Not Found\n");
	else
	{
		const char *file = this->config.getFile(req.method.path)->c_str();
		// const char *file = open(, O_RDONLY);


		fd.open(file, std::fstream::in);
		if (fd.is_open())
			reponse.append(" 200 OK\n");
		else if (!this->config.getDirectoryListing(req.method.path).empty())
		{
			DIR *dir = opendir(this->config.getDirectoryListing(req.method.path).c_str());
			if (dir)
			{
				reponse += " 200 OK\r\n";
				buffer = "<html><body><ul>";

				struct dirent *entry;
				while ((entry = readdir(dir)) != nullptr)
					if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
						buffer += "<li>" + std::string(entry->d_name) + "</li>\n";
				buffer += "</ul></body></html>\r\n";
				closedir(dir);
			}
			else
			{
				fd.open(this->config.getError(404)->c_str());
				if (!fd.is_open())
					fd.open("http/404.html");
				reponse.append(" 404 Not Found\n");
			}
		}
	}
	while (getline(fd, tmp))
		buffer += tmp + "\n";
	reponse += Settings::date();
	reponse += "server: " + *this->config.getName() + "\n";
	reponse += "Last-Modified: \n";

	n << buffer.size();
	reponse += "Content-Length: " + n.str() + "\n";
	reponse += "Content-Type: " + req.header.content_type + "\n";
	reponse += "Connection: keep-alive\n";
	reponse += "\n" + buffer;
	fd.close();
	return (reponse);
}

std::string Settings::post(Request const &req)
{
	std::stringstream reponse;
	std::string rvalue_script;
	std::fstream fd;

	reponse << "HTTP/1.1";
	fd.open(this->config.getFile(req.method.path)->c_str(), std::fstream::in);
	if (!fd.is_open())
	{
		fd.open(this->config.getError(404)->c_str(), O_RDONLY);
		if (!fd.is_open())
			fd.open("http/404.html", O_RDONLY);
		reponse << " 404 Not Found\n";
	}
	else
	{
		std::string retour_cgi = "Status: 500";
		if (!retour_cgi.c_str())
			reponse << " 204 No Content\n";
		else if (strcmp(retour_cgi.c_str(), "Status: 500") == 0)
			reponse << "500 Internal Server Error\n";
	}
	std::cout << "Executing CGI..." << std::endl;
	rvalue_script = CGI::execute_cgi(this->config, req);
	std::cout << "Executing CGI end" << std::endl;
	reponse << Settings::date();
	reponse << "server: " << *this->config.getName() << "\n";
	reponse << "Content-Length: " << rvalue_script.size();
	reponse << "\nContent-Type: text/html\n";
	reponse << "Connection: keep-alive\n\n";
	reponse << rvalue_script;

	std::string::size_type pos = 0;
	if ((pos = rvalue_script.find("content_length")) != std::string::npos)
		reponse << EOF;
	fd.close();
	return (reponse.str());
}

std::string Settings::badRequest()
{
	std::stringstream reponse;
	reponse << "HTTP/1.1 400 Bad Request\n";
	reponse << Settings::date() << "\n";
	reponse << "server: " << *this->config.getName() + "\n";
	reponse << "server: " << "myserv" << "\n";
	reponse << "Content-Length: 0\n";
	reponse << "Connection: closed\n\n";
	return (reponse.str());
}

Settings::Settings(Config const& base) : config(base)
{
}

Settings::Settings()
{
}
Settings::~Settings()
{
}