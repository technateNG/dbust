#include "batch_opt_parser.hpp"

std::vector<std::string> dbust::models::BatchOptParser::parse(const char* opt) const
{
    std::string_view r_opt(opt);
    auto f_opt =
            (r_opt[0] == '\'' && r_opt[r_opt.size() -1 ] == '\'') ||
            (r_opt[0] == '"' && r_opt[r_opt.size() - 1] == '\"') ?
            r_opt.substr(1, r_opt.length() - 2) : r_opt;
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
