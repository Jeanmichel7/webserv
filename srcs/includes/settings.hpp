/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 14:00:16 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/20 14:12:02 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETTINGS_HPP
# define SETTINGS_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/ip.h>
# include <arpa/inet.h>
# include <string>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include "Config.hpp"
# include <exception>
# include <ctime>
# include <cstring>
# include <vector>
# include <sstream>
# include <sys/event.h>

class settings
{
	public:
		struct	sockaddr_in interface;
		int	build();
		std::string	get(Config const& config);
		int	settings::execute_cgi(std::string path);
		std::string	date( void );
		settings();
		~settings();
		class badCreation : public std::exception
		{
			virtual const char* what() const throw(){
				return ("settings::ErrorCreationInterfaceNetwork");
			}
		};
};

#endif