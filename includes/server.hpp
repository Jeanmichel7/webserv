/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 15:20:31 by jrasser           #+#    #+#             */
/*   Updated: 2023/02/10 13:49:42 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/ip.h>
# include <arpa/inet.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/event.h>
# include <netdb.h>

# include <algorithm>
# include <iostream>
# include <string>
# include <cstring>
# include <ctime>
# include <sstream>
# include <vector>
# include <exception>
# include <cerrno>
# include <map>
# include <fstream>
# include <cstdio>
# include <cstdlib>
# include <dirent.h>

# include "Settings.hpp"
# include "Request.hpp"
# include "Config.hpp"
# include "Cgi.hpp"
#endif