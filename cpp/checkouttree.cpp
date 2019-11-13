#include <fmt/core.h>
#include <fmt/format.h>
#include <zlib.h>

#include <algorithm>
#include <string_view>
#include <vector>
#include <fstream>

std::vector<unsigned char> get_object(std::string_view name)
{
    auto file_name = fmt::format(".git/objects/{}/{}", name.substr(0, 2), name.substr(2));
    std::ifstream ifs(file_name, std::ifstream::binary | std::ifstream::in);

    std::vector<unsigned char> input;
    std::copy(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), std::back_inserter(input));

    size_t len = input.size();
    std::vector<unsigned char> obj_data(2048, '\0');
    ::uncompress(obj_data.data(), &len, input.data(), input.size());
    obj_data.resize(len);

    return obj_data;
}

int main(int argc, char const *argv[])
{
    std::string_view tree_name(argv[1]);

    auto obj_data = get_object(tree_name);

    auto space = std::find(obj_data.begin(), obj_data.end(), ' ');
    auto null = std::find(space, obj_data.end(), '\0');

    auto start = null + 1;
    auto end = obj_data.end();

    while (std::distance(start, end) > 0)
    {
        auto space = std::find(start, end, ' ');
        auto null = std::find(space, end, '\0');


        std::string_view filename(reinterpret_cast<char*>(&*(space + 1)), std::distance(space, null));

        fmt::memory_buffer buf;
        for (auto it = null + 1; it < null + 21; ++it)
            fmt::format_to(buf, "{:02x}", *it);

        auto hash_str = fmt::to_string(buf);

        auto obj = get_object(hash_str);

        td::ofstream ofs(filename.data());
        ofs.write(reinterpret_cast<const char*>(obj.data()), obj.size());

        start = null + 21;
    }

    return 0;
}
