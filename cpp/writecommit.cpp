#include "writeobject.h"
#include "zstr.hpp"

#include <fmt/format.h>

#include <vector>
#include <array>

int main(int argc, char const *argv[])
{
    std::vector<char> commit_data;
    auto commit_bi = std::back_inserter(commit_data);

    std::string_view tree(argv[1]);
    std::string_view msg(argv[2]);

    fmt::format_to(commit_bi, "tree {}\n", tree);

    for (size_t i = 3; i < argc; ++i)
        fmt::format_to(commit_bi, "parent {}\n", argv[i]);

    std::string commit_header_format(
R"KEK(author Øystein Walle <oystein.walle@infrontfinance.com> 1571650134 +0200
committer Øystein Walle <oystein.walle@infrontfinance.com> 1571650134 +0200

{}
)KEK");

    fmt::format_to(commit_bi, commit_header_format, msg);

    std::vector<char> obj_data;
    auto obj_bi = std::back_inserter(obj_data);

    fmt::format_to(obj_bi, std::string("commit {:d}\0", 12), commit_data.size());
    std::copy(commit_data.begin(), commit_data.end(), obj_bi);

    auto hash_str = write_object(obj_data);
    fmt::print("{}\n", hash_str);

    return 0;
}

