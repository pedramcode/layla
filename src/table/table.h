#ifndef LAYLA_TABLE_H
#define LAYLA_TABLE_H

#include <string>
#include <xxhash.h>

namespace layla {
    class Table {
    public:
        static size_t get_key_index(const std::string key, size_t table_size);
    };
}


#endif //LAYLA_TABLE_H
