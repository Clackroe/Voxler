#include "Asset.hpp"

// #include <stduuid.hpp>

namespace Dimensional {

// using uuid = uuids::uuid;

// uuids::uuid Asset::generateUUID()
// {
//     std::random_device rd;
//     auto seed_data = std::array<int, 6> {};
//     std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
//     std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
//     std::ranlux48_base generator(seq);
//     uuids::basic_uuid_random_generator<std::ranlux48_base> gen(&generator);
//     return gen();
// }
u64 Asset::generateHash(const std::string& path)
{
    return std::hash<std::string> {}(path);
}
//
// std::string Asset::idAsString() const
// {
//     return uuids::to_string(UID);
// }

}
