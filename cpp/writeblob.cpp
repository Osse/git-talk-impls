#include "writeobject.h"
#include "zstr.hpp"

#include <fmt/format.h>

#include <iostream>
#include <vector>
#include <array>

int main(int argc, char const *argv[])
{
    std::array<char, 1024> buf;
    std::vector<char> data;

    while (true)
    {
        std::cin.read(buf.data(), buf.size());
        auto cnt = std::cin.gcount();
        if (cnt == 0)
            break;
        std::copy(buf.data(), buf.data() + cnt, std::back_inserter(data));
    }

    std::vector<char> obj_data;
    auto bi = std::back_inserter(obj_data);

    fmt::format_to(bi, std::string("blob {:d}\0", 10), data.size());
    std::copy(data.begin(), data.end(), bi);

    auto hash_str = write_object(obj_data);

    fmt::print("Written object: {}\n", hash_str);

    return 0;
}

