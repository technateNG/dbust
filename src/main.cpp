#include "main.hpp"

int main(const int argc, const char* argv[])
{
    dbust::MMapDictionaryReader mmap_d_read;
    dbust::BatchOptParser batch_opt_parser;
    dbust::CmdParser cmd_parser(mmap_d_read, batch_opt_parser);
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
    std::vector<dbust::Unit> units;
    units.reserve(config.get_nb_of_sockets());
    ::SSL_CTX* ctx{nullptr};
    if (config.get_target().is_ssl())
    {
        ::SSL_library_init();
        ::SSLeay_add_ssl_algorithms();
        ::SSL_load_error_strings();
        ctx = ::SSL_CTX_new(TLS_client_method());
        for (std::size_t i = 0; i < config.get_nb_of_sockets(); ++i)
        {
            dbust::Unit unit(
                    dbust::SslFlavour::instance(),
                    dbust::get_socket(*dns_result)
            );
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
            dbust::Unit unit(
                    dbust::HttpFlavour::instance(),
                    dbust::get_socket(*dns_result)
            );
            unit.prepare();
            units.emplace_back(unit);
            ::pollfd pfd{};
            pfd.fd = unit.get_file_descriptor();
            pfd.events = POLLIN | POLLOUT;
            polls.emplace_back(pfd);
        }
    }

    std::size_t word_ptr{0};
    std::size_t ext_pointer{0};
    std::size_t counter{0};
    const std::size_t one_percent = config.get_dictionary().size() / 100;
    while (word_ptr < config.get_dictionary().size())
    {
        poll(polls.data(), polls.size(), 0);
        for (std::size_t i = 0; i < polls.size(); ++i)
        {
            ::pollfd& pfd = polls.at(i);
            dbust::Unit& unit = units.at(i);
            if (unit.get_state() == dbust::Unit::State::SENDED && pfd.revents & POLLIN)
            {
                char buff[13]{'\0'};
                unit.receive(buff, 12);
                dbust::Response response(buff);
                if (counter % one_percent == 0)
                {
                    std::cerr << "[*] " << '(' << counter / one_percent << "%) " <<
                              counter << '/' << config.get_dictionary().size() << std::endl;
                }
                if (config.get_status_codes().contains(response.get_status_code_ptr()))
                {
                    std::string to_print = unit.get_path();
                    to_print.insert(0, 1, '/');
                    std::cout << to_print << std::endl;
                }
                unit.close();
                ++counter;
                int fd = dbust::get_socket(*dns_result);
                pfd.fd = fd;
                unit.set_file_descriptor(fd);
                unit.prepare();
            }
            else if (
                    unit.get_state() != dbust::Unit::State::SENDED &&
                    pfd.revents & POLLOUT
                    )
            {
                std::chrono::milliseconds time_diff =
                        std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::system_clock::now() - unit.get_delay_tp());
                if (time_diff.count() >= config.get_delay())
                {
                    unit.connect(*dns_result);
                    if (unit.get_state() != dbust::Unit::State::DICONNECTED)
                    {
                        if (unit.get_state() != dbust::Unit::State::BROKEN)
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
                        auto request = dbust::create_request(unit.get_path(), config);
                        unit.send(request);
                        unit.set_timeout_tp(std::chrono::system_clock::from_time_t(0));
                        unit.set_delay_tp(std::chrono::system_clock::now());
                    }
                    else
                    {
                        unit.close();
                        unit.set_timeout_tp(std::chrono::system_clock::from_time_t(0));
                        int fd = dbust::get_socket(*dns_result);
                        pfd.fd = fd;
                        unit.set_file_descriptor(fd);
                        unit.prepare();
                    }
                }
            }
            else
            {
                if (unit.get_timeout_tp().time_since_epoch().count())
                {
                    std::chrono::duration<double> duration =
                            std::chrono::system_clock::now() - unit.get_timeout_tp();
                    if (duration.count() > config.get_timeout())
                    {
                        unit.set_state(dbust::Unit::State::BROKEN);
                        std::cerr << "[!] Timeout hit for endpoint: " << unit.get_path() <<
                                  " Reconnect issued." << std::endl;
                        unit.close();
                        unit.set_timeout_tp(std::chrono::system_clock::from_time_t(0));
                        int fd = dbust::get_socket(*dns_result);
                        pfd.fd = fd;
                        unit.set_file_descriptor(fd);
                        unit.prepare();
                    }
                }
                else
                {
                    unit.set_timeout_tp(std::chrono::system_clock::now());
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