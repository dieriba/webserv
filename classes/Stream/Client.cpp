/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                  map_locate                              +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 01:36:42 by rferradi          #+#    #+#             */
/*   Updated: 2023/08/05 23:47:03 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Client.hpp"
# include "Get.hpp"
# include "Post.hpp"
# include "Delete.hpp"
# include "Server.hpp"

char Client::_buff[BUF_SIZE] = {0};
char Client::_error[9] = "404.html";

/*--------------------------------------------------CONSTRUCTOR/DESTRUCTOR--------------------------------------------------*/
Client::Client(int fd, Server *ptr): Stream(fd, ptr)
{
	//std::cout << "SERV " << _server << std::endl;
	_cgi = new Cgi(fd, this);
	_method = NULL;
}

Client::Client(int fd, Server *ptr, int epfd): Stream(fd, ptr, epfd)
{
	//std::cout << "SERV " << _server  << std::endl;
	_method = NULL;
	_cgi = new Cgi(fd, this);
}


Client::~Client()
{
	if (_method)
		delete _method;
	delete _cgi;
	close(_socketFd);
}
/*--------------------------------------------------CONSTRUCTOR/DESTRUCTOR--------------------------------------------------*/

/*--------------------------------------------------MEMBER FUNCTION--------------------------------------------------*/

void	Client::setMethods(Methods *met){ _method = met; }

bool	Client::didIreadAll()
{
	std::size_t pos = _request.find("Content-Length: ") + 16;
	std::size_t end = _request.find("\r\n", pos);
	if (end == std::string::npos || pos == std::string::npos)
		return 0;
	//std::cout << "pos = " << pos << " end = " << end << std::endl;
	std::stringstream ss(_request.substr(pos, end - pos));
	std::size_t res;
	ss >> res;
	//std::cout << "---> subSTR " << _request.substr(pos, end - pos)<<std::endl;
	//std::cout << "did i read = " << res << std::endl;
	if (_request.size() < res + end)
		return (0);
	return 1;
}

int		Client::readingHttpRequest(epoll_event& ev)
{
	if (_request.find("\r\n\r\n") != std::string::npos)
	{
		if (_request.find("GET") != std::string::npos)
		{
			//std::cout << "Requete GET------------------------------------------\n";
			//std::cout << " la requete " << _request << std::endl;
			//std::cout << "-------------------------------------------------------\n\n";
				
			//std::cout << "METHOD VALUE " << _method << std::endl;
				
			if (_method == NULL)
				_method = new Get(_request, (_server->getLocations()), _cgi);
			_response = _method->HttPage(_epfd);
				
			//std::cout << "respons size " << _response.size() << std::endl;
			//std::cout << "La reponse --> " << _response << std::endl;
				
			if (_response != "" || _method->isCgi() == true)
				ev.events = EPOLLOUT;
			_reqType = GET;
			if (epoll_ctl(_epfd, EPOLL_CTL_MOD, _socketFd, &ev) < 0)
				return -1;
		}
		else if (_request.find("POST") != std::string::npos)
		{
			//std::cout << "Requete POST------------------------------------------\n";
			//std::cout << "zebi ya heja\n";
			//std::cout << " la requete " << _request << std::endl;
			//std::cout << "-------------------------------------------------------\n\n";
				
			if (_method == NULL)
				_method = new Post(_request, (_server->getLocations()), _cgi);
			_reqType = POST;
				
			if (_method->handle())
			{
				_response = _method->HttPage(_epfd);
				if (_response != "" || _method->isCgi() == true)
					ev.events = EPOLLOUT;
				if (epoll_ctl(_epfd, EPOLL_CTL_MOD, _socketFd, &ev) < 0)
					return -1;
			}	
		}
		else if (_request.find("DELETE") == 0)
		{
			if (_method == NULL)
				_method = new Delete(_request, (_server->getLocations()), _cgi);
				
			_method->handle();
			_response = _method->HttPage(_epfd);
			ev.events = EPOLLOUT;
				
			if (epoll_ctl(_epfd, EPOLL_CTL_MOD, _socketFd, &ev) < 0)
				return -1;
		}
	}
	
	return 0;
}

int		Client::handleRequest(int epfd, epoll_event& ev)
{
	if (ev.events == EPOLLIN)
	{
		ssize_t rec = read(_socketFd, Client::_buff, BUF_SIZE);

		if (rec <= 0)
			return -1;

		_request.append(Client::_buff, rec);
		
		return readingHttpRequest(ev);
	}
	else
	{
		if (_method && _method->isCgi())
			return 0;

		delete _method;
		_method = NULL;

		if (send(_socketFd, _response.c_str(), _response.length(), 0) < 0)
			return -1;

		//std::cout << "j'ai recu " << _request << std::endl;
		//std::cout << "\n\nj'ai rep\n\n" << _response << std::endl;

		ev.events = EPOLLIN;
		if (epoll_ctl(epfd, EPOLL_CTL_MOD, _socketFd, &ev) < 0)
			return 0;
		
		_request.clear();
	}

	return (1);
}

/*--------------------------------------------------MEMBER FUNCTION--------------------------------------------------*/
