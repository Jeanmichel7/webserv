/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 14:15:11 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/20 14:15:12 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>


struct	Methods
{
	bool	isget;
	bool	ispost;
	bool	isdelete;
}; 

class Config
{
	public:
		void	selectServ(const std::string &host, const unsigned int port = 80, const unsigned int ip = 12701);
		const std::string &getRoot(std::string path) const;
		const std::string &getFile(std::string path) const;
		const std::string &getError(const unsigned int error) const;
		const std::string &getCgi(std::string path, std::string cgi) const;
};

#endif