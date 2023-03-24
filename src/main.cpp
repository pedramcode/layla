#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

#include "xxhash.h"

#define TABLE_SIZE 83231
#define BATCH_SIZE 5849
#define VALUE_SIZE 256

#define DB_PATH "/home/pedram/sources/cpp-layla/data"

unsigned int key_index(const std::string &key, size_t table_size);

void create_files(size_t table_size, size_t batch_size, size_t value_size);

size_t get_batch_index(size_t index);

void store_key(const std::string &key, const ::std::string &value);

std::string read_key(const std::string &key);


int main() {
    bool running = true;
    while (running) {
        std::cout << "0) Reset database" << std::endl;
        std::cout << "1) New record" << std::endl;
        std::cout << "2) Read record" << std::endl;
        std::cout << "3) Exit" << std::endl;

        unsigned int input;
        std::cout << "\tEnter your choice: #";
        std::cin >> input;
        std::cin.ignore();

        switch (input) {
            case 0:
                create_files(TABLE_SIZE, BATCH_SIZE, VALUE_SIZE);
                break;
            case 1: {
                std::string key, value;
                std::cout << "Enter key: ";
                std::getline(std::cin, key);
                std::cout << "Enter value: ";
                std::getline(std::cin, value);
                store_key(key, value);
                break;
            }
            case 2: {
                std::string key;
                std::cout << "Enter key: ";
                std::getline(std::cin, key);
                std::string value = read_key(key);
                std::cout << "Value: " << value << std::endl;
                break;
            }
            case 3:
                running = false;
                break;
            default:
                break;
        }
    }
    return 0;
}


void create_files(size_t table_size, size_t batch_size, size_t value_size) {
    const int num_batches = (table_size + batch_size - 1) / batch_size;

    char *buf = (char *) calloc(value_size * batch_size, sizeof(char));
    for (int i = 0; i < num_batches; i++) {
        int batch_start = i * batch_size;
        int batch_end = (i + 1) * batch_size - 1;
        if (batch_end >= table_size) {
            batch_end = table_size - 1;
        }

        std::string res = "data_" + std::to_string(i) + ".dat";
        std::fstream file;
        std::string path = DB_PATH;
        file.open(path.append("/").append(res), std::ios::binary | std::ios::out);
        file.write(buf, value_size * batch_size);
        file.close();
    }
    free(buf);
}


void store_key(const std::string &key, const std::string &value) {
    if (value.length() >= BATCH_SIZE) return;

    size_t index = key_index(key, TABLE_SIZE);
    auto file_index = get_batch_index(index);
    unsigned int record_index = index - (BATCH_SIZE * file_index);

    std::string file_name = "data_" + std::to_string(file_index) + ".dat";
    std::string path = DB_PATH;
    std::fstream file(path.append("/").append(file_name), std::ios::binary | std::ios::out);
    if (!file) return;
    file.seekp(record_index);
    file.write(value.c_str(), value.length());
    file.close();
}


std::string read_key(const std::string &key) {
    size_t index = key_index(key, TABLE_SIZE);
    auto file_index = get_batch_index(index);
    unsigned int record_index = index - (BATCH_SIZE * file_index);

    std::string file_name = "data_" + std::to_string(file_index) + ".dat";
    std::string path = DB_PATH;
    std::fstream file(path.append("/").append(file_name), std::ios::binary | std::ios::in);
    if (!file) return "";

    file.seekg(record_index);
    char res_c[BATCH_SIZE] = {0};
    file.read(res_c, BATCH_SIZE);
    std::string res(res_c);
    file.close();

    return res;
}


size_t get_batch_index(size_t index) {
    if (index < 0 || index > TABLE_SIZE) {
        return -1;
    }
    return index / BATCH_SIZE;
}


unsigned int key_index(const std::string &key, size_t table_size) {
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