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
	this->error[200] = "200 OK";
	this->error[204] = "204 No Content";
	this->error[400] = "400 Bad Request";
	this->error[401] = "401 Unauthorized";
	this->error[403] = "403 Forbidden";
	this->error[404] = "404 Not Found";
	this->error[413] = "413 Payload Too Large";
	this->error[500] = "500 Internal Server Error";
}

Settings::~Settings()
{
}

Sbuffer::Sbuffer() : _req(), readed(), time_start(), is_chunked(), status_code(200), _add_eof(),_cgi_data(), _pid(),_status(), _total_sent()
{

}
void Sbuffer::clean()
{
	this->_req.reset();
	this->readed = 0;
	this->time_start = 0;
	this->is_chunked = false;
	this->status_code = 200;
	this->_add_eof = false;
	this->_cgi_data.~CGI();
	this->_buffer.clear();
	this->header_script.clear();
	this->_pid = 0;
	this->_status = WAITNG_FOR_REQUEST;
	this->_total_sent = 0;
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
	r << std::put_time(localtime(&tmm), "date: %a, %d %b %G %T GMT");
	return(r.str());
}

void Settings::generate_header(Sbuffer &client)
{
	std::stringstream header;

	// std::cout << "generate_header\n";
	header << "HTTP/1.1 " << this->error[client.status_code];
	header << "\n" << this->date();
	if (client.header_script.find("Content-Type") == std::string::npos)
		header << "\n" << "Content-Type: " << this->checkextension(*this->config.getFile(client._req.method.path)) ;
	header << handleCookie(client);
	if (client.header_script.find("Content-Length") == std::string::npos && (client._pid == 0))
		header << "\n" << "Content-Length: " << (client._buffer.size() + client._body_cookie.size());
	if (client.header_script.find("Content-Length") == std::string::npos && client._pid  != 0)
 		client._add_eof = 1;
	if (client.header_script.size() > 0)
		header << client.header_script;
 	else
 		header << "\r\n\r\n";
	client._header = header.str();
	client._status = HEADER_GENERATED;
	// cout << "header : " << client._header << endl;
	//MEMO ICI
}

std::string Settings::handleCookie(Sbuffer &client) {
	std::string header_cookie = "";
	
	static map<string, string> sessions_date;
	static map<string, int> sessions_count;
	
	Header::t_cookie_it it = client._req.header.cookies.find("wsid");
	if (it == client._req.header.cookies.end()) {
		string sessionId = yd::generateSessionId();
		header_cookie = "\nSet-Cookie: wsid=";
		header_cookie += sessionId;
		sessions_date[sessionId] = this->date();
		sessions_count[sessionId]++;
	} 
	else {
		client._body_cookie = sessions_date[client._req.header.cookies.at("wsid")];
		sessions_date[client._req.header.cookies.at("wsid")] = this->date();
		
		sessions_count[client._req.header.cookies.at("wsid")]++;
		client._body_cookie += sessions_count[client._req.header.cookies.at("wsid")];
		client._body_cookie += "\n";
	}
	return (header_cookie);
}

void Settings::generate_body(Sbuffer &client, struct sockaddr_in const& client_addr)
{
	std::fstream		fd;

	// std::cout << "generate_body\n";
	if (client.status_code == 200 && ((client._status == CGI_PROCESS_LAUNCHED) || (config.getCgi(client._req.method.path, yd::getExtension(client._req.method.path)) != NULL)))
	{
		CGI::execute_cgi(this->config, client, client_addr);
		if (client._status == CGI_PROCESS_LAUNCHED)
			return ;
		yd::extractHeader(client.header_script, client._buffer);
		if (client._buffer.size() == 0)
			client.status_code = 204;
		else if (strcmp(client.header_script.c_str(), "Status: 500") == 0)
			client.status_code = 500;
	}
	else if (client.status_code == 200)
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
		if (this->config.getError(client.status_code) != NULL)
		{
		fd.open(*this->config.getError(client.status_code), std::fstream::in);
			if (!fd.is_open())
				fd.open(page.str().c_str(), std::fstream::in);
		}
		else 
			fd.open(page.str().c_str(), std::fstream::in);
			if (!fd.is_open())
			{
				client._buffer.clear();
				client._buffer.insert(client._buffer.begin(), error[client.status_code].c_str(), error[client.status_code].c_str() + error[client.status_code].size());
			}	
	}
	if (fd.is_open())
	{
		client._buffer.clear();
		fd.seekg(0, std::ios::end);
		std::streampos fileSize = fd.tellg();
		fd.seekg(0, std::ios::beg);
		client._buffer.resize(fileSize);
		fd.read(&*(client._buffer.begin()), fileSize);
		fd.close();
	}
	client._status = BODY_GENERATED;
}

void	Settings::gestion_413(Sbuffer &client, int socket)
{
	// std::cout << "gestion 413\n";
	char tmp_buff[409700];
	client._add_eof = 1;
	usleep(100000);
	int o_read_p = recv(socket, &tmp_buff, 409700, MSG_DONTWAIT);
	if (o_read_p < 0)
	// the error EAGAIN and EWOULDBLOCK appears when we attempt to recv a empty socket with a O_NONBLOCK flag, so it's a normal error
	if (o_read_p == 0 || (o_read_p < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))) {
		client._status = REQUEST_RECEIVED;
}
	if (o_read_p < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
		std::cerr << "Erreur : " << strerror(errno) << std::endl;
}
}

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
		client.status_code = 404;
}

size_t Settings::reading_header(int socket, unsigned int &readed, time_t &time_starting, char *buff)
{
	// cout << "READING HEADER" << std::endl;
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
	}
	strcpy(buff, sbuffer.str().c_str());
	return(o_read) ;
}

size_t Settings::reading(int socket, unsigned int &readed, time_t &time_starting, char *buff)
{
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
	int o_read = 0;
	time(&time_starting);

	char tmp[2];
	char tmp_purge[2];
	std::memset(tmp, 0, 2);
	std::memset(tmp_purge, 0, 2);
	stringstream ssize;
	stringstream sbuffer;
	
	o_read = recv(socket, tmp, 1, 0);
	if (o_read == -1 || o_read == 0)
		return(NULL);
	ssize << tmp;

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


void Settings::writeResponse(Sbuffer &client, int socket)
{
	usleep(1000);
	std::vector<char>::iterator start = client._buffer.begin();
	size_t size_data = client._buffer.size();
	if (client._status == BODY_SENT)
		return ;
	if (client._status == HEADER_GENERATED)
	{
		send(socket, client._header.c_str(), client._header.size(), 0);
		client._status = HEADER_SENT;
	}
	if (client._total_sent < size_data) 
	{
		int sent = send(socket, &*start + client._total_sent, size_data - client._total_sent, 0);
		client._total_sent += sent; 
    }
	if (client._total_sent == size_data)
	{
		send(socket, client._body_cookie.c_str(), client._body_cookie.size(), 0);
		client._status = BODY_SENT;
	}
}

void Settings::parseRequest(Sbuffer &client)
{
	std::fstream fd;
	client._status = REQUEST_PARSED;
	if (client.status_code == 413)
	{
		client._status = REQUEST_PARSED;
		return; 
	}
	else if (client._req.parseRequest(client._buffer))
		client.status_code = 405;
	else if (!this->config.selectServ(client._req.header.host_ip, client._req.header.port))
		client.status_code = 400;
	else if (!this->checkmethod(client._req, this->config.getMethod(client._req.method.path)))
		client.status_code = 405;
	else if (check_forbidden(*this->config.getFile(client._req.method.path)) && checkextension(*this->config.getFile(client._req.method.path)).empty())
		client.status_code = 404;
	else if (client._req.method.isGet || client._req.method.isPost || client._req.method.isDelete)
	{
		if (client._req.method.isDelete)
			this->del(client);
		if (!client._req.header.connection)
			client._add_eof = 1;
		return ;
	}
	else 
		client.status_code = 400;
	if (!client._req.header.connection)
		client._add_eof = 1;
	client._status = REQUEST_PARSED;
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
	size_t pos = path.rfind('.');
	
	if (pos != path.npos)
	{
		std::map<std::string, std::string>::iterator start = this->ext.begin();
		std::map<std::string, std::string>::iterator end = this->ext.end();
		std::string extension = path.substr(pos);
		// cout << "extension : " << extension << std::endl;
		while(start != end)
		{
			if (start->first == extension){
				// cout << "start first : " << start->second << std::endl;
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
		string strbuffer = buffer.str();
		const char *start = strbuffer.c_str();
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

void		Settings::check_timeout(std::map<int, Sbuffer> &requests, int ke, std::map<int, sockaddr_in>& clients)
{
	time_t actual_time;
	time(&actual_time);
	std::map<int, Sbuffer> ::iterator start = requests.begin();
	for (int i = 0; start != requests.end(); start++)
	{
		if ((*start).second.readed != 0 && difftime(actual_time, (*start).second.time_start) > 2)
		{
			usleep(1000);
			this->set_event(ke, (*start).first, EVFILT_READ, EV_DELETE);
			this->set_event(ke, (*start).first, EVFILT_WRITE, EV_ADD | EV_ENABLE);
			std::string rep(this->timeout());
			send((*start).first, rep.c_str(), rep.size(), 0);
			this->set_event(ke, (*start).first, EVFILT_WRITE, EV_DELETE);
			clients.erase(i);
			close(i);
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


bool Settings::reqIsChuncked(std::string req)
{
	std::string::size_type pos = req.find("Transfer-Encoding: chunked");
	if (pos != std::string::npos)
		return (true);
	return (false);
}

void Settings::reading_request(Sbuffer &sbuffer, Settings &server, int ke, uintptr_t ident, Request &req)
{
	char buffer[4097];
	char header_buffer[4097];
	memset(buffer, 0, 4097);
	memset(header_buffer, 0, 4097);
	size_t readed = 0;
	size_t header_readed = 0;

	// std::cout << "READ\n";
	if (!this->reqIsChuncked(header_buffer) && sbuffer.is_chunked == false && sbuffer._buffer.size() == 0)
	{
		header_readed = server.reading_header(ident, sbuffer.readed, sbuffer.time_start, header_buffer);
		if (req.check_header_buffer(header_buffer, server.config))
		{
			sbuffer.status_code = 413;
			server.set_event(ke, ident, EVFILT_READ, EV_DELETE);
			server.set_event(ke, ident, EVFILT_WRITE, EV_ADD | EV_ENABLE);
			return;
		}
		for (unsigned long j = 0; j < sbuffer.readed; j++)
			sbuffer._buffer.push_back(header_buffer[j]);
	}
	if (reqIsChuncked(header_buffer) == true)
		sbuffer.is_chunked = true;

	if (sbuffer.is_chunked == true)
	{
		char *chunck_buffer = NULL;
		sbuffer.readed = 0;
		chunck_buffer = server.reading_chunck(ident, sbuffer.readed, sbuffer.time_start);

		if (chunck_buffer == NULL)
		{
			server.set_event(ke, ident, EVFILT_READ, EV_DELETE);
			server.set_event(ke, ident, EVFILT_WRITE, EV_ADD | EV_ENABLE);
			sbuffer._buffer.push_back('\r');
			sbuffer._buffer.push_back('\n');
			sbuffer._buffer.push_back('\r');
			sbuffer._buffer.push_back('\n');
			sbuffer.is_chunked = false;
		}
		else
		{
			cout << "sbuffer.read : " << sbuffer.readed << endl;
			for (unsigned long j = 0; j < sbuffer.readed; j++)
				sbuffer._buffer.push_back(chunck_buffer[j]);
			
			std::vector<char>::iterator it = sbuffer._buffer.begin();
			cout << "the buffer : \n";
			for(; it != sbuffer._buffer.end(); it++)
				std::cout << *it;
			cout << "\n\n";

		}
	}
	else
	{
		// check si il y a un body
		std::string header(header_buffer);
		std::string::size_type pos = header.find("Content-Length");
		if (header.size() > 0 && pos == std::string::npos)
		{
			// cout << "FINISHED" << std::endl;
			server.set_event(ke, ident, EVFILT_READ, EV_DELETE);
			server.set_event(ke, ident, EVFILT_WRITE, EV_ADD | EV_ENABLE);
			// break;
		}
		else {
			readed = server.reading(ident, sbuffer.readed, sbuffer.time_start, buffer);
			for (unsigned long j = 0; j < readed; j++)
				sbuffer._buffer.push_back(buffer[j]);

			if (req.isFinishedRequest(sbuffer._buffer, sbuffer.readed))
			{
				// cout << "FINISHED" << std::endl;
				server.set_event(ke, ident, EVFILT_READ, EV_DELETE);
				server.set_event(ke, ident, EVFILT_WRITE, EV_ADD | EV_ENABLE);
			}
			
		}
	}
	sbuffer._status = REQUEST_RECEIVED;
}