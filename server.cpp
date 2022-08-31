/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarouf <cmarouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 09:20:49 by cmarouf           #+#    #+#             */
/*   Updated: 2022/08/31 12:50:07 by cmarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>


/*

	int socket(int domain, int type, int protocol);
	
	A socket can be seen an fd
	This function return -1 if an error occured, otherwise it return an fd assigned to the socket

	first parameter  : int domain
	it refer to the protocol the socket will use to communicate
	Possible value:
	{
		AF_UNIX, AF_LOCAL - Local communication
		AF_INET - IPv4 Internet protocols
		AF_INET6 - IPv6 Internet protocols
		AF_IPX - IPX Novell protocols
	}
	second parameter : int type
	it specifies if the communication will be connectionless, or persistent, not all types are compatible with the domain
	Possible value:
	{
		SOCK_STREAM - Two-way reliable communication (TCP) (we will use this one)
		SOCK_DGRAM - Connectionless, unreliable (UDP)
	}
	third parameter  : int protocol
	we will use the value 0
	

*/



int main ( int ac, char **av )
{
	int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if ( socket_fd < 0 )
	{
		std::cout << "ERROR CREATING THE SOCKET FD " << std::endl;
		return 0;
	}

	
	sockaddr_in sockaddr;

	sockaddr.sin_family = AF_UNIX;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(1111);

	if ( bind(socket_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0 )
	{
		std::cout << "ERROR BINDING THE SOCKET TO AN IP" << errno 	<< std::endl;
		return 0;
	}

	if ( listen(socket_fd, 10) < 0 )
	{
		std::cout << "ERROR LISTENING TO THE SOCKET "  << std::endl;
		return 0;
	}
	

	socklen_t addrlen = sizeof(sockaddr);

	int connection = accept(socket_fd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);
	
	if ( connection < 0 )
	{
		std::cout << "ERROR EXTRACTING A CLIENT FROM THE SOCKET " << std::endl;
		return 0;
	}
	
	
	char buffer[1024];

	read(connection, buffer, 1024);
	std::cout << "The message is :" << buffer << std::endl;

	std::string reponses = " HELLO FROM THE SERVER ";	

	send(connection, reponses.c_str(), reponses.size(), 0);

	close(connection);
	close(socket_fd);
	
	
	return 0;
}