/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileReader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtoure <dtoure@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:42:41 by jewancti          #+#    #+#             */
/*   Updated: 2023/09/19 20:27:10 by dtoure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileReader.hpp"

cvstring	FileReader :: readAllLines(const std :: string& filename)
{
	vstring	lines;
	std :: ifstream	ift(filename.c_str());

	if (!ift.good()) {
		std :: cerr << "File not found (" << filename << ")\n";
		return lines;
	}
	std :: string	tmp;
	while ((std :: getline(ift, tmp))) {
		lines.push_back(tmp);
	}
    return lines;
}

const std :: string	FileReader :: readAllText(const std :: string& filename)
{
	std :: ifstream	ift(filename.c_str());
	if (!ift.good()) {
		std :: cerr << "File not found (" << filename << ")\n";
		return "";
	}
	return std :: string(
		std :: istreambuf_iterator<char>(ift),
		std :: istreambuf_iterator<char>()
	);
}

const std :: string	FileReader :: readHTMLFile(const std :: string& filename)
{
	std::string res;
	try
	{
		res = readAllText(filename);
		return res;
	}
	catch(const std::exception& e)
	{
		std::cerr << "Erreur lors de l'ouverture de " \
		<< filename << " " << e.what() << '\n';
	}
	return ("");
}