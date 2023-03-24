#include "storage/storage.h"
#include "table/table.h"
#include <iostream>

int main() {
    const size_t table_size = 83231;
    auto result = layla::Storage::init_database("/home/pedram/sources/cpp-layla/data",
                                                table_size, 5849, 256);
    auto index = layla::Table::get_key_index("pedram", table_size);
    layla::Storage::set(&result, index, "developer");
    auto value = layla::Storage::get(&result, index);
    std::cout << value << std::endl;
    return 0;
}
