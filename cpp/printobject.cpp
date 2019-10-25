#include <fmt/core.h>
#include <zlib.h>

#include <algorithm>
#include <cstdio>
#include <string_view>
#include <vector>

int main(int argc, char const *argv[])
{
    std::vector<unsigned char> input(1024, '\0');

    auto size = std::fread(input.data(), 1, 1024, stdin);
    input.resize(size);

    size_t len = 2048;
    std::vector<unsigned char> obj_data(len, '\0');
    ::uncompress(obj_data.data(), &len, input.data(), input.size());
    obj_data.resize(len);

    auto space = std::find(obj_data.begin(), obj_data.end(), ' ');
    auto null = std::find(space, obj_data.end(), '\0');

    std::string_view type(reinterpret_cast<char*>(obj_data.data()), std::distance(obj_data.begin(), space));

    auto content_len = obj_data.size() - std::distance(obj_data.begin(), null) - 1;
    std::string_view content(reinterpret_cast<char*>(obj_data.data()) + std::distance(obj_data.begin(), null + 1), content_len);

    fmt::print("Type: {}\nContent:\n{}", type, content);

    return 0;
}
