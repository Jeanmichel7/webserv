/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 14:00:16 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/20 14:12:02 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETTINGS_HPP
# define SETTINGS_HPP

# include "server.hpp"
# include "Config.hpp"
# define ERROR_BRUT_FOLDER "http/"
struct Sbuffer {
	Request				_req; 
	unsigned int 		readed;
	time_t 				time_start;

	bool 				is_chunked;
	bool				is_413;
	int					status_code;
	bool				_add_eof;

	CGI 				_cgi_data;
	int					_pid;

	std::vector<char> 	_buffer;
	std::string 		_header;
	std::string 		_cookie;
	
	bool				_request_parsed;
	bool				_cgi_process_launched;
	bool				_body_ready;
	bool				_header_sent;
	bool				_response_sent;
	
	size_t				_total_sent;
};

struct Request;


class Settings
{
	private:
		std::map<std::string, std::string> ext;
		std::map<int, std::string> error;
		std::string _header;
		std::vector<char> _body;
		std::string _cookie;
		bool _add_eof;
	public:
		Config			config;
		struct timespec check_request_timeout;
		int 			checkArgs(int argc);
		void			build(int ke);
		std::string		createHeader( Request const &req,  struct sockaddr_in const& client_addr);
		void			del(Sbuffer &client)
		std::string		date( void );
		std::string		badRequest( Request const& req );
		std::string		forbidden_error( void );
		std::string 	not_found( void );
		std::string		Unauthorized( void );
		int 			check_forbidden(std::string const& path);
		std::string		method_not_allowed( Request const& req );
		size_t 			reading(int socket, unsigned int &readed, time_t &time_starting, char *buffer);
		size_t 			reading_header(int socket, unsigned int &readed, time_t &time_starting, char *buff);
		char * 			reading_chunck(int socket, unsigned int &readed, time_t &time_starting);
		std::string		checkextension(std::string const& path);
		void			folder_gestion(Sbuffer &client);
		void			set_event(int ke, int socket, short filter, short flag);
		bool			checkmethod(Request const& req, Methods const& t);
		bool			parseRequest(Sbuffer &client);
		bool 			writeResponse(Sbuffer &client, int socket);
		void			generate_cookie(Sbuffer &client, struct sockaddr_in const& client_addr);
		
		void			check_timeout(Sbuffer *requests, int ke, std::map<int, sockaddr_in> &clients);
		std::string		timeout( void );

		void			generate_body(Sbuffer &client, struct sockaddr_in const& client_addr);

		std::string handleCookie(const Request & req, std::string &date, int &count);
		Settings( Config const& base );
		Settings();
		~Settings();
		class badCreation : public std::exception
		{
			virtual const char* what() const throw(){
				return ("Settings::ErrorCreationInterfaceNetwork");
			}
		};
};

#endif
