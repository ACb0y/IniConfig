//
// Created by ACb0y on 2022/3/19.
//

#pragma once

#include <stdint.h>
#include <string>

namespace IniConfig {

class Value {
public:
  Value(std::string data) {
    this->data = data;
  }
  int32_t Int32() {
    return std::stoi(data.c_str());
  }
  int64_t Int64() {
    return std::stoll(data.c_str());
  }
  uint32_t UInt32() {
    return uint32_t(std::stoi(data.c_str()));
  }
  uint64_t UInt64() {
    return uint64_t(std::stoll(data.c_str()));
  }
  bool Bool() {
    if (data == "1" || data == "t" || data == "T" || data == "true"||
        data == "TRUE" || data == "True") {
      return true;
    }
    return false;
  }
  std::string String() {
    return data;
  }
  std::vector<Value> Split(std::string separator) {

  }

private:
  std::string data;
};

class Config {

};

class Section {

};

}


