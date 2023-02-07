/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:11:03 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/07 12:13:17 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Settings.hpp"

int	Settings::build( Config const& config, struct kevent *change, const char *i, int ke)
{
	int					socket_fd;
	struct sockaddr_in	serv_addr;
	(void)config;

	std::memset(&serv_addr , 0, sizeof(sockaddr_in));
 	// inet_pton(AF_INET, "8.8.8.8", &serv_addr.sin_addr);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(i));
	serv_addr.sin_family = AF_INET;
	socket_fd = socket(AF_INET, SOCK_STREAM, 6);
	if (bind(socket_fd, reinterpret_cast<const struct sockaddr *>(&serv_addr), (socklen_t)sizeof(serv_addr)) == -1)
		throw Settings::badCreation();
	if (listen(socket_fd, 10) == -1)
		throw Settings::badCreation();
	EV_SET(change, socket_fd, EVFILT_READ , EV_ADD, 0, 0, &serv_addr);
	if (kevent(ke, change, 1, NULL, 0, NULL) == -1)
			throw Settings::badCreation();
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

std::string	Settings::get( Config& config, Request const& req )
{
	std::string buffer;
	std::string	reponse = "HTTP/1.1";
	std::stringstream n;
	std::fstream fd;
	std::string tmp;
	
	if (config.getFile(req.method.path) == NULL)
		reponse.append(" 404 Not Found\n");
	else {
		const char * file = config.getFile(req.method.path)->c_str();
		fd.open(file, std::fstream::in);
		if (fd.is_open())
			reponse.append(" 200 OK\n");
		else if (!config.getDirectoryListing(req.method.path).empty())
		{
			DIR* dir = opendir(config.getDirectoryListing(req.method.path).c_str());
			if (dir)
			{
				reponse += " 200 OK\r\n";
				buffer = "<html><body><ul>";
				
				struct dirent* entry;
				while ((entry = readdir(dir)) != nullptr)
					if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
						buffer += "<li>" + std::string(entry->d_name) + "</li>\n";
				buffer += "</ul></body></html>\r\n";
				closedir(dir);
			}
			else
			{
				fd.open(config.getError(404)->c_str());
				if (!fd.is_open())
					fd.open("http/404.html");
				reponse.append(" 404 Not Found\n");
			}
		}
	}
	while(getline(fd, tmp))
		buffer += tmp + "\n";
	reponse += Settings::date();
	reponse += "server: " + *config.getName() + "\n";
	reponse += "Last-Modified: \n";
	
	n << strlen(buffer.c_str());
	reponse += "Content-Length: " + n.str() + "\n";
	reponse += "Content-Type: " + req.header.content_type + "\n";
	reponse += "Connection: keep-alive\n";
	reponse += "\n" + buffer;
	fd.close();
	return (reponse);
}

std::string Settings::post( Config& config, Request const& req )
{
	std::stringstream	reponse;
	std::string			rvalue_script;
	std::fstream		fd;

	reponse << "HTTP/1.1";
	fd.open(config.getFile(req.method.path)->c_str(), std::fstream::in);
	if (!fd.is_open())
	{
		fd.open(config.getError(404)->c_str(), O_RDONLY);
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
	rvalue_script = CGI::execute_cgi(config, req);
	reponse << Settings::date();
	reponse << "server: " + *config.getName() + "\n";
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

std::string	Settings::badRequest( Config const& config )
{
	std::stringstream	reponse;
	reponse << "HTTP/1.1 400 Bad Request\n";
	reponse << Settings::date() << "\n";
	reponse << "server: " << *config.getName() + "\n";
	reponse << "Content-Length: 0\n";
	reponse << "Connection: closed\n\n";
	return (reponse.str());
}

Settings::Settings()
{
			
}
Settings::~Settings()
{
			
}