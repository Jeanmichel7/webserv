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
	std::vector<char> buffer;
	unsigned int readed;
	time_t time_start;
};

struct Request;

class Settings
{
	private:
		std::map<std::string, std::string> ext;
	public:
		Config		config;
		struct timespec check_request_timeout;
		void		build(int ke);
		std::string	get( Request const& req,  struct sockaddr_in const& client_addr, size_t size_read);
		std::string	post( Request const& req,  struct sockaddr_in const& client_addr, size_t size_read);
		std::string	del( void );
		std::string	date( void );
		std::string	badRequest( Request const& req );
		std::string	forbidden_error( void );
		std::string not_found( void );
		std::string	Unauthorized( void );
		int 				check_forbidden(std::string const& path);
		std::string			method_not_allowed( Request const& req );
		// std::string reading(int socket);
		std::vector<char> reading(int socket, unsigned int &readed, time_t &time_starting);
		std::string			checkextension(std::string const& path);
		std::string			folder_gestion(Request const& req);
		void						set_event(int ke, int socket, short filter, short flag);
		bool						checkmethod(Request const& req, Methods const& t);
		void						writing(int socket, std::vector<char> &sbuffer, struct sockaddr_in const& client_addr, unsigned int size_read);
		void						check_timeout(Sbuffer *requests, int ke);
		std::string	timeout( void );



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
