#include "main.hpp"

int main(int argc, char* argv[])
{
    CmdParser cmd_parser;
    auto config = cmd_parser.parse(argc, argv);
    ::addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;

    ::addrinfo* dns_result;
    ::getaddrinfo(
            config.get_target().get_host().c_str(),
            config.get_target().get_port().c_str(),
            &hints,
            &dns_result
            );

    std::vector<pollfd> polls;
    polls.reserve(config.get_nb_of_sockets());
    std::vector<Unit> units;
    units.reserve(config.get_nb_of_sockets());
    ::SSL_CTX* ctx{ nullptr };
    if (config.get_target().is_ssl())
    {
        ::SSL_library_init();
        ::SSLeay_add_ssl_algorithms();
        ::SSL_load_error_strings();
         ctx = ::SSL_CTX_new(TLS_client_method());
        for (std::size_t i = 0; i < config.get_nb_of_sockets(); ++i)
        {
            Unit unit(SslFlavour::instance(), dbust::utils::get_socket(*dns_result));
            unit.set_ssl_ptr(::SSL_new(ctx));
            unit.prepare();
            units.emplace_back(unit);
            ::pollfd pfd{};
            pfd.fd = unit.get_file_descriptor();
            pfd.events = POLLIN | POLLOUT;
            polls.emplace_back(pfd);
        }
    }
    else
    {
        for (std::size_t i = 0; i < config.get_nb_of_sockets(); ++i)
        {
            Unit unit(HttpFlavour::instance(), dbust::utils::get_socket(*dns_result));
            unit.prepare();
            units.emplace_back(unit);
            ::pollfd pfd{};
            pfd.fd = unit.get_file_descriptor();
            pfd.events = POLLIN | POLLOUT;
            polls.emplace_back(pfd);
        }
    }

    std::size_t word_ptr{ 0 };
    std::size_t ext_pointer{ 0 };
    std::size_t counter{ 0 };
    const std::size_t one_procent = config.get_dictionary().size() / 100;
    while (word_ptr < config.get_dictionary().size())
    {
        poll(polls.data(), polls.size(), 0);
        for (std::size_t i = 0; i < polls.size(); ++i)
        {
            ::pollfd& pfd = polls.at(i);
            Unit& unit = units.at(i);
            if (unit.get_state() == Unit::State::SENDED && pfd.revents & POLLIN)
            {
                char buff[13] { '\0' };
                unit.receive(buff, 12);
                if (counter % one_procent == 0)
                {
                    std::cout << counter << '/' << config.get_dictionary().size() << std::endl;
                }
                if (dbust::utils::is_in_status_codes(buff, config.get_status_codes()))
                {
                    std::cout << unit.get_path() << std::endl;
                }
                unit.close();
                ++counter;
                int fd = dbust::utils::get_socket(*dns_result);
                pfd.fd = fd;
                unit.set_file_descriptor(fd);
                unit.prepare();
            }
            else if (unit.get_state() != Unit::State::SENDED && pfd.revents & POLLOUT)
            {
                unit.connect(*dns_result);
                if (unit.get_state() != Unit::State::DICONNECTED)
                {
                    if (unit.get_state() != Unit::State::BROKEN)
                    {
                        std::string t_path{config.get_dictionary()[word_ptr]};
                        t_path += config.get_file_extensions()[ext_pointer];
                        unit.set_path(t_path);
                        ++ext_pointer;
                        if (ext_pointer >= config.get_file_extensions().size())
                        {
                            ext_pointer = 0;
                            ++word_ptr;
                        }
                    }
                    auto request = dbust::utils::create_request(unit.get_path(), config);
                    unit.send(request);
                    unit.set_time_point(std::chrono::system_clock::from_time_t(0));
                }
                else
                {
                    unit.close();
                    unit.set_time_point(std::chrono::system_clock::from_time_t(0));
                    int fd = dbust::utils::get_socket(*dns_result);
                    pfd.fd = fd;
                    unit.set_file_descriptor(fd);
                    unit.prepare();
                }
            }
            else
            {
                if (unit.get_time_point().time_since_epoch().count())
                {
                    std::chrono::duration<double> duration =
                            std::chrono::system_clock::now() - unit.get_time_point();
                    if (duration.count() > config.get_timeout())
                    {
                        unit.set_state(Unit::State::BROKEN);
                        std::cerr << "[!] Timeout hit for endpoint: " << unit.get_path() <<
                        " Reconnect issued." << std::endl;
                        unit.close();
                        unit.set_time_point(std::chrono::system_clock::from_time_t(0));
                        int fd = dbust::utils::get_socket(*dns_result);
                        pfd.fd = fd;
                        unit.set_file_descriptor(fd);
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
    if (config.get_target().is_ssl())
    {
        ::SSL_CTX_free(ctx);
    }
    return EXIT_SUCCESS;
}