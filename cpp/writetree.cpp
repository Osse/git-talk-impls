#include "writeobject.h"

#include "fmt/format.h"

#include <vector>
#include <string_view>
#include <charconv>
#include <array>

int main(int argc, char const *argv[])
{
    std::vector<char> tree_data;
    auto tree_bi = std::back_inserter(tree_data);

    std::string tree_entry_format("100644 {}\0", 10);

    for (size_t i = 1; i < argc; ++i)
    {
        auto pos = std::string_view(argv[i]).find("=");
        auto name = std::string_view(argv[i], pos);

        fmt::format_to(tree_bi, tree_entry_format, name);

        std::string_view hash_str(argv[i] + pos + 1);

        std::array<unsigned char, 20> hash;
        for (size_t i = 0; i < 20; ++i)
            std::from_chars(hash_str.data() + 2*i, hash_str.data() + 2*i + 2, hash[i], 16);

        std::copy(hash.begin(), hash.end(), tree_bi);
    }

    std::vector<char> obj_data;
    auto obj_bi = std::back_inserter(obj_data);

    fmt::format_to(obj_bi, std::string("tree {:d}\0", 10), tree_data.size());
    std::copy(tree_data.begin(), tree_data.end(), obj_bi);

    auto hash_str = write_object(obj_data);
    fmt::print("Written object: {}\n", hash_str);

    return 0;
}

