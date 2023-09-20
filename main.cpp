/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtoure <dtoure@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:19:41 by jewancti          #+#    #+#             */
/*   Updated: 2023/09/19 21:23:57 by dtoure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileReader.hpp"
# include "Parser.hpp"
# include "Server.hpp"
# include "HttpServer.hpp"
# include "LibHelp.hpp"

std :: ostream	&operator<<(std :: ostream &os, const p_locate &locate)
{
	os << "Alias:\t[" << locate.first << "]\n";
	os << "Server name:\t[" << locate.second -> getServerName() << "]\n";
	os << "IP:\t\t[" << locate.second -> getListen() << "]\n";
	os << "Port:\t\t[" << locate.second -> getPort() << "]\n";
	os << "Root:\t\t[" << locate.second -> getRoot() << "]\n";
	os << "Route:\t\t[" << locate.second -> getRoute() << "]\n";
	os << "Index:\t\t[" << locate.second -> getIndex() << "]\n";
	os << "Redirect:\t[" << locate.second -> getRedirect() << "]\n";
	os << "Max body size:\t[" << locate.second -> getMaxBodySize() << "]\n";
	os << "Error page:\t[" << locate.second -> getErrorPage() << "]\n";
	os << "Post file path:\t[" << locate.second -> getFilePathPost() << "]\n";
	os << "AutoIndex:\t[" << locate.second -> getAutoIndex() << "]\n";
	os << "Allowed Methods:[" << LibHelp :: toString(locate.second -> getAllowedMethods(), "|") << "]\n";
	return os;
}

void signalHandler(int signum) {
	(void) signum;
	HttpServer::_SigInt = 1;
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return 0;
	signal(SIGINT, signalHandler);
	HttpServer httpServ;
	const std :: string	filename = av[1];
	if (! Parser :: parse(filename, httpServ)) {
		std :: cout << Parser :: getError() << '\n';
		return 0;
	}
	httpServ.printServers();

	signal(SIGPIPE, SIG_IGN);
	try
	{
		httpServ.startServer();
	}
	catch(const std::exception& e)
	{
		std::cerr << "CATCH " << e.what() << '\n';
	}
	
	return 0;
}

	// std::ifstream ift("dieriba.html");
	// if (!ift.good())
	// 	std::cout << "file not found" << std::endl;
	// std::string str;
	// ift >> str;
	// std::cout << str << std::endl;