#include "batch_opt_parser.hpp"

std::vector<std::string> dbust::BatchOptParser::parse(const std::string_view& opt) const
{
    auto f_opt =
            (opt[0] == '\'' && opt[opt.size() -1 ] == '\'') ||
            (opt[0] == '"' && opt[opt.size() - 1] == '\"') ?
            opt.substr(1, opt.length() - 2) : opt;
    std::vector<std::string> res;
    std::string tmp;
    for (const char c : f_opt)
    {
        if (c == ',')
        {
            res.push_back(std::move(tmp));
            tmp = "";
        } else
        {
            tmp.push_back(c);
        }
    }
    res.push_back(tmp);
    return res;
}
