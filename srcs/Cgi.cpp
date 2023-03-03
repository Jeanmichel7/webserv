/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 09:46:36 by ydumaine          #+#    #+#             */
/*   Updated: 2023/01/30 10:33:33y ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"


CGI::CGI():_body(), _env(),_arg(),_file_stdin(),_file_stdout(),_fd_stdin(), _fd_stdout() {}


void CGI::build(Config &conf, const Request &req,  struct sockaddr_in const &client_addr)
{
	//ACQUISITION DES RESSOURCE

	// RECUPERATION DE L'IP CLIENT
	socklen_t client_addr_len = sizeof(client_addr);

	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);

	// preparer les variables environnements 
	std::map<std::string, std::string> env;
	env["REQUEST_METHOD"] = req.method.type;
	env["QUERY_STRING"] = req.method.parameters;
	env["CONTENT_LENGTH"] = req.header.content_length ;
	env["CONTENT_TYPE"] = req.header.content_type;
	env["PATH_INFO"] = req.method.path;
	env["HTTP_USER_AGENT"] = req.header.str_user_agent;
	env["REMOTE_ADDR"] = client_ip;
	env["REMOTE_HOST"] = req.header.host;
	env["REMOTE_USER"] = "";
	env["REMOTE_PASSWORD"] = "";
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "WebServ/0.1";
	env["SERVER_NAME"] = *conf.getName();
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["SCRIPT_NAME"] = req.method.path;

	Header::t_cookie_it it = req.header.cookies.begin();
	Header::t_cookie_it ite = req.header.cookies.end();
	std::string cookie;
	for (; it != ite; it++) { cookie += it->first + "=" + it->second + ";"; }
	env["HTTP_COOKIE"] = cookie;

	
	// Transformer la map en char ** (excve prend un char **)
	_env = new char*[env.size() + 1];
	if (_env == NULL)
		throw std::exception();
	std::map<std::string, std::string>::iterator start = env.begin();
	std::map<std::string, std::string>::iterator end = env.end();
	int i = 0;
	for (; start != end;start++) 
	{
		std::string el = (start->first + "=" + start->second).c_str();
		_env[i] = new char[el.size() + 1];
		if (_env[i] == NULL)
			throw std::exception();
		_env[i] = strcpy(_env[i], el.c_str());
		++i;
	}
	_env[i] = NULL;

	// ouvrir 2 fichiers, un pour entre standard
	// et un pour la sortie standard
	/* au debut j'avais fais qu'un fichier sauf que quand le script ecrivait pas sur la sortie standard j'avais
	 comme retour de mon programme un fichier avec l'entree standard lol, il est possible que le script n'ecrive pas 
	 sur la sortie standard quand par exemple le script lancer par execve n'existe pas, donc execve ne fait rien */
	_file_stdin = std::tmpfile();
	_file_stdout = std::tmpfile();
	if (_file_stdin == NULL || _file_stdout == NULL)
		throw std::exception();
	_fd_stdin = fileno(_file_stdin);
	_fd_stdout = fileno(_file_stdout);

	// creer un arg pour l'envoyer au programme (obligatoire et excve prend un char**)
	// l'arg est le chemin du fichier
	size_t length = conf.getFile(req.method.path)->length();
	this->_arg = new char *[2];
	if (this->_arg == NULL)
		throw std::exception();
	this->_arg[0] = new char[length + 1];
	if (this->_arg[0] == NULL)
		throw std::exception();
	this->_arg[0][length] = 0;
	conf.getFile(req.method.path)->copy(_arg[0], length);
	this->_arg[1] = NULL;
}

CGI::~CGI()
{
	if (_env != NULL)
	{
		for (int i = 0; _env[i] != NULL; i++)
			delete[] _env[i];
		delete[] _env;
	}
	if (_arg != NULL)
	{
		if (_arg[0] != NULL)
			delete _arg[0];
		delete[] _arg;
	}
	if (_file_stdin != NULL)
		fclose(_file_stdin);
	if (_file_stdout != NULL)
		fclose(_file_stdin);
	if (_fd_stdin != 0)
		close(_fd_stdin);
	if (_fd_stdout != 0)
		close(_fd_stdout);
}

std::vector<char> CGI::execute_cgi(Config &config, const Request &req, struct sockaddr_in const &client_addr)
{	//declarer variable
	CGI data;
	std::vector<char> return_value;


	string scriptName;

	// On extrait le nom du script pour ensuite pouvoir checher l'extension adequat

	// On essaye d'allouer les ressources necessaires
	try 
	{
		data.build(config, req, client_addr);
	}
	catch (std::exception &e)
	{
		return(error_500());
	}
	std::vector<char>::iterator start = req.body.vector_body->begin();
	size_t size_data = req.body.vector_body->size();
	write(data._fd_stdin, &*start, size_data);
	fseek(data._file_stdin, 0, SEEK_SET);
	
	// on cherche l'extension adequate
	string ext = yd::getExtension(req.method.path);
	if (ext == "")
		return(error_404());
	// pos = std::strchr(scriptName.c_str(), '.');
	std::string const *cgi_path_str = config.getCgi(req.method.path, ext);
	// si pas de cgi_path, on retourne NULL
	if (cgi_path_str == NULL)
		return(error_404());
	// creer le fork 
	const char *cgi_path = cgi_path_str->c_str();
	pid_t pid = fork();
	if (pid == -1)
			return (error_500());
	 if (pid == 0)
	 {
		// erase stdin and stdout
		// std::cout << "VALEUR DE data._arg[0] : " << data._arg[0] << std::endl;
		dup2(data._fd_stdin, 0);
		dup2(data._fd_stdout, 1);
		// executer l'interpreteur avec le programme et le nom de l'argument
		// l'emplacement relatif de l'executable correspond au programme qui utilise execve et pas celui du binaire appele. 
		execve(cgi_path, data._arg, data._env);
		std::cerr << "\e[0;31mWebServ$> " << "Execve has crashed " << "\e[0m" << std::endl;
	 }
	 else
	 {
		// on attends le processuce 
		int rt;
		waitpid(pid, &rt, 0);
		if (rt == 1)
			return(error_500());
		char buffer[1024];
		// on recupere le contenue du fichier
	  fseek(data._file_stdout, 0, SEEK_SET);
		for (int rt = 1023; rt == 1023;)
		{
			rt = fread(&buffer, sizeof(char), 1023, data._file_stdout);
			return_value.insert(return_value.end(), buffer, buffer + rt);
		}
	 }
		// on supprime l'environnement  cree
		return (return_value);
}

		std::vector<char> CGI::error_500()
		{
			std::vector<char> return_value;
			std::string error = "Status: 500\n\r\n\r";
			return_value.insert(return_value.end(), error.begin(), error.end());
			return (return_value);
		}
		std::vector<char> CGI::error_404(){
			std::vector<char> return_value;
			std::string error = "Status: 404\n\r\n\r";
			return_value.insert(return_value.end(), error.begin(), error.end());
			return (return_value);
		}