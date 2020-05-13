#include <fcntl.h>
#include "config.hpp"

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

std::vector<std::string> dbust::BatchOptParser::parse(const std::string_view& opt) const
{
    auto f_opt = (opt[0] == '\'' && opt[opt.size() - 1] == '\'') || (opt[0] == '"' && opt[opt.size() - 1] == '\"') ? opt.substr(1, opt.length() - 2) : opt;
    std::vector<std::string> res;
    std::string tmp;
    for (const char c : f_opt) {
        if (c == ',') {
            res.push_back(std::move(tmp));
            tmp = "";
        } else {
            tmp.push_back(c);
        }
    }
    res.push_back(tmp);
    return res;
}

const std::string dbust::CmdParser::description {
    "Usage: dbust [OPTION...]\n\n"
    "-u, --url <string> (*)                                               target url\n"
    "-d, --dictionary <string> (*)                                        path to dictionary\n"
    "-s, --sockets <int> (100)                                            number of concurrent sockets\n"
    "-t, --timeout <int> (10)                                             duration in seconds to reconnect try\n"
    "-e, --file-extensions <[string]> ()                                  file extensions to search for\n"
    "-c, --status-codes <[string]> (200, 201, 400, 401, 403, 500)         valid status codes\n"
    "-y, --delay <int> (100)                                              delay in milis between requests\n"
    "--user-agent <string> (dbust)                                        request user agent\n"
    "--get                                                                use GET instead HEAD\n"
    "-h, --help                                                           this help\n\n"
    "Examples:\n"
    "dbust -u https://www.example.com/dir/ -d /home/user/dict.txt\n"
    "dbust -c '200,201,400,401,403,500' -e 'php,txt' -s 1000 -u http://192.168.0.92:8080/ -d /home/user/dict.txt"
};

dbust::Config dbust::CmdParser::parse(int argc, const char* argv[])
{
    int get { 0 };
    ::option long_options[] {
        { "status-codes", required_argument, nullptr, 'c' },
        { "file-extensions", required_argument, nullptr, 'e' },
        { "sockets", required_argument, nullptr, 's' },
        { "dictionary", required_argument, nullptr, 'd' },
        { "url", required_argument, nullptr, 'u' },
        { "timeout", required_argument, nullptr, 't' },
        { "delay", required_argument, nullptr, 'y' },
        { "user-agent", required_argument, nullptr, 'a' },
        { "get", no_argument, &get, 1 },
        { "help", no_argument, nullptr, 'h' }
    };
    char c { '\0' };
    int option_index { 0 };
    std::size_t bitmask { 0 };
    Config config;
    while ((c = getopt_long(argc, const_cast<char**>(argv), "y:c:t:e:s:d:u:h", long_options, &option_index)) != -1) {
        switch (c) {
        case 'c': {
            auto sc_vec { parser.parse(optarg) };
            //TODO Fix status codes.
            break;
        }
        case 'e': {
            auto ex_vec { parser.parse(optarg) };
            for (auto& ex : ex_vec) {
                ex.insert(0, 1, '.');
            }
            ex_vec.emplace_back("");
            config.set_file_extensions(ex_vec);
            break;
        }
        case 's':
            config.set_nb_of_sockets(std::stoi(::optarg));
            break;
        case 'd':
            config.set_dictionary(reader.read_dictionary(::optarg));
            bitmask |= 1u << 1u;
            break;
        case 'u': {
            auto target = Target::parse_url(::optarg);
            config.set_target(target);
            bitmask |= 1u << 0u;
            break;
        }
        case 't':
            config.set_timeout(std::stoi(::optarg));
            break;
        case 'a':
            config.set_user_agent(::optarg);
            break;
        case 'y':
            config.set_delay(std::stoi(::optarg));
            break;
        case 'h':
            std::cout << description << std::endl;
            std::exit(0);
        default: {
        }
        }
    }
    config.set_get(get);
    switch (bitmask) {
    case 3:
        return config;
    case 2:
        throw dbust::ArgumentNotSetException(dbust::ArgumentNotSetException::Arg::URL);
    case 1:
        throw dbust::ArgumentNotSetException(dbust::ArgumentNotSetException::Arg::DIRECTORY);
    case 0:
        throw dbust::ArgumentNotSetException(dbust::ArgumentNotSetException::Arg::ALL);
    default:
        throw dbust::UnexpectedException();
    }
}

dbust::CmdParser::CmdParser(const dbust::DictionaryReader& d_reader,
    const dbust::BatchOptParser& parser)
    : reader { d_reader }
    , parser { parser }
{
}

const dbust::StatusCodes& dbust::Config::get_status_codes() const
{
    return status_codes;
}

const std::vector<std::string>& dbust::Config::get_file_extensions() const
{
    return file_extensions;
}

std::size_t dbust::Config::get_nb_of_sockets() const
{
    return nb_of_sockets;
}

std::size_t dbust::Config::get_timeout() const
{
    return timeout;
}

const dbust::Target& dbust::Config::get_target() const
{
    return target;
}

bool dbust::Config::is_get() const
{
    return get;
}

const std::vector<std::string>& dbust::Config::get_dictionary() const
{
    return dictionary;
}

const std::string& dbust::Config::get_user_agent() const
{
    return user_agent;
}

void dbust::Config::set_status_codes(const dbust::StatusCodes& new_status_codes)
{
    //TODO Fix status_codes setter.
}

void dbust::Config::set_file_extensions(const std::vector<std::string>& new_file_extensions)
{
    Config::file_extensions = new_file_extensions;
}

void dbust::Config::set_dictionary(const std::vector<std::string>& new_dictionary)
{
    Config::dictionary = new_dictionary;
}

void dbust::Config::set_target(const dbust::Target& new_target)
{
    Config::target = new_target;
}

void dbust::Config::set_nb_of_sockets(std::size_t new_nb_of_sockets)
{
    Config::nb_of_sockets = new_nb_of_sockets;
}

void dbust::Config::set_timeout(std::size_t new_timeout)
{
    Config::timeout = new_timeout;
}

void dbust::Config::set_get(bool new_get)
{
    Config::get = new_get;
}

void dbust::Config::set_user_agent(const std::string& new_user_agent)
{
    Config::user_agent = new_user_agent;
}

std::size_t dbust::Config::get_delay() const
{
    return delay;
}

void dbust::Config::set_delay(std::size_t delay)
{
    Config::delay = delay;
}

dbust::Config::Config() = default;

std::vector<std::string> dbust::MMapDictionaryReader::read_dictionary(std::string_view path) const
{
    int fd = ::open(path.data(), O_RDONLY);
    if (fd < 0) {
        std::cerr << "[!] Can't open file." << std::endl;
    }
    struct ::stat dict_stat {
    };
    if (fstat(fd, &dict_stat) < 0) {
        std::cerr << "[!] Can't evaluate file size." << std::endl;
    }
    ::__off_t dict_len { dict_stat.st_size };
    const char* mapped_addr {
        static_cast<const char*>(
            ::mmap(
                nullptr,
                dict_len,
                PROT_READ,
                MAP_PRIVATE,
                fd,
                0u))
    };
    std::string_view view(mapped_addr, dict_len);
    std::vector<std::string> res;
    std::string tmp_str;
    for (auto& c : view) {
        switch (c) {
        case ' ': {
            tmp_str += "%20";
            break;
        }
        case '#': {
            tmp_str += "%23";
            break;
        }
        case '%': {
            tmp_str += "%25";
            break;
        }
        case '"': {
            tmp_str += "%22";
            break;
        }
        case '\r':
            break;
        case '\n': {
            res.push_back(std::move(tmp_str));
            tmp_str = "";
            break;
        }
        default: {
            tmp_str.push_back(c);
        }
        }
    }
    return res;
}
