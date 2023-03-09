/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:11:03 by lomasson          #+#    #+#             */
/*   Updated: 2023/03/02 19:23:28by ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Settings.hpp"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <unistd.h>
#include <cstdio>

Settings::Settings()
{
	this->check_request_timeout.tv_sec = 1;
	this->ext[".aac"] = "audio/aac";
	this->ext[".avi "] = "video/x-msvideo";
	this->ext[".bin"] = "application/octet-stream";
	this->ext[".bmp"] = "image/bmp";
	this->ext[".bz"] = "application/x-bzip";
	this->ext[".bz2"] = "application/x-bzip2";
	this->ext[".csh"] = "application/x-csh";
	this->ext[".css"] = "text/css";
	this->ext[".csv"] = "text/csv";
	this->ext[".doc"] = "application/msword";
	this->ext[".docx "] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	this->ext[".gif"] = "image/gif";
	this->ext[".htm"] = "text/html";
	this->ext[".html"] = "text/html";
	this->ext[".ico"] = "image/x-icon";
	this->ext[".jar"] = "application/java-archive";
	this->ext[".jpeg"] = "image/jpeg";
	this->ext[".jpg"] = "image/jpeg";
	this->ext[".js"] = "application/javascript";
	this->ext[".json"] = "application/json";
	this->ext[".mid"] = "audio/midi";
	this->ext[".midi"] = "audio/midi";
	this->ext[".mpeg"] = "video/mpeg";
	this->ext[".mp3"] = "audio/mpeg";
	this->ext[".mp4"] = "video/mp4";
	this->ext[".odp "] = "application/vnd.oasis.opendocument.presentation";
	this->ext[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	this->ext[".odt"] = "application/vnd.oasis.opendocument.text";
	this->ext[".oga"] = "audio/ogg";
	this->ext[".ogv"] = "video/ogg";
	this->ext[".ogx"] = "application/ogg";
	this->ext[".otf"] = "font/otf";
	this->ext[".png"] = "image/png";
	this->ext[".pdf"] = "application/pdf";
	this->ext[".ppt"] = "application/vnd.ms-powerpoint";
	this->ext[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	this->ext[".rar"] = "application/x-rar-compressed";
	this->ext[".rtf"] = "application/rtf";
	this->ext[".sh"] = "application/x-sh";
	this->ext[".svg"] = "image/svg+xml";
	this->ext[".swf"] = "application/x-shockwave-flash";
	this->ext[".tar"] = "application/x-tar";
	this->ext[".tif"] = "image/tiff";
	this->ext[".tiff"] = "image/tiff";
	this->ext[".ts"] = "application/typescript";
	this->ext[".ttf"] = "font/ttf";
	this->ext[".txt"] = "text/plain";
	this->ext[".vsd"] = "application/vnd.visio";
	this->ext[".wav"] = "audio/x-wav";
	this->ext[".weba"] = "audio/webm";
	this->ext[".webm"] = "video/webm";
	this->ext[".webp "] = "image/webp";
	this->ext[".woff"] = "font/woff";
	this->ext[".woff2"] = "font/woff2";
	this->ext[".xhtml"] = "application/xhtml+xml";
	this->ext[".xls"] = "application/vnd.ms-excel";
	this->ext[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	this->ext[".xml"] = "application/xml";
	this->ext[".xul"] = "application/vnd.mozilla.xul+xml";
	this->ext[".zip"] = "application/zip";
	this->ext[".3gp"] = "video/3gpp";
	this->ext[".3g2"] = "video/3gpp2";
	this->ext[".7z"] = "application/x-7z-compressed";
	this->error[200] = "OK";
	this->error[204] = "No Content";
	this->error[400] = "Bad Request";
	this->error[401] = "Unauthorized";
	this->error[403] = "Forbidden";
	this->error[404] = "Not Found";
	this->error[500] = "Internal Server Error";
}

Settings::~Settings()
{
}

void	Settings::build(int ke)
{
	static struct kevent	change;
	int						socket_fd;
	struct addrinfo			serv_addr, *res;
	std::vector<std::string>	all_ports;

	this->config.selectFirstServ();
	for(unsigned int i = 0; i < this->config.getServNumb(); i++)
	{
		if (std::find(all_ports.begin(), all_ports.end(), this->config.getPort()) == all_ports.end())
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
			if (listen(socket_fd, 1024) == -1)
				throw Settings::badCreation();
			EV_SET(&change, socket_fd, EVFILT_READ , EV_ADD | EV_ENABLE, 0, 0, 0);
			if (kevent(ke, &change, 1, NULL, 0, NULL) == -1)
					throw Settings::badCreation();
			all_ports.push_back(this->config.getPort());
		}
		++config;
	}
}



std::string Settings::date(void)
{
	time_t				tmm = time(0);
	std::ostringstream	r;
	r << std::put_time(localtime(&tmm), "date: %a, %d %b %G %T GMT\n");
	return(r.str());
}

std::string Settings::generate_header()
{

}

std::string Settings::generate_cookie(Sbuffer &client, struct sockaddr_in const& client_addr)
{

}

void Settings::generate_body(Sbuffer &client, struct sockaddr_in const& client_addr)
{
	std::fstream		fd;

	client.status_code = 200;
	if ((client._cgi_process_launched == true && client._body_ready == false) || (config.getCgi(client._req.method.path, yd::getExtension(client._req.method.path)) != NULL))
	{
		CGI::execute_cgi(this->config, client, client_addr);
		if (client._body_ready == false )
			return ;
		yd::extractHeader(header_script, _body);
		if (_body.size() == 0)
			client.status_code = 204;
		else if (strcmp(header_script.c_str(), "Status: 500") == 0)
			client.status_code = 500;
	}
	else
	{
		char *file = (char *)this->config.getFile(client._req.method.path)->c_str();
		if(file[0] == '/')
			file++;
		fd.open(file, std::fstream::in | std::fstream::out);
		if (!fd.is_open())
		{
			if (!this->checkextension(client._req.method.path).empty())
				client.status_code = 404;
			else if (!this->config.getDirectoryListing(client._req.method.path).empty())
				folder_gestion(client);
		}
	}
	if (client.status_code != 200 && client.status_code != 204)
	{
		std::stringstream page;
		page << ERROR_BRUT_FOLDER;
		page << client.status_code;
		page << ".html";
		fd.open(*this->config.getError(client.status_code), std::fstream::in);
			if (!fd.is_open())
				fd.open(page.str().c_str(), std::fstream::in);
	}
	if (fd.is_open())
	{
		fd.seekg(0, std::ios::end);
		std::streampos fileSize = fd.tellg();
		fd.seekg(0, std::ios::beg);
		client._buffer.reserve(fileSize);
		fd.read(&client._buffer[0], fileSize);
		fd.close();
	}
	
}
// std::string Settings::get(Request const &req, struct sockaddr_in const& client_addr)
// {

// 	std::string header_script;
// 	std::stringstream	n;
// 	std::fstream		fd;
// 	std::string			tmp;
// 	std::string buffer;
// 	bool cgi_executed = false;

// 	if (!this->config.getMethod(req.method.path).isget)
// 	{
// 			this->method_not_allowed(req);
// 			return;
// 	}
// 	if (this->config.getFile(req.method.path)->empty())
// 	{
// 		_header.append(" 404 Not Found\n");
// 		fd.open(this->config.getError(404)->c_str());
// 		if (!fd.is_open())
// 			fd.open("http/404.html");
// 	}
// 	else
// 	{
// 		if (this->config.getCgi(req.method.path, yd::getExtension(req.method.path)) != NULL)
// 		{

// 			_header.append("200 OK\n");
// 			_body = CGI::execute_cgi(this->config, req, client_addr);
// 			yd::extractHeader(header_script, _body);
// 			cgi_executed = true;
// 		}
// 		else
// 		{
// 			char *file = (char *)this->config.getFile(req.method.path)->c_str();
// 			if(file[0] == '/')
// 				file++;
// 			fd.open(file, std::fstream::in | std::fstream::out);
// 			if (fd.is_open())
// 			{
// 				_header.append("200 OK\n");
// 			}
// 			else if (!this->checkextension(req.method.path).empty())
// 			{
// 				_header.append("404 Not Found\n");
// 				fd.open("http/404.html");
// 			}
// 			else if (!this->config.getDirectoryListing(req.method.path).empty())
// 			{
// 				folder_gestion(req);
// 				return;
// 			}
// 			while (getline(fd, tmp))
// 				buffer += tmp + "\n";
// 		}
// 	}
// 	_header += Settings::date();
// 	_header += "server: " + *this->config.getName() + "\n";
// 	std::string date = "";
// 	int count = 0;
// 	_header += handleCookie(req, date, count);
// 	n <<  _body.size() + buffer.size() + date.size() + std::to_string(count).size() + 17;
// 	if (!cgi_executed)
// 		_header += "Content-Length: " + n.str() + "\n";
// 	size_t pos = 0;
// 	if ((pos = header_script.find("Content-Type")) == std::string::npos)
// 		_header += "Content-Type: " + this->checkextension(*this->config.getFile(req.method.path)) + "\n" ;
// 	_header += "Connection: keep-alive";
// 	if (header_script.size() > 0)
// 		_header += '\n' + header_script;
// 	else
// 		_header += "\r\n\r\n";
// 	_header += buffer;
// 	_cookie += "nb de refres : " + std::to_string(count) + ", ";
// 	if (date.size() > 0)
// 		_cookie += date;
// 	if (_body.size() == 0)
// 		_header += "\n";
// 	if (header_script.find("Content-Length") == std::string::npos)
// 		_add_eof = 1;
// 	fd.close();
// }

void Settings::recoverBody()
{
		if (this->config.getCgi(req.method.path, yd::getExtension(req.method.path)) != NULL)
		{
			_body = CGI::execute_cgi(this->config, req, client_addr);
			yd::extractHeader(_header_script, _body);
		}
		else
}

// void Settings::post(Request const &req, struct sockaddr_in const& client_addr)
// {
// 	std::stringstream header;
// 	std::string header_script;
// 	std::fstream fd;

// 	header << "HTTP/1.1 ";
// 	if (!this->config.getMethod(req.method.path).ispost)
// 	{
// 		this->method_not_allowed(req);
// 		return ;
// 	}
// 	fd.open(this->config.getFile(req.method.path)->c_str(), std::fstream::in);
// 	if (this->config.getCgi(req.method.path, yd::getExtension(req.method.path)) != NULL)
// 	{
// 		_body = CGI::execute_cgi(this->config, req, client_addr);
// 		yd::extractHeader(header_script, _body);
// 	}
// 	else if (!fd.is_open())
// 	{
// 		fd.open(this->config.getError(404)->c_str(), O_RDONLY);
// 		if (!fd.is_open())
// 			fd.open("http/404.html", O_RDONLY);
// 		header << "404 Not Found\n";
// 	}
// 	if (_body.size() == 0)
// 		header << "204 No Content\n";
// 	else if (strcmp(header_script.c_str(), "Status: 500") == 0)
// 		header << "500 Internal Server Error\n";
// 	else
// 		header << "200 OK " << "\n";
// 	header << Settings::date();
// 	header << "server: " << *this->config.getName() << "\n";
// 	header << "Content-Length: " << _body.size() << "\n";
// 	std::string::size_type pos = 0;
// 	if ((pos = header_script.find("Content-Type")) == std::string::npos)
// 		header << "Content-Type: " << this->checkextension(*this->config.getFile(req.method.path)) << "\n" ;
// 	header << "Connection: keep-alive";
// 	std::cout << "VALEUR HEADER SCRIPT : " << header_script << std::endl;
// 	if (header_script.size() > 0)
// 		header << '\n' << header_script;
// 	else
// 		header << "\r\n\r\n";
// 	this->_header = header.str();
// 	// std::cout << rvalue_script << std::endl;
// 	pos = 0;
// 	if ((pos = header_script.find("Content-Lenght")) == std::string::npos)
// 		_add_eof = 1;
// 	fd.close();
// }


void Settings::del(Sbuffer &client)
{
	if (!this->config.getMethod(client._req.method.path).isdelete)
	{
		client.status_code = 405;
		return ;
	}
	string path_file = this->config.getFile(client._req.method.path)->c_str();
	std::ifstream infile(path_file);
	if (infile.good()) {
		std::cout << "Le fichier existe" << std::endl;
		if (remove(path_file.c_str()) != 0)
		{
			client.status_code = 403;
			perror("Error deleting file");
		}
		else
		{
			client.status_code = 204;
			puts("File successfully deleted");
		}
	}
	else
	{
		client.status_code = 404;
	}
}

size_t Settings::reading_header(int socket, unsigned int &readed, time_t &time_starting, char *buff)
{
	cout << "READING HEADER" << endl;
	int o_read = 0;
	time(&time_starting);

	char tmp[2];
	std::memset(tmp, 0, 2);
	std::memset(buff, 0, 4097);
	stringstream sbuffer;
	
	o_read = recv(socket, tmp, 1, 0);
	readed ++;
	if (o_read == -1 || o_read == 0)
		return(o_read);
	sbuffer << tmp;
	while (sbuffer.str().find("\r\n\r\n") == string::npos)
	{
		o_read = recv(socket, tmp, 1, 0);
		readed ++;
		if (o_read == -1 || o_read == 0)
			break;
		sbuffer << tmp;
		// cout << tmp;
	}
	strcpy(buff, sbuffer.str().c_str());
	return(o_read) ;
}

size_t Settings::reading(int socket, unsigned int &readed, time_t &time_starting, char *buff)
{
	cout << "READING" << endl;
	size_t	o_read = 0;
	time(&time_starting);
	std::memset(buff, 0, 4097);
	
	o_read = recv(socket, buff, 4096, 0);
	if (o_read == (size_t)-1 || o_read == (size_t)0)
		return(o_read);
	readed += o_read;
	return(o_read) ;
}

char* Settings::reading_chunck(int socket, unsigned int &readed, time_t &time_starting)
{
	cout << "READING CHUNCK" << endl;
	int o_read = 0;
	time(&time_starting);

	char tmp[2];
	char tmp_purge[2];
	std::memset(tmp, 0, 2);
	std::memset(tmp_purge, 0, 2);
	stringstream ssize;
	stringstream sbuffer;
	
	o_read = recv(socket, tmp, 1, 0);
	cout << "tmp[0] : " << "'" << tmp << "'"  << endl;
	if (o_read == -1 || o_read == 0)
		return(NULL);
	ssize << tmp;

	// purge
	if (tmp[0] == '\r')
	{
		o_read = recv(socket, tmp, 1, 0);
		if (o_read == -1 || o_read == 0)
			return(NULL);
	}

	while (ssize.str().find("\r\n") == string::npos )
	{
		o_read = recv(socket, tmp, 1, 0);
		if (o_read == -1 || o_read == 0)
			break;
		ssize << tmp;
	}
	if (ssize.str().empty())
		return (NULL);

	string::size_type x;
	std::stringstream ss;
	std::stringstream ssbody;
	ss << std::hex << ssize.str();
	ss >> x;
	long size = static_cast<string::size_type>(x);
	cout << "size : " << size << endl;
	if (size == 0)
		return (NULL);

	char *buff = new char[size + 1];
	std::memset(buff, 0, size + 1);

	o_read = recv(socket, buff, size, 0);
	readed += o_read;
	if (o_read == -1 || o_read == 0)
		return(NULL);
	printf("buff char * : '%s'\n", buff);
	return (buff);
}


bool Settings::writeResponse(Sbuffer &client, int socket)
{
	std::vector<char>::iterator start = _body.begin();
	size_t size_data = _body.size();
	if (client._header_sent == 0)
	{
		send(socket, client._header.c_str(), client._header.size(), 0);
		client._header_sent = 1;
	}
	if (client._total_sent < size_data) 
	{
		int sent = send(socket, &*start + client.total_sent, size_data - client.total_sent, 0);
		if (sent == -1) 
        	return (-1);
		else 
			client._total_sent += sent; 
    }

	if (client._total_sent == size_data)
		write(socket, client._cookie.c_str(), client._cookie.size());
	if (_add_eof)
		return (-1);
	return (client._req.header.connection);
}

bool Settings::parseRequest(Sbuffer &client)
{
	std::fstream fd;
	client._header = "HTTP/1.1 ";
	client._body_ready = 1;
	if (client._req.parseRequest(client._buffer))
		client._header = method_not_allowed(client._req);
	else if (!client._req.method.path.empty())
		fd.open(*this->config.getFile(client._req.method.path));
	else if (!this->config.selectServ(client._req.header.host_ip, client._req.header.port))
		 client._header = this->badRequest(client._req);
	else if (!this->checkmethod(client._req, this->config.getMethod(client._req.method.path)))
		 client._header = this->method_not_allowed(client._req);
	else if (check_forbidden(*this->config.getFile(client._req.method.path)) && checkextension(*this->config.getFile(client._req.method.path)).empty())
		client._header=  this->not_found();
	else if (client._req.method.isGet || client._req.method.isPost  || client._req.method.isDelete)
		client._body_ready = 0;
	else 
		this->badRequest(client._req);
}

// bool Settings::createResponse(Sbuffer &client, sockaddr_in const& client_addr)
// {
// 	client._buffer.clear();	
// 	if (client._req.method.isGet)
// 		client._header = this->get(client._req, client_addr);
// 	else if (client._req.method.isPost)
// 		client._header = this->post(client._req, client_addr);
// 	else if (client._req.method.isDelete)
// 		client._header = this->del(client._req, client_addr);
// }

std::string	Settings::checkextension(std::string const& path)
{
	std::cout << "PATH : " << path << std::endl;
	size_t pos = path.rfind('.');
	
	if (pos != path.npos)
	{
		std::map<std::string, std::string>::iterator start = this->ext.begin();
		std::map<std::string, std::string>::iterator end = this->ext.end();
		std::string extension = path.substr(pos);
		cout << "extension : " << extension << endl;
		while(start != end)
		{
			if (start->first == extension){
				cout << "start first : " << start->second << endl;
				return(start->second);
			}
			start++;
		}
		return(std::string(""));
	}
	return (std::string());
}



void Settings::folder_gestion(Sbuffer &client)
{
	std::stringstream	buffer;

	DIR *dir = opendir(this->config.getDirectoryListing(client._req.method.path).c_str());
	if (dir)
	{
		buffer << "<html><body><ul>";
		struct dirent *entry;
		while ((entry = readdir(dir)) != nullptr)
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				buffer << "<li>" + std::string(entry->d_name) << "</li>\n";
		buffer << "</ul></body></html>\r\n";
		closedir(dir);
		const char *start = buffer.str().c_str(); 
		client._buffer.insert(client._buffer.begin(), start, start + buffer.str().size());
	}
	else
		client.status_code = 404;
}




bool Settings::checkmethod(Request const& req, Methods const& t)
{
	if (req.method.isPost && t.ispost)
		return (true);
	else if (req.method.isGet && t.isget)
		return (true);
	else if (req.method.isDelete && t.isdelete)
		return (true);
	return (false);
}


void	Settings::set_event(int ke, int socket, short filter, short flag)
{
	struct kevent changeEvent;
	EV_SET(&changeEvent, socket, filter, flag, 0, 0, nullptr);
	if (kevent(ke, &changeEvent, 1, nullptr, 0, &this->check_request_timeout) == -1)
		std::cerr << "Could not add client " << socket <<" socket to kqueue" << std::endl;
	
}

std::string Settings::handleCookie(const Request &req, std::string &date, int &count) {
	std::string cookie = "";
	static map<string, string> sessions_date;
	static map<string, int> sessions_count;

	Header::t_cookie_it it = req.header.cookies.find("wsid");
	if (it == req.header.cookies.end()) {
		string sessionId = yd::generateSessionId();
		cookie = "Set-Cookie: ";
		cookie += "wsid=";
		cookie += sessionId;
		cookie += "\r\n";
		
		sessions_date[sessionId] = this->date();
		sessions_count[sessionId]++;
	} 
	else {
		date = sessions_date[req.header.cookies.at("wsid")];
		sessions_date[req.header.cookies.at("wsid")] = this->date();
		
		sessions_count[req.header.cookies.at("wsid")]++;
		count = sessions_count[req.header.cookies.at("wsid")];
	}
	return cookie;
}


/********************************************/
/*											*/
/*				ERROR FONC					*/
/*											*/
/********************************************/


int Settings::checkArgs(int argc)
{
	if (argc < 2) {
		std::cerr << RED << "WebServ$> Bad argument: please enter the path of the configuration file." << DEF << std::endl;
		return (1);
	}
	if (argc > 3) {
		std::cerr << RED << "WebServ$> Bad argument: please enter only the path of the configuration file." << DEF << std::endl;
		return (1);
	}
	return (0);
}

void		Settings::check_timeout(Sbuffer *requests, int ke, std::map<int, sockaddr_in>& clients)
{
	time_t actual_time;
	time(&actual_time);
	for (int i = 0; i < MAX_REQUESTS; i++)
	{
		if (requests[i].readed != 0)
		{
			std::cout << "READED : " <<requests[i].readed << std::endl; 
		}	
		if (requests[i].readed != 0 && difftime(actual_time, requests[i].time_start) > 2)
		{
			std::cout << "DIFF TIME " << difftime(actual_time, requests[i].time_start) << std::endl;
			usleep(1000);
			this->set_event(ke, i, EVFILT_READ, EV_DELETE);
			this->set_event(ke, i, EVFILT_WRITE, EV_ADD);
			std::string rep(this->timeout());
			write(i, rep.c_str(), rep.size());
			this->set_event(ke, i, EVFILT_WRITE, EV_DELETE);
			clients.erase(i);
			close(i);
			std::cout << "CLOSE\n";
		}
	}
}



int Settings::check_forbidden(std::string const& path)
{
	struct stat info;

	int i = 0;
	if (path.c_str()[0] == '/')
		i = 1;
	if (stat(path.c_str() + i, &info) != 0)
		return(1);
	if (S_ISDIR(info.st_mode))
		return(1);
	else
		return(0);
}


std::string Settings::not_found( void )
{
	std::fstream		file("http/404.html");
	std::stringstream	contentefile;
	std::stringstream	reponse;
	
	if (file.is_open())
		contentefile << file.rdbuf();
	reponse << "HTTP/1.1 404 Not Found\n";
	reponse << this->date();
	reponse << "server: " << *this->config.getName() << "\n";
	reponse << "Content-Type: text/html\n";
	if (file.is_open())
	{
		reponse << "Content-Length: " << contentefile.str().size() << "\n\n";
		reponse << contentefile.str() << "\n";
	}
	else
		reponse << "Content-Length: 0\n\n"; 
	return(reponse.str());
}



std::string Settings::Unauthorized( void )
{
	std::fstream		file("http/401.html");
	std::stringstream	contentefile;
	std::stringstream	reponse;
	
	if (file.is_open())
		contentefile << file.rdbuf();
	reponse << "HTTP/1.1 401 Unauthorized\n";
	reponse << this->date();
	reponse << "server: " << *this->config.getName() << "\n";
	reponse << "Content-Type: text/html\n";
	if (file.is_open())
	{
		reponse << "Content-Length: " << contentefile.str().size() << "\n\n";
		reponse << contentefile.str() << "\n";
	}
	else
		reponse << "Content-Length: 0\n\n"; 
		return(reponse.str());
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

	reponse << Settings::date();
	reponse << "server: " << *this->config.getName() + "\n";
	reponse << "Content-Length: 25\n";
	reponse << "Connection: keep-alive\n";
	reponse << "\nMethod Not Allowed (405)\n";
	return(reponse.str());
}



std::string Settings::forbidden_error( void )
{
	std::fstream		file("http/403.html");
	std::stringstream	contentefile;
	std::stringstream	reponse;
	
	if (file.is_open())
		contentefile << file.rdbuf();
	reponse << "HTTP/1.1 403 Forbidden\n";
	reponse << this->date();
	reponse << "server: " << *this->config.getName() << "\n";
	reponse << "Content-Type: text/html\n";
	if (file.is_open())
	{
		reponse << "Content-Length: " << contentefile.str().size() << "\n\n";
		reponse << contentefile.str() << "\n";
	}
	else
		reponse << "Content-Length: 0\n\n"; 
		return(reponse.str());
}



std::string Settings::badRequest(Request const& req)
{
	std::stringstream	reponse;
	
	reponse << "HTTP/1.1 400 Bad Request\n";
	reponse << Settings::date();
	reponse << "server: " << *this->config.getName() + "\n";
	reponse << "Content-Length: 21\n";
	reponse << "Connection: keep-alive\n";
	reponse << "\nMalformed ";
	if (req.method.isGet)
		reponse << "GET";
	else if (req.method.isPost)
		reponse << "POST";
	reponse << " request\n";
	return(reponse.str());
}

std::string Settings::timeout( void )
{
	std::stringstream	reponse;
	std::fstream		fd;

	reponse << "HTTP/1.1 408 Timeout\n";
	reponse << Settings::date();
	reponse << "server: " << *this->config.getName() + "\n";
	reponse << "Connection: close\n";
	if (this->config.getError(408))
		fd.open(this->config.getError(408)->c_str());
	if (fd.is_open())
	{
		std::stringstream t;
		t << fd.rdbuf();
		reponse << "Content-Length: " << t.str().size() << "\n\n";
		reponse << t.str() << "\n";
	}
	else
	{
		reponse << "Content-Length: 17\n\n";
		reponse << "Request Timeout\n";
	}
	return(reponse.str());
}