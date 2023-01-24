/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 15:11:48 by jrasser           #+#    #+#             */
/*   Updated: 2023/01/24 15:22:21 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "server.hpp"

struct	Methods
{
	bool	isget;
	bool	ispost;
	bool	isdelete;
};

//this class serv to extracte and create config and serveur object in a file 


namespace yd{
	bool isPath(std::string const &s);
}

class Tokenizer;

class Location
{
	public:
		Location();
		void setRoot(Tokenizer &tok);
		void setDefaultFile(Tokenizer &tok);
		void setDirectoryListing(Tokenizer &tok);
		void setCgi(Tokenizer &tok);
		void setUploadFile(Tokenizer &tok);	
		void setGet(Tokenizer &tok);
		void (Location::*selectSetter(std::string const &token))(Tokenizer &tok);
		std::string _upload_file;
		std::string _default_file;
		std::string	_root;
		std::map<std::string, std::string> _cgi;
		int	_is_get;
		int	_is_post;
		int	_is_delete;
		int _directory_listing;
	private:
		std::map<std::string, void (Location::*)(Tokenizer &tok)> _tokens;
};

class FormatError: public std::exception
{
	public:
		FormatError(std::string token, std::string expected_format);
};

class ConfigurationError: public std::exception
{
	public:
		ConfigurationError(std::string message);
};

	
class Server
{
	public:
		Server();
		void setIp(Tokenizer &tok);
		void setServerName(Tokenizer &tok);
		void setPort(Tokenizer &tok);
		void setMaxBody(Tokenizer &tok);
		void setError(Tokenizer &tok);
		void (Server::*selectSetter(std::string const &token))(Tokenizer &tok);
		void addLocation(Location location);
	private:
		std::string _server_name;
		int					_port;
		uint32_t		_ip;
		int _max_body_size;
		std::map<unsigned int, std::string> _error_pages;
		std::vector<Location> _locations;
		std::map<std::string, void (Server::*)(Tokenizer &tok)> _tokens;
};

// this class serv to store and use Server and Location object

class Config
{
	public:
		friend class Tokenizer;
		Config(const std::string &path);
		//void	selectServ(const std::string &host, const unsigned int port = 80, const unsigned int ip = 12701) {}
		//const std::string &getFile(const std::string &path) const;
		//const std::string &getError(const unsigned int error) const;
		//const std::string &getCgi(const std::string &path, std::string cgi) const;
		//const std::string &getUpload(const std::string &path) const;
		//const bool getDirectoryListing(const std::string &path) const;
		//const unsigned int &getMaxSize() const;
		void addServer(Server server);
		//~Config();
	private : 
		Config();
	
		std::vector<Server> _server;
};

class Tokenizer
{
	public:
		Tokenizer(Config &config, std::string const &path);
		const std::string &nextToken();
		const std::string &getTokenBefore();
		const std::string &getToken();
	private:
		std::string _token_before;
		std::string _token;
		std::ifstream _file;
		Server parsServer(); 
		Location parsLocation();
		class Unexpected: public std::exception
		{
			public:
				Unexpected(std::string token);
		};
};



#endif

