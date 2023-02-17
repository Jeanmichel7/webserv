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
struct Request;

class Settings
{
	private:
		std::map<std::string, std::string> ext;
	public:
		Config		config;
		int			*list_of_serv_socket;
		void		build(int ke);
		std::string	get( Request const& req );
		std::string	post( Request const& req );
		std::string	del( void );
		std::string	date( void );
		std::string	badRequest( Request const& req );
		std::string	forbidden_error( void );
		std::string not_found( void );
		std::string	Unauthorized( void );
		int 		check_forbidden(std::string const& path);
		std::string	method_not_allowed( Request const& req );
		std::string	reading(int socket, Request req);
		std::string	checkextension(std::string const& path);
		std::string	folder_gestion(Request const& req);
		void		set_event(int ke, int socket, short filter, short flag);
		int 		checkmethod(std::string const& request, Methods const& t);
		void writing(int socket, Request & req, std::string sbuffer);
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
