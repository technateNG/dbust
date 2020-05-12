#include "dictionary_reader.hpp"
#include <fcntl.h>

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
