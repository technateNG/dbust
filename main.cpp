#include "main.hpp"

inline int get_socket(addrinfo& addrinfo)
{
    return ::socket(addrinfo.ai_family, addrinfo.ai_socktype, addrinfo.ai_protocol);
}

std::string create_request(std::string& path)
{
    std::string res{ "HEAD " };
    res += path;
    res += " HTTP/1.1\r\nHost: ";
    res += "192.168.122.134";
    res += "\r\nUser-Agent: ";
    res += "curl/7.58.0";
    res += "\r\nAccept: */*\r\n\r\n";
    //"HEAD " +
    //path +
    //" HTTP/1.1\r\nHost: " +
    //host +
    //"\r\nUser-Agent: " +
    //ua +
    //"\r\nAccept: */*\r\n\r\n";
    //res.reserve(53 + host.len + ua.len + path.length());*/
    return res;
}

bool is_in_status_codes(const char* buff, std::unordered_set<std::string>& status_codes) {
    std::string_view buff_view(buff);
    std::string_view sc{ buff_view.substr(9, 3) };
    const char* scd = sc.data();
    int res = status_codes.count(scd);
    return res;
}



int main(int argc, char* argv[])
{
    CmdParser cmd_parser;
    auto c = cmd_parser.parse(argc, argv);
    std::cout << c.get_host() << std::endl;
    ::addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;

    ::addrinfo* dns_result;
    ::getaddrinfo("192.168.122.134", "443", &hints, &dns_result);

    ::SSL_library_init();
    ::SSLeay_add_ssl_algorithms();
    ::SSL_load_error_strings();
    ::SSL_CTX* ctx = ::SSL_CTX_new(TLS_client_method());

    std::vector<std::string> dictionary
    {"/f1", "/f2", "/f3", "/f4", "/f5", "/f6", "/f7", "/f8"};
    std::unordered_set<std::string> status_codes
    {"200", "201", "301", "302", "500", "401", "403"};
    std::vector<pollfd> polls;
    polls.reserve(2);
    std::vector<Unit> units;
    units.reserve(2);
    for (std::size_t i = 0; i < 2; ++i)
    {
        Unit unit(SslFlavour::instance(*ctx), get_socket(*dns_result));
        unit.prepare();
        units.emplace_back(unit);
        ::pollfd pfd{};
        pfd.fd = unit.get_file_descriptor();
        pfd.events = POLLIN | POLLOUT;
        polls.emplace_back(pfd);
    }
    std::size_t word_ptr{ 0 };
    while (word_ptr < dictionary.size())
    {
        poll(polls.data(), polls.size(), 0);
        for (std::size_t i = 0; i < polls.size(); ++i)
        {
            ::pollfd& pfd = polls.at(i);
            Unit& unit = units.at(i);
            if (unit.get_state() == Unit::State::SENDED && pfd.revents & POLLIN)
            {
                char buff[12] { '\0' };
                unit.receive(buff, 12);
                if (is_in_status_codes(buff, status_codes))
                {
                    std::cout << unit.get_path() << std::endl;
                }
                unit.close();
                int fd = get_socket(*dns_result);
                pfd.fd = fd;
                unit.set_file_descriptor(fd);
                unit.prepare();
            }
            else if (unit.get_state() == Unit::State::EMPTY && pfd.revents & POLLOUT)
            {
                unit.connect(*dns_result);
                if (unit.get_state() != Unit::State::EXCEPTION)
                {
                    unit.set_path(dictionary.at(word_ptr));
                    ++word_ptr;
                }
                auto request = create_request(unit.get_path());
                unit.send(request);
            }
            else
            {
                if (unit.get_time_point().time_since_epoch().count())
                {
                    std::chrono::duration<double> duration =
                            std::chrono::system_clock::now() - unit.get_time_point();
                    if (duration.count() > 10)
                    {
                        unit.set_state(Unit::State::EXCEPTION);
                        unit.close();
                        int fd = get_socket(*dns_result);
                        pfd.fd = fd;
                        unit.set_file_descriptor(get_socket(*dns_result));
                        unit.prepare();
                    }
                }
                else
                {
                    unit.set_time_point(std::chrono::system_clock::now());
                }
            }
        }
    }
    ::freeaddrinfo(dns_result);
    ::SSL_CTX_free(ctx);
    return EXIT_SUCCESS;
}