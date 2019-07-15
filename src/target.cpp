#include "target.hpp"
#include "exceptions.hpp"

const std::string& dbust::Target::get_host() const
{
    return host;
}

const std::string& dbust::Target::get_port() const
{
    return port;
}

const std::string& dbust::Target::get_resource_path() const
{
    return resource_path;
}

bool dbust::Target::is_ssl() const
{
    return ssl;
}

dbust::Target dbust::Target::parse_url(const char* url)
{
    Target target;
    std::size_t eop{ 4 };
    if (url[0] == 'h' && url[1] == 't' && url[2] == 't' && url[3] == 'p')
    {
        if (url[4] == 's')
        {
            target.ssl = true;
            eop = 5;
        }
        if (!(url[eop] == ':' && url[eop + 1] == '/' && url[eop + 2] == '/'))
        {
            throw dbust::IncorrectUrlException();
        }
        std::string_view v_url(url);
        std::size_t b_host = eop + 3;
        std::size_t e_host = b_host;
        while (e_host < v_url.length() && v_url[e_host] != ':' && v_url[e_host] != '/') {
            ++e_host;
        }
        target.host = v_url.substr(b_host, e_host - b_host);
        std::size_t b_port{ e_host }, e_port{ e_host };
        if (v_url[e_host] == ':')
        {
            while (e_port < v_url.length() && v_url[e_port] != '/')
            {
                ++e_port;
            }
            target.port = v_url.substr(b_port + 1, e_port - b_port - 1);
            try
            {
                std::stoi(target.port);
            }
            catch (std::exception& e)
            {
                throw dbust::IncorrectUrlException();
            }
        }
        else
        {
            target.port = target.ssl ? "443" : "80";
        }
        target.resource_path = v_url.substr(e_port, v_url.length() - e_port);
        if (*(target.resource_path.cend() - 1) != '/')
        {
            target.resource_path.push_back('/');
        }
    }
    else
    {
        throw dbust::IncorrectUrlException();
    }
    return target;
}

dbust::Target::Target() = default;
