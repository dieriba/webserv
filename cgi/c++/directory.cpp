# include <string.h>
# include <cstring>
# include <dirent.h>
# include <iostream>
# include <stdlib.h>

# include "../../includes/utils/CommonLib.hpp"

int main ()
{
    const std::string& path(getenv(PATH));

    DIR* dir = opendir(getenv(ROOT));

    if (dir) 
    {
        std::cout << "Content-Type: text/html\r\n\r\n";
        std::cout << "<!DOCTYPE html>\n";
        std::cout << "<html lang=\"en\">\n";
        std::cout << "<head>\n";
        std::cout << "<meta charset=\"UTF-8\">\n";
        std::cout << "<title>Directory Listing</title>\n";
        std::cout << "</head>\n";
        std::cout << "<body>\n";
        std::cout << "<h2>Directory Listing</h2>\n";

        std::cout << "<table>\n";
        std::cout << "<thead><tr><th>Name</th><th>Type</th></tr></thead>\n";
        std::cout << "<tbody>\n";

        struct dirent* entry;

        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry -> d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0 || entry -> d_name[0] == '.') continue ;

            std::string name = entry -> d_name;
            std::string type;

            if (entry -> d_type == DT_DIR)
                type = "Directory";
            else 
                type = "File";
         
            std::cout << "<tr><td><a href=\"" << path + "/" + name << "\">" << name << "</a></td><td>" << type << "</td></tr>\n";
        }

        closedir(dir);

        std::cout << "</tbody>\n";
        std::cout << "</table>\n";
        std::cout << "</body>\n";
        std::cout << "</html>\n";
    }
    else 
    {
        std::cout << "Failed to open directory.";
    }
}