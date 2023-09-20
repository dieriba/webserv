/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtoure <dtoure@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 23:21:46 by mmourdal          #+#    #+#             */
/*   Updated: 2023/09/20 01:52:17 by dtoure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CGI_HPP
# define CGI_HPP

# include "Stream.hpp"
# include "Methods.hpp"
# include "Client.hpp"

class Client;

class Cgi: public Stream
{
	public:
		Cgi ( int fd, Client * );
		Cgi ( Methods *req );
		Cgi ( Cgi const & src );
		Cgi &operator= ( Cgi const & rhs );
		~Cgi ( void );
		
		std::string	executeCgi ( void );
		std::string createPage ( void );
		bool 		ErrorStatusInBody ( std::string body );
		char **		mapToDoubleArrayEnv ( void );
		void		checkHeaderCGI ( std::string &body );
		void 		checkContentType ( std::string &body );
		void 		setCgiEnv ( void );
		void		clearCgi(void);
		void 		DisplayEnv ( void );
		int 		resetCgi(const int&);
		void 		setupCgi(Methods *);
		void		setupCgi( Methods *req, std::string body );
		virtual int		handleRequest(int epfd, epoll_event& ev);
		const int& 	getOutPipe(void);

		void	setPid(const pid_t&);

	private:
		Methods *							_req;
		
		char **								_envp;

		size_t 								_code;
		pid_t								_pid;
		std::string 						_method;
		std::string 						_response;
		std::string 						_body;
		std::string 						_typeCgi;
		std::string 						_binPathExec;
		std::string 						_scriptName;
		std::string 						_pathFile;
		std::string							_contentType;
		std::string							_queryString;
		int									_out_pipe[2];
		int 								_in_pipe[2];
		Client 								*_client;
		
		std::map <std::string, std::string> _env;
};

#endif