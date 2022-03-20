//
// Created by ACb0y on 2022/3/19.
//

#include "Config.h"
#include <fstream>
#include <iostream>

namespace Config {

static void ltrim(std::string& str) {
  if (str.empty()) {
    return;
  }
  size_t len = 0;
  char * cStr = (char *)str.c_str();
  while (cStr && *cStr && isblank(*cStr)) {
    ++len;
    ++cStr;
  }
  if (len > 0) {
    str.erase(0, len);
  }
}

static void rtrim(std::string& str) {
  if (str.empty()) {
    return;
  }
  size_t len = str.length();
  size_t pos = len;
  while (pos > 0) {
    if (!isblank(str[pos - 1])) {
      break;
    }
    --pos;
  }
  if (pos != len) {
    str.erase(pos);
  }
}

static void trim(std::string& str) {
  ltrim(str);
  rtrim(str);
}

void Ini::Dump() {
  std::map<std::string, std::map<std::string, std::string> >::iterator iter = cfg.begin();
  while (iter != cfg.end()) {
    std::map<std::string, std::string>::iterator kvIter = iter->second.begin();
    while (kvIter != iter->second.end()) {
      std::cout << "section:[" << iter->first << "],keyValue:[" << kvIter->first << "=" << kvIter->second <<"]"<< std::endl;
      ++kvIter;
    }
    ++iter;
  }
}

bool Ini::Load(std::string fileName) {
  if (fileName == "") {
    return false;
  }
  std::ifstream in;
  std::string line;
  in.open(fileName.c_str());
  if (!in.is_open()) {
    return false;
  }
  while (getline(in, line)) {
    std::string section = "";
    std::string key = "";
    std::string value = "";
    if (!parseLine(line, section, key, value)) {
      continue;
    }
    setSectionKeyValue(section, key, value);
  }
  return true;
}

bool Ini::GetStrValue(std::string section, std::string key, std::string& value) {
  value = "";
  if (cfg.find(section) == cfg.end()) {
    return false;
  }
  if (cfg[section].find(key) == cfg[section].end()) {
    return false;
  }
  value = cfg[section][key];
  return true;
}

bool Ini::parseLine(std::string& line, std::string& section, std::string& key, std::string& value) {
  static std::string curSection = "";
  //去掉注释的内容
  std::string nodes[2] = {"#", ";"};
  for (int i = 0; i < 2; ++i) {
    std::string::size_type pos = line.find(nodes[i]);
    if (pos != std::string::npos) {
      line.erase(pos);
    }
  }
  trim(line);
  if (line == "") {
    return false;
  }
  if (line[0] == '[' && line[line.size() - 1] == ']') {
    section = line.substr(1,line.size() - 2);
    trim(section);
    curSection = section;
    return false;
  }
  if (curSection == "") {
    return false;
  }
  bool isKey = true;
  for (size_t i = 0; i < line.size(); ++i) {
    if (line[i] == '=') {
      isKey = false;
      continue;
    }
    if (isKey) {
      key += line[i];
    } else {
      value += line[i];
    }
  }
  section = curSection;
  trim(key);
  trim(value);
  return true;
}

void Ini::setSectionKeyValue(std::string & section, std::string & key, std::string & value) {
  if (cfg.find(section) == cfg.end()) {
    std::map<std::string, std::string> kvMap;
    cfg[section] = kvMap;
  }
  if (key != "" && value != "") {
    cfg[section][key] = value;
  }
}

}

