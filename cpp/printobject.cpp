#include "zstr.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>

int main(int argc, char const *argv[])
{
    zstr::istreambuf zsbuf(std::cin.rdbuf(), 1<<16, true);
    std::istream is(&zsbuf);
    is.exceptions(std::ios_base::badbit);

    std::string obj_data{std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>()};

    auto space = std::find(obj_data.begin(), obj_data.end(), ' ');
    auto null = std::find(space, obj_data.end(), '\0');

    std::string_view type(obj_data.data(), std::distance(obj_data.begin(), space));

    auto content_len = obj_data.size() - std::distance(obj_data.begin(), null) - 1;
    std::string_view content(obj_data.data() + std::distance(obj_data.begin(), null + 1), content_len);

    fmt::print("Type: {}\nContent:\n{}", type, content);

    return 0;
}
