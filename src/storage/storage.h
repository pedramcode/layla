#ifndef LAYLA_STORAGE_H
#define LAYLA_STORAGE_H

#include <cstdlib>
#include <string>
#include <filesystem>
#include <fstream>

namespace layla {

    typedef struct {
        std::string path;
        size_t table_size;
        size_t batch_size;
        size_t value_size;
    } StorageInfo;

    class Storage {
    public:
        static StorageInfo init_database(const std::string &path, size_t table_size, size_t batch_size,
                                         size_t value_size);

        static void set(const StorageInfo *storageInfo, size_t index, const std::string &value);
        static std::string get(const StorageInfo *storageInfo, size_t index);
    };
}

#endif //LAYLA_STORAGE_H
