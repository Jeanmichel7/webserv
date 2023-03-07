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

struct Sbuffer {
	std::vector<char> 	buffer;
	unsigned int 		readed;
	time_t 				time_start;
	bool 				is_chunked;
	bool				is_413;
};

struct Request;

class Settings
{
	private:
		std::map<std::string, std::string> ext;
		std::string _header;
		std::vector<char> _body;
		std::string _cookie;
		bool _add_eof;
	public:
		Config			config;
		struct timespec check_request_timeout;
		int 			checkArgs(int argc);
		void			build(int ke);
		void			get( Request const &req,  struct sockaddr_in const& client_addr);
		void			post( Request const &req,  struct sockaddr_in const& client_addr);
		void 			del( Request const &req, struct sockaddr_in const& client_addr);
		std::string		date( void );
		void			badRequest( Request const& req );
		void			forbidden_error( void );
		void 			not_found( void );
		void			Unauthorized( void );
		int 			check_forbidden(std::string const& path);
		void			method_not_allowed( Request const& req );
		size_t 			reading(int socket, unsigned int &readed, time_t &time_starting, char *buffer);
		size_t 			reading_header(int socket, unsigned int &readed, time_t &time_starting, char *buff);
		char * 			reading_chunck(int socket, unsigned int &readed, time_t &time_starting);
		std::string		checkextension(std::string const& path);
		void			folder_gestion(Request const& req);
		void			set_event(int ke, int socket, short filter, short flag);
		bool			checkmethod(Request const& req, Methods const& t);
		bool			writing(int socket, std::vector<char> &sbuffer, struct sockaddr_in const& client_addr, bool &close_connexion);
		void			check_timeout(Sbuffer *requests, int ke);
		std::string		timeout( void );

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
