#include "table.h"

namespace layla {
    size_t Table::get_key_index(const std::string key, size_t table_size) {
        if (key.empty()) return 0;

        size_t input_size = key.size();
        unsigned int seed = 0;
        unsigned int hash_value = XXH32(key.c_str(), input_size, seed);

        char first = key.front();
        char middle = key[key.size() / 2];
        char last = key.back();

        size_t hash_value_character = (size_t) first * 31 * 31 + (size_t) middle * 31 + (size_t) last;
        size_t hash_value_computed = hash_value_character * 31 + (size_t) hash_value;
        return hash_value_computed % table_size;
    }
}