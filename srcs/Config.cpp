#include "Config.hpp"

/*
	const std::string &Config::getFile(const std::string &path) const
	{
		return ("~/webServ/website/index.html");
	}
	const Method	&Config::getMethod(const std::string &path) const{
		i.isget = true;
		i.ispost = true;
		i.isget = true;
		return(i);
	}
	const std::string &Config::getError(const unsigned int error) const
	{
		return ("~/webServ/website/404.html");
	}
	const std::string &Config::getCgi(const std::string &path, const std::string &cgi)
	{
		return ("usr/bin/php");
	}
	const unsigned int &Config::getMaxSize() const
	{
		return (10000000);
	}
	const std::string &Config::getUpload(const std::string &path) const
	{
		return ("/bonjour")
	}
	bool Config::getDirectoryListing(const std::string &path) const
	{
		return (true);
	}
	std::string Config::getName(const std::string &path) const
	{
		return (name);
	}
	*/
//--------------------------------------------------------------------------------------//
//                                       Location                                       //
//--------------------------------------------------------------------------------------//

bool yd::isPath(std::string const &s)
{
	bool hasAlphaNum = false;
	for (unsigned int i = 0; i < s.size(); i++) {
			if (isalnum(s[i])) {
					hasAlphaNum = true;
			}
			else if (s[i] != '/') {
					return false;
			}
			if (s[i] == '/' && s[i + 1] == '/') {
					return false;
			}
	}
	if (!hasAlphaNum) {
			return false;
	}
	return true;
}

	void Location::setCgi(Tokenizer &tok)
	{
		if (tok.getToken()[0] != '.')
			throw (FormatError(tok.getToken(), ".extension"));
		for (unsigned int i = 1; i < tok.getToken().length(); i++) 
		{
		if (!isalpha(tok.getToken()[i])) 
			throw (FormatError(tok.getToken(), ".extension"));
		}
		if (tok.nextToken()[0] == ';')
			throw (ConfigurationError("cgi need two arguments"));
		if (!yd::isPath(tok.getToken()))
			throw (FormatError(tok.getToken(), "path /x/x/x"));
		_cgi[tok.getTokenBefore()] = tok.getToken();
	}

	void Location::setGet(Tokenizer &tok)
	{
		if (tok.getToken() == "GET") 
			_is_get = 1;
		else if (tok.getToken() == "POST") 
			_is_post = 1;
		else if (tok.getToken() == "DELETE") 
			_is_delete = 1;
		else 
			throw (FormatError(tok.getToken(), "must be GET, POST or DELETE"));
	}

	void Location::setDefaultFile(Tokenizer &tok)
	{
		if (!yd::isPath(tok.getToken()))
			throw (FormatError(tok.getToken(), "path /x/x/x"));
		else 
			_default_file =  tok.getToken();
	}

	void Location::setDirectoryListing(Tokenizer &tok)
	{
		if (tok.getToken() == "on")
			_directory_listing = true;
		else if (tok.getToken() == "off")
			_directory_listing = false;
		else
			throw (FormatError(tok.getToken(), "must be \"on\" or \"off\" "));
	}

	void Location::setRoot(Tokenizer &tok)
	{
		if (!yd::isPath(tok.getToken()))
			throw (FormatError(tok.getToken(), "path /x/x/x"));
		else 
			_root =  tok.getToken();
	}

	void Location::setUploadFile(Tokenizer &tok)	
	{
		if (!yd::isPath(tok.getToken()))
			throw (FormatError(tok.getToken(), "path /x/x/x"));
		else 
			_default_file = tok.getToken();
	}


	Location::Location(): _upload_file(), _default_file(), _root(), _cgi(), _is_get(), 
	_is_post(), _is_delete(), _directory_listing()
	{
		_tokens["root"] = &Location::setRoot; 
		_tokens["cgi"] = &Location::setCgi; 
		_tokens["upload_file"] = &Location::setUploadFile; 
		_tokens["root"] = &Location::setRoot;
		_tokens["allow"] = &Location::setGet;
		_tokens["default_file"] = &Location::setDefaultFile; 
		_tokens["directory_lisiting"] = &Location::setDirectoryListing;
	}

		void (Location::*Location::selectSetter(std::string const &token))(Tokenizer &tok)
	{
		std::map<std::string , void (Location::*)(Tokenizer &tok)>::iterator start = _tokens.begin();
		std::map<std::string , void (Location::*)(Tokenizer &tok)>::iterator end = _tokens.end();
		for (;start != end; start++)
		{
			if (token == (*start).first)
				return (start->second);
		}
		return (NULL);
	}
//--------------------------------------------------------------------------------------//
//                                        Server                                        //
//--------------------------------------------------------------------------------------//

		void Server::setPort(Tokenizer &tok)
		{
				if (atoi(tok.getToken().c_str()) <= 0 || atoi(tok.getToken().c_str()) > 65535)
					throw (FormatError(tok.getToken(), "a number enter 1 and 65535 "));
				_port = atoi(tok.getToken().c_str());
		}

		void Server::setServerName(Tokenizer &tok)
		{
			for (unsigned int i = 0; i < tok.getToken().length(); i++) 
			{
				if (!isalpha(tok.getToken()[i])) 
					throw (FormatError(tok.getToken(), "alphanumeric characters"));
			}
			_server_name = tok.getToken();
		}

		void Server::setMaxBody(Tokenizer &tok)
		{
			if (atoi(tok.getToken().c_str()) <= 0 || atoi(tok.getToken().c_str()) > 2147483647)
				throw (FormatError(tok.getToken(), "positif numbers less than  2147483647" ));
			_max_body_size = atoi(tok.getToken().c_str());

		}

		void Server::setIp(Tokenizer &tok)
		{
			int dots = 0;
		for (unsigned int i = 0; i < tok.getToken().length(); i++) {
				if (tok.getToken()[i] == '.') {
						dots++;
				} else if (!isdigit(tok.getToken()[i])) {
					throw (FormatError(tok.getToken(), "Only numbers"));
				}
		}
		if (dots != 3) {
			throw (FormatError(tok.getToken(), "xxx.xxx.xxx.xxx"));
		}

		unsigned int num = 0;
		for (unsigned int i = 0; i < tok.getToken().length(); i++) {
				if (tok.getToken()[i] == '.') {
						if (num < 0 || num > 255) {
							throw (FormatError(tok.getToken(), "numbers between 0 and 255"));
						}
						num = 0;
				} else {
						num = num * 10 + (tok.getToken()[i] - '0');
				}
		}
		if (num < 0 || num > 255) {
				throw (FormatError(tok.getToken(), "numbers between 0 and 255"));
		}
		for (unsigned int i = 0; i < tok.getToken().length(); i++) {
				if (tok.getToken()[i] == '.') {
						continue;
				}
				num = num * 10 + (tok.getToken()[i] - '0');
		}
		_ip = num;
		}

		void Server::setError(Tokenizer &tok)
		{
			unsigned int code = atoi(tok.getToken().c_str());
			if (code < 100 || code > 600)
				throw (FormatError(tok.getToken(), "numbers between 100 and 600"));
			if (_error_pages[code].empty())
			{
				_error_pages[code] = tok.nextToken();
			}
			else
				throw (ConfigurationError("redefinition of error page"));
		}

	Server::Server(): _server_name(), _port(), _ip(), _max_body_size(), 
	_error_pages(), _locations(), _tokens() 
	{
	_tokens["server_name"] = &Server::setServerName;
	_tokens["max_body_size"] = &Server::setMaxBody; 
	_tokens["port"] = &Server::setPort; 
	_tokens["ip"] = &Server::setIp;
	}

		void (Server::*Server::selectSetter(std::string const &token))(Tokenizer &tok)
	{
		std::map<std::string , void (Server::*)(Tokenizer &tok)>::iterator start = _tokens.begin();
		std::map<std::string , void (Server::*)(Tokenizer &tok)>::iterator end = _tokens.end();
		for (;start != end; start++)
		{
			if (token == start->first)
				return (start->second);
		}
		return (NULL);
	}
		

	void Server::addLocation(Location location)
	{
		this->_locations.push_back(location);
	}

// Exception gestion for token
	Tokenizer::Unexpected::Unexpected(std::string token)  
	{
	std::cout << "\e[0;31mSyntax error near unexpected token`" + token + "\"\e[0m " << std::endl; 
	}

	FormatError::FormatError(std::string token, std::string expected_format)  
	{
	std::cout << "Format error \"" << token << "\"" << "Format expected : \"" << expected_format << "\"." << std::endl ; 
	}

	ConfigurationError::ConfigurationError(std::string message)
	{
		std::cout << "Configuration error : " <<  message << std::endl;
	}


//--------------------------------------------------------------------------------------//
//                                        Config                                        //
//--------------------------------------------------------------------------------------//


	Config::Config(std::string const &path)
	{
		Tokenizer tokenizer(*this, path);
	}
	

	void	Config::addServer(Server server)
	{
		this->_server.push_back(server);
	}

//--------------------------------------------------------------------------------------//
//                                        Tokenizer                                     //
//--------------------------------------------------------------------------------------//


	const std::string &Tokenizer::nextToken()
	{
		_token_before = _token;
		_file >> _token;
		return (_token);
	}

		const std::string &Tokenizer::getToken()
	{
		return (_token);
	}

		const std::string &Tokenizer::getTokenBefore()
	{
		return (_token_before);
	}

	Tokenizer::Tokenizer(Config &config, std::string const &path)
	{
		std::ifstream config_file;
		config_file.open(path, std::ios::in);
		if ( _file.is_open())
		{
			while (_file)
			{
				nextToken();
				if (_token == "server")
				{
					nextToken();
					if (_token == "{")
					{
						nextToken();
						config.addServer(parsServer());	
					}
					else
						throw Tokenizer::Unexpected(_token);
				}
				else 
					throw Tokenizer::Unexpected(_token);
			}
		}
		else
				throw std::runtime_error("Couldn't open config file");
		}

	Server	Tokenizer::parsServer()
	{
		Server server;
		void (Server::*pf)(Tokenizer &tok);
		while (_token != "}")
		{
			if (getToken() != ";")
					throw Tokenizer::Unexpected(_token);
			nextToken();
			if (_token == "location")
			{
				nextToken();
				if (_token == "{")
				{
					nextToken();
					server.addLocation(parsLocation());
				}
				else
					throw Tokenizer::Unexpected(_token);
			}
			else
			{
				pf = server.selectSetter(_token);
				if (pf == NULL)
					throw Tokenizer::Unexpected(_token);
				nextToken();
				if (getToken() == ";")
					throw (ConfigurationError(getTokenBefore() + "need at least one argument"));
				(server.*pf)(*this);
				nextToken();
			}
		}
		return (server);
	}

	Location Tokenizer::parsLocation()
	{
		Location location;
		void (Location::*pf)(Tokenizer &tok);
		while (_token != "}")
		{
			if (getToken() != ";")
					throw Tokenizer::Unexpected(_token);
			pf = location.selectSetter(_token);
			if (pf == NULL)
					throw Tokenizer::Unexpected(_token);
			nextToken();
				if (getToken() == ";")
					throw (ConfigurationError(getTokenBefore() + "need at least one argument"));
			(location.*pf)(*this);
			nextToken();
		}
		return (location);
	}
