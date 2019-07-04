#include "target.hpp"
#include "exceptions.hpp"

Target::Target(const char *url)
{
    std::size_t eop{ 4 };
    if (url[0] == 'h' && url[1] == 't' && url[2] == 't' && url[3] == 'p')
    {
        if (url[4] == 's')
        {
            ssl = true;
            eop = 5;
        }
        std::string_view v_url(url);
        std::size_t b_host = eop + 3;
        std::size_t e_host = b_host;
        while (e_host < v_url.length() && v_url[e_host] != ':' && v_url[e_host] != '/') {
            ++e_host;
        }
        host = v_url.substr(b_host, e_host - b_host);
        std::size_t b_port{ e_host }, e_port{ e_host };
        if (v_url[e_host] == ':')
        {
            while (e_port < v_url.length() && v_url[e_port] != '/')
            {
                ++e_port;
            }
            port = v_url.substr(b_port + 1, e_port - b_port - 1);
        }
        else
        {
            port = ssl ? "443" : "80";
        }
        resource_path = v_url.substr(e_port, v_url.length() - e_port);
    }
    else
    {
        throw IncorrectUrl();
    }
}

const Target& Target::instance(const char* url)
{
    static Target target(url);
    return target;
}

const std::string& Target::get_host() const
{
    return host;
}

const std::string& Target::get_port() const
{
    return port;
}

const std::string& Target::get_resource_path() const
{
    return resource_path;
}

bool Target::is_ssl() const
{
    return ssl;
}
