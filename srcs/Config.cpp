/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 14:14:59 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/23 12:28:00 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Config.hpp"

const std::string &Config::getFile(const std::string &path) const
{
	return ("~/webServ/website/index.html");
}
const Method	&Config::getMethod(const std::string &path) const{
	
	Methods i;
	i.isget = true;
	i.ispost = true;
	i.isget = true;
	return(i);
}
const std::string &Config::getError(const unsigned int error) const
{
	return ("~/webServ/website/404.html");
}
const std::string &Config::getCgi(const std::string &cgi)
{
	return ("usr/bin/php");
}
const unsigned int &Config::getMaxSize()
{
	return (10000000);
}
const std::string &Config::getUpload(const std::string &path) const
{
	return ("/bonjour");
}
bool Config::getDirectoryListing(const std::string &path) const
{
	return (true);
}