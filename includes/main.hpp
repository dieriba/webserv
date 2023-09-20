/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtoure <dtoure@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 00:21:15 by jewancti          #+#    #+#             */
/*   Updated: 2023/09/19 23:16:58 by dtoure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef __MAIN_HPP__
# define __MAIN_HPP__

# define BUF_SIZE 42000


# include <iostream>
# include <cstring>
# include <string>
# include <fstream>
# include <sstream>
# include <istream>
# include <algorithm>
# include <vector>
# include <map>
# include <sys/epoll.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <fstream>
# include <string>
# include <signal.h>

typedef struct sockaddr_in  t_sockaddr;
typedef const std :: vector<std :: string>				cvstring;
typedef std :: vector<std :: string> :: iterator		vstring_it;
typedef std :: vector<std :: string> :: const_iterator	cvstring_it;

typedef std :: string :: iterator						string_it;
typedef std :: string :: const_iterator					cstring_it;
typedef std :: vector<std :: string>					vstring;


# define INTERNAL_SERVER_ERROR 500
# define BAD_GATEWAY 502 

# define INTERNAL_SERVER_ERROR_HTML() "<html><head><title>500 Internal Server Error</title></head><body><center><h1>500 Internal Server Error</h1></center></body></html>"
# define BAD_GATEWAY_HTML() "<html><head><title>502 Bad Gateway</title></head><body><center><h1>502 Bad Gateway</h1></center></body></html>"

#endif