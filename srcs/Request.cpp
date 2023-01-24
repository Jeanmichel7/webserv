/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:56:22 by jrasser           #+#    #+#             */
/*   Updated: 2023/01/24 17:45:37 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
// #include <iostream>



/*
La taille maximale d'une URI (Uniform Resource Identifier) est de 2048 caractères selon la spécification 
de la norme HTTP/1.1.
La taille maximale des en-têtes de requête est de 8192 octets, selon la même spécification.
La taille maximale du corps de la requête dépend des capacités du serveur, mais généralement elle est de 8 Mo 
pour Apache et de 2 Mo pour IIS.
Il est important de noter qu'il est possible de configurer ces limites en fonction de vos besoins spécifiques 
en utilisant les paramètres de configuration appropriés de votre serveur.
*/

/*
Pour être considérée comme valide, une requête HTTP 1.1 doit respecter les règles définies dans 
la spécification HTTP 1.1 du RFC 7230 à 7235. Les principales exigences sont les suivantes:

La méthode de la requête (GET, POST, etc.) doit être valide et correctement spécifiée.
L'URI de la ressource demandée doit être valide et correctement formatée.
Les en-têtes de la requête doit être valide et correctement formaté.
Le corps de la requête doit être valide s'il est présent.
Le format général de la requête doit respecter la syntaxe HTTP standard.

Il est important de noter que ces exigences ne garantissent pas que la requête sera 
traitée avec succès par le serveur, il est nécessaire de respecter les règles de la spécification 
HTTP pour que la requête soit considérée comme valide.

  //check uri size max 2048 char
  //check header size max 8192 octets
  //check body size max 8Mo

  
*/




Request::Request()
:
  str_method(""),
  path(""),
  parameters(""),
  anchor(""),
  protocole(""),
  host(""),
  user_agent(),
  accept(),
  accept_language(),
  accept_encoding()
{
  method.isGet = false;
  method.isPost = false;
  method.isDelete = false;
  return ;
}

Request::Request(Request const &src)
{
  *this = src;
  return ;
}

Request::~Request()
{
  return ;
}

Request &Request::operator=(Request const &rhs)
{
  if (this != &rhs)
  {
    this->method = rhs.method;
    this->path = rhs.path;
    this->parameters = rhs.parameters;
    this->anchor = rhs.anchor;
    this->protocole = rhs.protocole;
    this->host = rhs.host;
    this->user_agent = rhs.user_agent;
    this->accept = rhs.accept;
    this->accept_language = rhs.accept_language;
    this->accept_encoding = rhs.accept_encoding;
  }
  return (*this);
}

void Request::parse(char *req_brut) {
  // Request req;
  std::cout << "parsing request" << std::endl;

  std::cout << "Request Brut : \n" << req_brut << std::endl;
  std::cout << "Request Brut size : " << strlen(req_brut) << std::endl;

  // return req;
}
