#include <bits/unique_ptr.h>
#include "main.hpp"

std::optional<dbust::Config> get_config(const int argc, const char** argv)
{
    try
    {
        dbust::MMapDictionaryReader mmap_d_read;
        dbust::BatchOptParser batch_opt_parser;
        dbust::CmdParser cmd_parser(mmap_d_read, batch_opt_parser);
        dbust::Config config = cmd_parser.parse(argc, argv);
        return config;
    }
    catch (dbust::IncorrectUrlException& e)
    {
        std::string msg{ "[!] Invalid url." };
        switch (e.get_part())
        {
            case dbust::IncorrectUrlException::Part::PROTOCOL:
                msg += "Specified protocol in url is incorrect. Supported are only http and https.";
                break;
            case dbust::IncorrectUrlException::Part::CONVENTION:
                msg += "There is a mistake in '://' part.";
                break;
            case dbust::IncorrectUrlException::Part::PORT:
                msg += "Port is non numeric.";
        }
        std::cerr << msg << std::endl;
        std::cout << dbust::CmdParser::description << std::endl;
        return std::nullopt;
    }
    catch (dbust::ArgumentNotSetException& e)
    {
        std::string msg{"[!] Required argument isn't set: "};
        switch (e.get_arg())
        {
            case dbust::ArgumentNotSetException::Arg::DIRECTORY:
                msg += "dictionary (-d --dictionary)";
                break;
            case dbust::ArgumentNotSetException::Arg::URL:
                msg += "url (-u --url)";
                break;
            case dbust::ArgumentNotSetException::Arg::ALL:
                msg += "dicionary (-d --dictionary), url (-u --url)";
        }
        std::cerr << msg << std::endl;
        std::cout << dbust::CmdParser::description << std::endl;
        return std::nullopt;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return std::nullopt;
    }
}

const ::addrinfo* resolve_address(const dbust::Config& config)
{
    ::addrinfo* dns_result;
    ::addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;

    ::getaddrinfo(
            config.get_target().get_host().c_str(),
            config.get_target().get_port().c_str(),
            &hints,
            &dns_result
    );
    return dns_result;
}

void pollout_branch(
        const dbust::Config& config,
        const addrinfo* dns_result,
        std::size_t& word_ptr,
        std::size_t& ext_pointer,
        const std::string& req_type,
        dbust::Unit& unit
        )
{
    using namespace std::literals::string_view_literals;
    std::chrono::milliseconds time_diff =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now() - unit.get_delay_tp());
    if (time_diff.count() >= config.get_delay())
    {
        if (unit.get_state() == dbust::Unit::State::DISCONNECTED)
        {
            unit.set_state(dbust::Unit::State::EMPTY);
        }
        else
        {
            unit.connect(*dns_result);
        }
        if (unit.get_state() != dbust::Unit::State::DISCONNECTED_WITH_ERROR &&
        unit.get_state() != dbust::Unit::State::DISCONNECTED)
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
            const std::size_t req_size =
                    req_type.length() +
                    config.get_target().get_resource_path().length() +
                    unit.get_path().length() +
                    19 +
                    config.get_target().get_host().length() +
                    16 +
                    config.get_user_agent().length() +
                    23;
            char* req_buff { reinterpret_cast<char*>(alloca(req_size)) };
            std::size_t idx{0};
            dbust::fill_buffer(
                    req_buff,
                    idx,
                    req_type,
                    config.get_target().get_resource_path(),
                    unit.get_path(),
                    " HTTP/1.1\r\nHost: "sv,
                    config.get_target().get_host(),
                    "\r\nUser-Agent: "sv,
                    config.get_user_agent(),
                    "\r\nAccept: */*\r\n\r\n"sv
            );
            std::string_view request(req_buff, req_size);
            unit.send(request);
            unit.set_timeout_tp(std::chrono::system_clock::from_time_t(0));
            unit.set_delay_tp(std::chrono::system_clock::now());
        }
        else if (unit.get_state() == dbust::Unit::State::DISCONNECTED_WITH_ERROR)
        {
            unit.close();
            unit.set_timeout_tp(std::chrono::system_clock::from_time_t(0));
            int fd = dbust::get_socket(*dns_result);
            unit.set_file_descriptor(fd);
            unit.prepare();
        }
    }
}

int main(const int argc, const char* argv[])
{
    const std::optional<dbust::Config> maybe_config{ get_config(argc, argv) };
    if (!maybe_config.has_value())
    {
        return 1;
    }
    const dbust::Config& config = maybe_config.value();

    const ::addrinfo* const dns_result = resolve_address(config);

    dbust::UnitPool units(config.get_nb_of_sockets());

    ::SSL_CTX* ctx{nullptr};

    if (config.get_target().is_ssl())
    {
        ::SSL_library_init();
        ::SSLeay_add_ssl_algorithms();
        ::SSL_load_error_strings();
        ctx = ::SSL_CTX_new(TLS_client_method());
        for (std::size_t i = 0; i < config.get_nb_of_sockets(); ++i)
        {
            units.initialize_unit(dbust::SslFlavour::instance());
            units[i].set_ssl_ptr(::SSL_new(ctx));
            units[i].set_file_descriptor(dbust::get_socket(*dns_result));
            units[i].set_poll_event_flags(POLLIN | POLLOUT);
            units[i].prepare();
        }
    }
    else
    {
        for (std::size_t i = 0; i < config.get_nb_of_sockets(); ++i)
        {
            units.initialize_unit(dbust::HttpFlavour::instance());
            units[i].set_file_descriptor(dbust::get_socket(*dns_result));
            units[i].set_poll_event_flags(POLLIN | POLLOUT);
            units[i].prepare();
        }
    }

    std::size_t word_ptr{0};
    std::size_t ext_pointer{0};
    std::size_t counter{0};
    const std::size_t one_percent{ config.get_dictionary().size() / 100 };
    const std::string req_type{config.is_get() ? "GET " : "HEAD " };
    while (word_ptr < config.get_dictionary().size())
    {
        poll(units.get_polls(), units.size(), 0);
        for (dbust::Unit& unit : units)
        {
            if (unit.get_state() == dbust::Unit::State::SENDED &&
            unit.get_poll_revents() & POLLIN)
            {
                char buff[13]{'\0'};
                unit.receive(buff, 12);
                dbust::Response response(buff);
                if (counter % one_percent == 0)
                {
                    std::cerr << "[*] " << '(' << counter / one_percent << "%) " <<
                              counter << '/' << config.get_dictionary().size() << '\n';
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
                unit.set_file_descriptor(fd);
                unit.prepare();
            }
            else if (
                    unit.get_state() != dbust::Unit::State::SENDED &&
                    unit.get_poll_revents() & POLLOUT
                    )
            {
                pollout_branch(config, dns_result, word_ptr, ext_pointer, req_type, unit);
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
    ::freeaddrinfo(const_cast<::addrinfo*>(dns_result));
    if (config.get_target().is_ssl())
    {
        ::SSL_CTX_free(ctx);
    }
    return EXIT_SUCCESS;
}