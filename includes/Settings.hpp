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
#define SETTINGS_HPP

#include "server.hpp"

#define ERROR_BRUT_FOLDER "webserv_default_ressources/"

struct Sbuffer
{
	Sbuffer();
	void clean();
	Request _req;
	unsigned int readed;
	time_t time_start;
	time_t purge_last_time;

	size_t _chunk_index;
	int status_code;
	bool _add_eof;

	CGI _cgi_data;
	int _pid;

	std::vector<char> _buffer;
	std::string _body_cookie;
	std::string _header;
	std::string header_script;

	Status _status;
	size_t _total_sent;
};

struct Request;

class Settings
{
private:
	std::map<std::string, std::string> ext;
	std::map<int, std::string> error;

public:
	Config config;
	struct timespec check_request_timeout;
	int checkArgs(int argc);
	void build(int ke);
	std::string createHeader(Request const &req, struct sockaddr_in const &client_addr);
	void del(Sbuffer &client);
	std::string date(void);
	std::string badRequest(Request const &req);
	std::string forbidden_error(void);
	std::string not_found(void);
	std::string Unauthorized(void);
	int check_forbidden(std::string const &path);
	std::string method_not_allowed(Request const &req);
	int reading_socket(int socket, time_t &time_starting, std::vector<char> &client_buff);
	size_t read_hex_size(const std::vector<char> &data, size_t &index);
	int process_chunks(std::vector<char> &data, size_t start_index, size_t &current_index);
	std::string checkextension(std::string const &path);
	void folder_gestion(Sbuffer &client);
	void set_event(int ke, int socket, short filter, short flag);
	bool checkmethod(Request const &req, Methods const &t);
	bool parseRequest(Sbuffer &client);
	void writeResponse(Sbuffer &client, int socket);
	void generate_cookie(Sbuffer &client, struct sockaddr_in const &client_addr);
	void gestion_413(Sbuffer &client, int socket);
	void check_timeout(std::map<int, Sbuffer> &requests, int ke, std::map<int, sockaddr_in> &clients);
	std::string timeout(void);

	void generate_body(Sbuffer &client, struct sockaddr_in const &client_addr);
	void generate_header(Sbuffer &client);

	bool reqIsChuncked(std::string req);
	void reading_request(Sbuffer &sbuffer, Settings &server, int ke, uintptr_t ident, Request &req);

	std::string handleCookie(Sbuffer &client);
	Settings(Config const &base);
	Settings();
	~Settings();
	class badCreation : public std::exception
	{
		virtual const char *what() const throw()
		{
			return ("Settings::ErrorCreationInterfaceNetwork");
		}
	};
};

#endif
