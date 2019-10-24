#include "zstr.hpp"

#include <fmt/format.h>
#include <openssl/sha.h>

#include <filesystem>
#include <array>

std::string write_object(std::vector<char> const& obj_data)
{
    std::array<unsigned char, 20> hash;
    ::SHA1(reinterpret_cast<const unsigned char*>(obj_data.data()), obj_data.size(), hash.data());

    std::string hash_str;
    hash_str.reserve(40);
    for (const auto d : hash)
        hash_str.append(fmt::format("{:02x}", d));

    std::filesystem::path obj_path(".git/objects");
    obj_path /= hash_str.substr(0, 2);

    std::filesystem::create_directories(obj_path);

    obj_path /= hash_str.substr(2);
    zstr::ofstream ofs(obj_path);

    ofs.write(obj_data.data(), obj_data.size());

    return hash_str;
}

