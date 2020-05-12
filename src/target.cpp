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

dbust::Target dbust::Target::parse_url(const std::string_view& url)
{
    Target target;
    std::size_t eop { 4 };
    if (url.at(0) == 'h' && url.at(1) == 't' && url.at(2) == 't' && url.at(3) == 'p') {
        if (url.at(4) == 's') {
            target.ssl = true;
            eop = 5;
        }
        if (!(url.at(eop) == ':' && url.at(eop + 1) == '/' && url.at(eop + 2) == '/')) {
            throw dbust::IncorrectUrlException(dbust::IncorrectUrlException::Part::CONVENTION);
        }
        std::size_t b_host = eop + 3;
        std::size_t e_host = b_host;
        while (e_host < url.length() && url[e_host] != ':' && url[e_host] != '/') {
            ++e_host;
        }
        target.host = url.substr(b_host, e_host - b_host);
        std::size_t b_port { e_host }, e_port { e_host };
        if (url[e_host] == ':') {
            while (e_port < url.length() && url[e_port] != '/') {
                ++e_port;
            }
            target.port = url.substr(b_port + 1, e_port - b_port - 1);
            try {
                std::stoi(target.port);
            } catch (std::exception& e) {
                throw dbust::IncorrectUrlException(dbust::IncorrectUrlException::Part::PORT);
            }
        } else {
            target.port = target.ssl ? "443" : "80";
        }
        target.resource_path = url.substr(e_port, url.length() - e_port);
        if (*(target.resource_path.cend() - 1) != '/') {
            target.resource_path.push_back('/');
        }
    } else {
        throw dbust::IncorrectUrlException(dbust::IncorrectUrlException::Part::PROTOCOL);
    }
    return target;
}

dbust::Target::Target() = default;
