#include "table_reader.hpp"

#include <filesystem>
#include <iostream>

namespace csv_reader::core
{
int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Invalid number of params" << argc << std::endl;
        return EXIT_FAILURE;
    }
    std::string file_name = argv[1];
    if (!std::filesystem::exists(file_name)) {
        std::cout << "File does not exist" << std::endl;
        return EXIT_FAILURE;
    }
    auto table = CsvTable(file_name);
    table.print();
    return EXIT_SUCCESS;
}
} // namespace csv_reader::core
