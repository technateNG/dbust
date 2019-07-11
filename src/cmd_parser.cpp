#include "cmd_parser.hpp"

const std::string dbust::CmdParser::description
        {
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
                "dbust -u https://www.example.com/dir/ -w /home/user/dict.txt\n"
                "dbust -c '200,201,400,401,403,500' -e 'php,txt' -s 1000 -u http://192.168.0.92:8080/ -w /home/user/dict.txt\n"
        };

dbust::Config dbust::CmdParser::parse(int argc, const char* argv[])
{
    int get{0};
    ::option long_options[]
            {
                    {"status-codes",    required_argument, nullptr, 'c'},
                    {"file-extensions", required_argument, nullptr, 'e'},
                    {"sockets",         required_argument, nullptr, 's'},
                    {"dictionary",      required_argument, nullptr, 'd'},
                    {"url",             required_argument, nullptr, 'u'},
                    {"timeout",         required_argument, nullptr, 't'},
                    {"delay",           required_argument, nullptr, 'y'},
                    {"user-agent",      required_argument, nullptr, 'a'},
                    {"get",             no_argument,       &get,    1},
                    {"help",            no_argument,       nullptr, 'h'}
            };
    char c{'\0'};
    int option_index{0};
    std::size_t bitmask{0};
    Config config;
    while ((c = getopt_long(argc, const_cast<char **>(argv), "y:c:t:e:s:d:u:h", long_options, &option_index)) != -1)
    {
        switch (c)
        {
            case 'c':
            {
                auto sc_vec{parser.parse(optarg)};
                StatusCodes sc(sc_vec.cbegin(), sc_vec.cend());
                config.set_status_codes(sc);
                break;
            }
            case 'e':
            {
                auto ex_vec{parser.parse(optarg)};
                for (auto& ex : ex_vec)
                {
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
            case 'u':
                config.set_target(Target(::optarg));
                bitmask |= 1u << 0u;
                break;
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
            default:
            {
            }
        }
    }
    config.set_get(get);
    switch (bitmask)
    {
        case 3:
            return config;
        case 2:
            throw dbust::ArgumentNotSetException("URL");
        case 1:
            throw dbust::ArgumentNotSetException("Dictionary");
        case 0:
            throw dbust::ArgumentNotSetException("URL, Dictionary");
        default:
            throw dbust::UnexpectedException();
    }
}

dbust::CmdParser::CmdParser(const dbust::DictionaryReader& d_reader,
                                    const dbust::BatchOptParser& parser) : reader{ d_reader }, parser{ parser }
{
}