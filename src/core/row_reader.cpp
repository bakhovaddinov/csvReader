#include "row_reader.hpp"
#include <sstream>
#include <string>

namespace csv_reader::core
{
void Row::parse(std::string line)
{
    std::stringstream input_row(line);
    std::string current_column;
    std::string row_index;

    while (std::getline(input_row, current_column, ',')) {
        if (!row_index.size()) {
            row_index = current_column;
        }
        values.emplace(stoi(row_index), stoi(current_column));
    }
};

int Row::operator[](int i)
{
    return values[i];
}
} // namespace csv_reader::core
