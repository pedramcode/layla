#include "storage.h"

namespace layla {
    StorageInfo Storage::init_database(const std::string &path, size_t table_size, size_t batch_size,
                                       size_t value_size) {
        const size_t num_batches = (table_size + batch_size - 1) / batch_size;

        char *buf = (char *) calloc(value_size * batch_size, sizeof(char));
        for (size_t i = 0; i < num_batches; i++) {
            size_t batch_start = i * batch_size;
            size_t batch_end = (i + 1) * batch_size - 1;
            if (batch_end >= table_size) {
                batch_end = table_size - 1;
            }

            std::string file_name = "db_" + std::to_string(i) + ".dat";
            std::fstream file;
            std::filesystem::path file_path;
            file_path.append(path);
            file_path.append(file_name);
            file.open(file_path, std::ios::binary | std::ios::out);
            file.write(buf, value_size * batch_size);
            file.close();
        }
        free(buf);

        StorageInfo result = {
                path,
                table_size,
                batch_size,
                value_size
        };
        return result;
    }

    void Storage::set(const StorageInfo *storageInfo, size_t index, const std::string &value) {
        if (value.length() >= storageInfo->batch_size) return;

        if (index > storageInfo->table_size) {
            return; // TODO exception
        }
        auto file_index = index / storageInfo->batch_size;
        unsigned int record_index = index - (storageInfo->batch_size * file_index);

        std::string file_name = "db_" + std::to_string(file_index) + ".dat";
        std::filesystem::path path;
        path.append(storageInfo->path);
        path.append(file_name);
        std::fstream file(path, std::ios::binary | std::ios::out);
        if (!file) return; // TODO exception
        file.seekp(record_index);
        file.write(value.c_str(), value.length());
        file.close();
    }

    std::string Storage::get(const StorageInfo *storageInfo, size_t index) {
        if (index > storageInfo->table_size) {
            return ""; // TODO exception
        }
        auto file_index = index / storageInfo->batch_size;
        unsigned int record_index = index - (storageInfo->batch_size * file_index);

        std::string file_name = "db_" + std::to_string(file_index) + ".dat";
        std::filesystem::path path;
        path.append(storageInfo->path);
        path.append(file_name);
        std::fstream file(path, std::ios::binary | std::ios::in);
        if (!file) return ""; // TODO exception

        file.seekg(record_index);
        char *res_c = (char *) calloc(storageInfo->batch_size, sizeof(char));
        file.read(res_c, storageInfo->batch_size);
        std::string res(res_c);
        free(res_c);
        file.close();

        return res;
    }
}