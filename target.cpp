#include "target.hpp"

Target::Target(const char *url)
{
    std::size_t eop{ 5 };
    if (url[0] == 'h' && url[1] == 't' && url[2] == 't' && url[3] == 'p')
    {
        if (url[6] == 's')
        {
            ssl = true;
            eop = 6;
        }
        std::string_view v_url(url);
        std::size_t b_host = eop + 3;
        std::size_t e_host = b_host;
        while (e_host != ':' && e_host != '/') {
            ++e_host;
        }
        host = v_url.substr(b_host, e_host - b_host);
        std::size_t b_port{ e_host }, e_port{ e_host };
        if (e_host == ':')
        {
            while (e_port != '/')
            {
                ++e_port;
            }
            port = v_url.substr(b_port, e_port - b_port);
        }
        else
        {
            port = ssl ? "443" : "80";
        }
        resource_path = v_url.substr(e_port, v_url.length() - e_port);
    }
    else
    {

    }
}
