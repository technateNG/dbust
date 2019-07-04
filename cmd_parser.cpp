#include "cmd_parser.hpp"
#include "exceptions.hpp"

const std::string CmdParser::description
{
    "Usage: dbust [OPTION...]\n\n"
    "-u, --url <string> (*)                                               target url\n"
    "-d, --dictionary <string> (*)                                        path to dictionary\n"
    "-s, --sockets <int> (700)                                            number of concurrent sockets\n"
    "-t, --timeout <int> (10)                                             duration in seconds to reconnect try\n"
    "-e, --file-extensions <[string]> ()                                  file extensions to search for\n"
    "-c, --status-codes <[string]> (200, 201, 400, 401, 403, 500)         valid status codes\n"
    "--user-agent <string> (dbust)                                        request user agent\n"
    "--get                                                                use GET instead HEAD\n"
    "-h, --help                                                           this help\n\n"
    "Examples:\n"
    "dbust -u https://www.example.com/dir/ -w /home/user/dict.txt\n"
    "dbust -c '200,201,400,401,403,500' -e 'php,txt' -s 1000 -u http://192.168.0.92:8080/ -w /home/user/dict.txt\n"
};

Configuration& CmdParser::parse(int argc, char** argv) {
    int get{ 0 };
    ::option long_options[]
    {
            {"status-codes", required_argument, nullptr, 'c'},
            {"file-extensions", required_argument, nullptr, 'e'},
            {"sockets", required_argument, nullptr, 's'},
            {"dictionary", required_argument, nullptr, 'd'},
            {"url", required_argument, nullptr, 'u'},
            {"timeout", required_argument, nullptr, 't'},
            {"user-agent", required_argument, nullptr, 'a'},
            {"get", no_argument, &get, 1},
            {"help", no_argument, nullptr, 'h'}
    };
    char c{ '\0' };
    int option_index{ 0 };
    std::size_t bitmask{ 0 };
    Configuration::Builder builder;
    while ((c = getopt_long(argc, argv, "c:t:e:s:d:u:h", long_options, &option_index)) != -1)
    {
        switch (c)
        {
            case 'c':
            {
                auto sc_vec{ parse_delimited_opt(optarg) };
                std::unordered_set<std::string> sc_set(sc_vec.cbegin(), sc_vec.cend());
                builder.set_status_codes(sc_set);
                break;
            }
            case 'e':
            {
                auto ex_vec{ parse_delimited_opt(optarg) };
                for (auto& ex : ex_vec)
                {
                    ex.insert(0, 1, '.');
                }
                ex_vec.emplace_back("");
                builder.set_file_extensions(ex_vec);
                break;
            }
            case 's':
                builder.set_nb_of_sockets(std::stoi(::optarg));
                break;
            case 'd':
                builder.set_dictionary(load_dictionary(::optarg));
                bitmask |= 1u << 1u;
                break;
            case 'u':
                builder.set_target(Target::instance(::optarg));
                bitmask |= 1u << 0u;
                break;
            case 't':
                builder.set_timeout(std::stoi(::optarg));
                break;
            case 'a':
                builder.set_user_agent(::optarg);
                break;
            case 'h':
                std::cout << description << std::endl;
                std::exit(0);
            default: {}
        }
    }
    builder.set_get(get);
    switch (bitmask)
    {
        case 3:
            return builder.build();
        case 2:
            throw ArgumentNotSet("URL");
        case 1:
            throw ArgumentNotSet("Dictionary");
        case 0:
            throw ArgumentNotSet("URL, Dictionary");
        default:
            throw UnexpectedException();
    }
}


std::vector<std::string> CmdParser::parse_delimited_opt(const char* opt)
{
    std::string_view v_opt(opt);
    std::vector<std::string> res;
    std::string tmp;
    for (const char c : v_opt)
    {
        if (c == ',')
        {
            res.push_back(std::move(tmp));
            tmp = "";
        }
        else
        {
            tmp.push_back(c);
        }
    }
    res.push_back(tmp);
    return res;
}

std::vector<std::string> CmdParser::load_dictionary(const char* file_name)
{
    int fd = ::open(file_name, O_RDONLY);
    if (fd < 0)
    {
        std::cerr << "[!] Can't open file." << std::endl;
    }
    struct ::stat dict_stat{};
    if (fstat(fd, &dict_stat) < 0)
    {
        std::cerr << "[!] Can't evaluate file size." << std::endl;
    }
    ::__off_t dict_len { dict_stat.st_size };
    const char* mapped_addr{
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
    for (auto& c : view)
    {
        switch (c)
        {
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