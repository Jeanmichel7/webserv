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
class Config;
struct Request;

class Settings
{
	public:
		struct		sockaddr_in interface;
		struct		sockaddr_in interfacee;
		int			build( Config const &config, struct kevent *change, const char *i, int ke);
		std::string	get( Config& config, Request const& req );
		std::string post( Config& config, Request const& req );
		std::string del( Config const& config );
		std::string	date( void );
		std::string	badRequest( Config const & config );
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
