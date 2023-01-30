#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

#include "row_reader.hpp"

namespace csv_reader::core
{
class TableReader
{
  public:
    TableReader()
    {
    }
    std::unordered_map<std::string, Row> read(std::string file_name);
};
} // namespace csv_reader::core
