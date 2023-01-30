#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

namespace csv_reader::core
{
class Row
{
  private:
    std::unordered_map<int, int> values;

  public:
    Row();
    ~Row();
    void parse(std::string line);
    int operator[](int i);
};
} // namespace csv_reader::core
