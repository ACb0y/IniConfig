//
// Created by ACb0y on 2022/3/19.
//

#pragma once

#include <stdint.h>
#include <map>
#include <vector>
#include <string>

namespace Config {

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
    std::vector<Value> result;
    if (data == "") {
      return result;
    }
    std::string::size_type prePos = 0;
    std::string::size_type curPos = data.find(separator);
    while (std::string::npos != curPos) {
      std::string subStr = data.substr(prePos, curPos - prePos);
      if (subStr != "") { //非空串才插入
        result.push_back(Value(subStr));
      }
      prePos = curPos + separator.size();
      curPos = data.find(separator, prePos);
    }
    if (prePos != data.length()) {
      result.push_back(Value(data.substr(prePos)));
    }
    return result;
  }

private:
  std::string data;
};

class Ini {
public:
  void Dump();
  bool Load(std::string fileName);
  bool GetStrValue(std::string section, std::string key, std::string& value);
private:
  void setSectionKeyValue(std::string& section, std::string& key, std::string & value);
  bool parseLine(std::string& line, std::string& section, std::string& key, std::string& value);
private:
  std::map<std::string, std::map<std::string, std::string> > cfg ;
};

}


