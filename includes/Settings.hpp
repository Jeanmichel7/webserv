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

#include "server.hpp"

class Settings
{
	public:
		struct	sockaddr_in interface;
		int	build();
		// std::string	get(Config const& config);
		// int	Settings::execute_cgi(std::string path);
		std::string	date( void );
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
