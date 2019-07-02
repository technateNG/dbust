#include "cmd_parser.hpp"

const std::string CmdParser::description
{
    "foobar"
};

Configuration& CmdParser::parse(int argc, char** argv) {
    ::option long_options[]
    {
            {"status_codes", optional_argument, nullptr, 'c'},
            {"file_extensions", optional_argument, nullptr, 'e'},
            {"sockets", optional_argument, nullptr, 's'},
            {"dictionary", required_argument, nullptr, 'w'},
            {"url", required_argument, nullptr, 'u'},
            {"timeout", optional_argument, nullptr, 't'},
            {"help", no_argument, nullptr, 'h'}
    };
    char c{ '\0' };
    int option_index{ 0 };
    Configuration::Builder config_builder;
    while ((c = getopt_long(argc, argv, "ctesw:u:", long_options, &option_index)) != 0)
    {
        switch (c)
        {
            case 'c':
                break;
            case 'e':
                break;
            case 's':
                config_builder.set_nb_of_sockets(std::stoi(::optarg));
                break;
            case 'w':
                config_builder.set_dictionary(load_dictionary(::optarg));
                break;
            case 'u':
                break;
            case 't':
                config_builder.set_timeout(std::stoi(::optarg));
                break;
            case 'h':
                std::cout << description << std::endl;
                break;
            default: {}
        }
    }
    return config_builder.build();
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
    std::string tmp_str { "/" };
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
            case '\n': {
                res.push_back(tmp_str);
                tmp_str = '/';
                break;
            }
            default: {
                tmp_str.push_back(c);
            }
        }
    }
    return res;
}