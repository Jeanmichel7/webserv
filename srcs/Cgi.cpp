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


CGI::CGI():_body(), _env(),_arg(),_tmpf(),_fd() {}


void CGI::build(std::string &scriptName, const Config &conf, const Request &req)
{
	string test(scriptName);
	//ACQUISITION DES RESSOURCES

	// preparer les variables environnements 
	std::map<std::string, std::string> env;
	env["REQUEST_METHOD"] = req.method.type;
	env["QUERY_STRING"] = req.method.parameters;
	env["CONTENT_LENGTH"] = req.header.content_length ;
	env["CONTENT_TYPE"] = req.header.content_type;
	env["PATH_INFO"] = req.method.path;
	env["HTTP_USER_AGENT"] = req.header.str_user_agent;
	env["REMOTE_ADDR"] = "78.211.126.94";
	env["REMOTE_HOST"] = req.header.host;
	env["REMOTE_USER"] = "";
	env["REMOTE_PASSWORD"] = "";
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "WebServ/0.1";
	env["SERVER_NAME"] = *conf.getName();
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	
	// Transformer la map en char ** (excve prend un char **)
	_env = new char*[env.size() + 1];
	std::map<std::string, std::string>::iterator start = env.begin();
	std::map<std::string, std::string>::iterator end = env.end();
	int i = 0;
	for (; start != end;start++) 
	{
		std::string el = (start->first + "=" + start->second).c_str();
		_env[i] = new char[el.size() + 1];
		_env[i] = strcpy(_env[i], el.c_str());
		++i;
	}
	_env[i] = NULL;

	// ouvrir un fichier
	_tmpf = std::tmpfile();
	if (_tmpf == NULL)
		throw std::exception();
	_fd = fileno(_tmpf);

	//creer un arg pour l'envoyer au programme  (obligatoire et excve prend un char **)
	size_t lenght = scriptName.size();
	this->_arg = new char*[2];
	this->_arg[0] = new char[lenght + 1];
	scriptName.copy(_arg[0],lenght);
	this->_arg[1] = NULL;
}

CGI::~CGI()
{
	if (_env != NULL)
	{
		for (int i = 0; _env[i] != NULL; i++)
			delete[] _env[i];
		_env = NULL;
		delete[] _env;
	}
	if (_arg != NULL)
		delete[] _arg;
	if (_tmpf != NULL)
		fclose(_tmpf);
	if (_fd != 0 && _fd != 1 && _fd != 2)
		close(_fd);
}

std::string CGI::execute_cgi(Config const &config, const Request &req)
{	//declarer variable
	std::string body;
	CGI data;


	string scriptName;
	string::size_type position = 0;

	if ((position = req.method.path.rfind("/")) != string::npos)
	{
		scriptName = req.method.path.substr(position + 1);
	}
	// cout << "script name : " << scriptName << endl;


	try 
	{
		data.build(scriptName, config, req);
	}
	catch (std::exception &e)
	{
		return ("Status: 500\n\r\n\r");
	}
	write(data._fd, req.body.content.c_str(), req.body.content.size());
	fseek(data._tmpf, 0, SEEK_SET);
	
	// on cherche l'extension adequate
	string ext = "";
	string::size_type dotpos;
	if ((dotpos = scriptName.rfind(".")) != string::npos)
		ext = scriptName.substr(dotpos);

	// pos = std::strchr(scriptName.c_str(), '.');
	std::string const *cgi_path_str = config.getCgi(req.method.path, ext);
	// si pas de cgi_path, on retourne NULL
	if (cgi_path_str == NULL)
		return ("Status: 404\n\r\n\r");

	// creer le fork 
	const char *cgi_path = cgi_path_str->c_str();
	pid_t pid = fork();
	if (pid == -1)
			return ("Status: 500\n\r\n\r");
	 if (pid == 0)
	 {
		// erase stdin and stdout
		// std::cout << "VALEUR DE data._arg[0] : " << data._arg[0] << std::endl;
		dup2(data._fd, 0);
		dup2(data._fd, 1);
		// executer l'interpreteur avec le programme et le nom de l'argument
		execve(cgi_path, data._arg, data._env);
		std::cerr << "\e[0;31mWebServ$> " << "Execve has crashed " << "\e[0m" << std::endl;
	 }
	 else
	 {
		// on attends le processuce 
		int rt;
		waitpid(pid, &rt, 0);
		if (rt == 1)
			return ("Status: 500\n\r\n\r");
		char buffer[1024];
		std::memset(&buffer, 0, 128);
		// on recupere le contenue du fichier
	  fseek(data._tmpf, 0, SEEK_SET);
		for (int rt = 1024; rt == 1024;)
		{
			rt = fread(&buffer, 8, 128, data._tmpf);
			body += buffer;
		}
	 }
		// on supprime l'environnement  cree

		return (body);
}