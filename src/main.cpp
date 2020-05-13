#include <unordered_set>
#include <vector>
#include <memory>
#include <alloca.h>
#include <poll.h>
#include <netdb.h>
#include "config.hpp"

std::optional<dbust::Config> get_config(const int argc, const char** argv)
{
    try {
        dbust::MMapDictionaryReader mmap_d_read;
        dbust::BatchOptParser batch_opt_parser;
        dbust::CmdParser cmd_parser(mmap_d_read, batch_opt_parser);
        dbust::Config config = cmd_parser.parse(argc, argv);
        return config;
    } catch (dbust::IncorrectUrlException& e) {
        std::string msg { "[!] Invalid url." };
        switch (e.get_part()) {
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
    } catch (dbust::ArgumentNotSetException& e) {
        std::string msg { "[!] Required argument isn't set: " };
        switch (e.get_arg()) {
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
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return std::nullopt;
    }
}

const ::addrinfo* resolve_address(const dbust::Config& config)
{
    ::addrinfo* dns_result;
    ::addrinfo hints {};
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
        &dns_result);
    return dns_result;
}

int main(const int argc, const char* argv[])
{
    const std::optional<dbust::Config> maybe_config { get_config(argc, argv) };
    if (!maybe_config.has_value()) {
        return 1;
    }
    const dbust::Config& config = maybe_config.value();

    const ::addrinfo* const dns_result = resolve_address(config);

    ::freeaddrinfo(const_cast<::addrinfo*>(dns_result));
    return EXIT_SUCCESS;
}
