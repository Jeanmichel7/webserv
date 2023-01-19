/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 14:00:16 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/18 12:29:45 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/ip.h>
# include <arpa/inet.h>
# include <cstring>

class request
{
	private:
	protected:
	public:
		request();
		request(request const& base);
		~request();
		request& operator =(request const& base);
};

typedef struct	s_methods
{
	bool	isget;
	bool	ispost;
	bool	isdelete;
} t_methods;

class Location
{
	public:
		t_methods	_methods;
		std::string	root;
}


class Config_data
{
	public:
		Config_data();
		std::string server_name;
		int					port; 
		uint32_t		ip;
		t_methods	methods;
		std::string	root;
		std::string index;
		std::vector<location> location_tab;
}

#endif
