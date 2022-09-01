/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarouf <cmarouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 13:50:09 by cmarouf           #+#    #+#             */
/*   Updated: 2022/09/01 14:49:49 by cmarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "request_handler.hpp"

//*? Ce qui nous interesse dans le header :
//*? Quel fichier est demander, donc verifier si ce fichier existe, OU est a executer avec la CGI
//*? La methode (GET, POST, ou DELETE)
//*? L'host, donc, l'adresse ip suivi du port, ou exclusivement localhost suivi du host (127.0.0.1:4242 OU localhost:4242 )
//*? La connection, doit etre keep-alive pour l'instant.
//*? sec-ch-ua (Le navigateur utiliser), doit etre Google Chrome

request_handler::request_handler( void ) : state(true), _data()
{

}

request_handler::request_handler( char * raw_header ) : _header(raw_header), state(true), _data()
{
	
}

request_handler::~request_handler( void )
{

}

void request_handler::parse_header( void )
{
	std::cout << RED << _header << WHITE << std::endl;
}