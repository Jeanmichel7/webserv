/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:11:03 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/15 15:01:35 by lomasson         ###   ########.fr       */
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

	this->config.selectFirstServ();
	for(unsigned int i = 0; i < this->config.getServNumb(); i++)
	{
		std::memset(&serv_addr , 0, sizeof(serv_addr));
		serv_addr.ai_family = AF_INET;
		serv_addr.ai_socktype = SOCK_STREAM;
		int status = getaddrinfo(this->config.getIp().c_str(), this->config.getPort().c_str(), &serv_addr, &res);
		if (status != 0)
			throw Settings::badCreation();
		socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socket_fd == -1)
			throw Settings::badCreation();
		int r = 1;
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &r, sizeof(r)) < 0)
			throw Settings::badCreation();
  		fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK);
		int bind_status = bind(socket_fd, res->ai_addr, res->ai_addrlen);
		if (bind_status == -1)
			throw Settings::badCreation();
		freeaddrinfo(res);
		if (listen(socket_fd, 4000) == -1)
			throw Settings::badCreation();
		EV_SET(&change, socket_fd, EVFILT_READ , EV_ADD | EV_ENABLE | EV_EOF, 0, 0, &serv_addr);
		if (kevent(ke, &change, 1, NULL, 0, NULL) == -1)
				throw Settings::badCreation();
		++config;
	}
}

std::string Settings::date(void)
{
	time_t				tmm = time(0);
	std::string			rdate, s, tmp;
	std::stringstream	str(asctime(localtime(&tmm)));

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
	std::string			buffer = "";
	std::string			reponse = "HTTP/1.1";
	std::stringstream	n;
	std::fstream		fd;
	std::string			tmp;

	if (!this->config.getMethod(req.method.path).isget)
		return (this->method_not_allowed(req));
	if (this->config.getFile(req.method.path) == NULL)
	{
		reponse.append(" 404 Not Found\n");
		fd.open(this->config.getError(404)->c_str());
		if (!fd.is_open())
			fd.open("http/404.html");
	}
	else
	{
		const char *file = this->config.getFile(req.method.path)->c_str();
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
				if (this->config.getError(404))
					fd.open(this->config.getError(404)->c_str());
				if (!fd.is_open())
					fd.open("http/404.html");
				reponse.append(" 404 Not Found\n");
			}
		}
	}
	while (getline(fd, tmp))
		buffer += tmp + "\n";
	reponse += Settings::date() + "\n";
	reponse += "server: " + *this->config.getName() + "\n";
	n << buffer.size();
	reponse += "Content-Length: " + n.str() + "\n";
	if (!req.header.content_type.empty())
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
	if (!this->config.getMethod(req.method.path).ispost)
		return (this->method_not_allowed(req));
	fd.open(this->config.getFile(req.method.path)->c_str(), std::fstream::in);
	if (!fd.is_open())
	{
		fd.open(this->config.getError(404)->c_str(), O_RDONLY);
		if (!fd.is_open())
			fd.open("http/404.html", O_RDONLY);
		reponse << " 404 Not Found\n";
	}
	// std::cout << "Executing CGI..." << std::endl;
	if (req.body.content.size() > 30)
		rvalue_script = CGI::execute_cgi(this->config, req);
	if (rvalue_script.size() == 0)
		reponse << " 204 No Content\n";
	else if (strcmp(rvalue_script.c_str(), "Status: 500") == 0)
		reponse << "500 Internal Server Error\n";
	// std::cout << "Executing CGI end" << std::endl;
	reponse << Settings::date() << "\n";
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

std::string Settings::badRequest(Request const& req)
{
	std::stringstream reponse;
	
	reponse << "HTTP/1.1 400 Bad Request\n";
	reponse << Settings::date() << "\n";
	reponse << "server: " << *this->config.getName() + "\n";
	reponse << "server: " << "myserv" << "\n";
	reponse << "Content-Length: 21\n";
	reponse << "Connection: keep-alive\n\n";
	reponse << "Malformed ";
	if (this->config.getMethod(req.method.path).isget)
		reponse << "GET";
	else if (this->config.getMethod(req.method.path).ispost)
		reponse << "POST";
	reponse << "\n request\n";
	return (reponse.str());
}

std::string Settings::method_not_allowed(Request const& req)
{
	std::stringstream reponse;
	
	reponse << "HTTP/1.1 405 Method Not Allowed\n";
	reponse << "Allow:";
	if (this->config.getMethod(req.method.path).ispost)
		reponse << " POST";
	if (this->config.getMethod(req.method.path).isget)
		reponse << " GET";
	reponse << "\nContent-Type: text/plain\n"; 

	reponse << Settings::date() << "\n";
	reponse << "server: " << *this->config.getName() + "\n";
	reponse << "Content-Length: 25\n\n";
	reponse << "Method Not Allowed (405)\n";
	return (reponse.str());
}

std::string Settings::reading(int socket, Request req)
{
	std::stringstream 	sbuffer;
	int					o_read = 0;
	std::memset(&req.buffer, 0, sizeof(req.buffer));
	// std::cout << "Max size: \n" << config.getMaxSize() << std::endl;
	o_read = recv(socket, req.buffer, config.getMaxSize(), MSG_DONTWAIT);
	// std::cout << "le retour de recv: " << o_read << std::endl;
	if (o_read == -1)
		return (std::string());
	sbuffer << req.buffer;
	if (o_read == config.getMaxSize()) {
		while (o_read == config.getMaxSize()) {
			// std::cout << "reading socket ..." << std::endl;
			req.resetBuffer();
			o_read = recv(socket, req.buffer, REQ_MAX_SIZE, 0);
			sbuffer << req.buffer;
		}
	}
	std::cout << "\n\nREAD:\n" << sbuffer.str() << "\n\n";
	return (sbuffer.str());
}

void Settings::writing(int socket, Request & req, std::string sbuffer)
{
	std::string reponse_request;
	if (std::strncmp(sbuffer.c_str(), "HEAD", 4) == 0 || std::strncmp(sbuffer.c_str(), "PUT", 4) == 0)
	{
		reponse_request = this->method_not_allowed(req);
		// reponse_request += "HTTP/1.1 200 OK\n";
		// reponse_request += this->date() + "\n";
		// reponse_request += "server: " + *this->config.getName() + "\n";
		// reponse_request += "Content-Length: 0\n";
		// reponse_request += "Connection: keep-alive\n";
	}
	else
	{
		if (req.parseRequest(sbuffer))
			reponse_request = this->method_not_allowed(req);
		else if (!this->config.selectServ(req.header.host_ip, req.header.port, req.header.host))
			reponse_request = this->badRequest(req);
		else if (req.method.isGet)
			reponse_request = this->get(req);
		else if (req.method.isPost || req.method.isDelete)
			reponse_request = this->post(req);
		else
			reponse_request = this->badRequest(req);
	}
	std::cout << std::endl << std::endl << "WRITE : "  << std::endl << reponse_request << std::endl;
	send(socket, reponse_request.c_str(), reponse_request.size(), MSG_DONTWAIT);	
}

int Settings::checkmethod(Request const& req)
{
	std::string method;
	for(int i = 0; sbuffer.c_str()[i] != ' '; i++)
		method += sbuffer.c_str()[i];
	if (method == "PUT" || method == "HEAD" || method == "OPTIONS" || method == "TRACE" || method == "CONNECT")
		return (-1);
	else if (method == "POST" && this->config.getMethod(req.method.path).ispost)
		return (0);
	if ()
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
/*
POST / HTTP/1.1
Host: 127.0.0.1:4241
User-Agent: Go-http-client/1.1\
Transfer-Encoding: chunked
Content-Type: test/file
Accept-Encoding: gzip

0

HEAD / HTTP/1.1
Host: 127.0.0.1:4241
User-Agent: Go-http-client/1.1


*/