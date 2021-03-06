//
// Created by ACb0y on 2022/3/14.
//

#include "CmdLine.h"
#include <map>
#include <stdint.h>
#include <stdio.h>
#include <string>

namespace CmdLine {

enum CommandLineOptType {
  INT64_T = 1,
  BOOL = 2,
  STRING = 3,
};

enum ParseOptResult {
  SUCC = 1,
  FAIL = 2,
};

struct CommandLineOptDefaultValue {
  bool boolValue;
  int64_t int64Value;
  std::string stringValue;
};

class CommandLineOpt {
public:
  CommandLineOpt() {
    // do nothing. for compile
  }
  CommandLineOpt(bool * value, std::string name, bool defaultValue, bool required) {
    this->type = BOOL;
    this->name = name;
    this->value = (void *)value;
    this->isValueSet = false;
    this->isOptSet = false;
    this->required = required;
    this->defaultValue.boolValue = defaultValue;
  }
  CommandLineOpt(int64_t * value, std::string name, int64_t defaultValue, bool required) {
    this->type = INT64_T;
    this->name = name;
    this->value = (void *)value;
    this->isValueSet = false;
    this->isOptSet = false;
    this->required = required;
    this->defaultValue.int64Value = defaultValue;
  }
  CommandLineOpt(std::string * value, std::string name, std::string defaultValue, bool required) {
    this->type = STRING;
    this->name = name;
    this->value = (void *)value;
    this->isValueSet = false;
    this->isOptSet = false;
    this->required = required;
    this->defaultValue.stringValue = defaultValue;
  }
  bool IsBoolOpt() {
    return type == BOOL;
  }
  void SetOptIsSet() {
    isOptSet = true;
  }
  void SetBoolValue(bool value) {
    this->isValueSet = true;
    *(bool*)this->value = value;
  }
  void SetValue(std::string value) {
    if (this->type == STRING) {
      *(std::string*)this->value = value;
    }
    if (this->type == INT64_T) {
      *(int64_t*)this->value = atoll(value.c_str());
    }
    this->isValueSet = true;
  }
  bool CheckRequired() {
    if (!required) {
      return true;
    }
    if (required && isValueSet) {
      return true;
    }
    if (not isOptSet) {
      printf("%s is required but not set\n", name.c_str());
      return false;
    }
    printf("required option %s not set argument\n", name.c_str());
    return false;
  }
  void SetDefaultValue() {
    if (isValueSet) {
      return;
    }
    if (type == BOOL) {
      *(bool*)value = defaultValue.boolValue;
    }
    if (type == INT64_T) {
      *(int64_t*)value = defaultValue.int64Value;
    }
    if (type == STRING) {
      *(std::string*)value = defaultValue.stringValue;
    }
  }

private:
  CommandLineOptType type;
  std::string name;
  void * value;
  bool isValueSet;
  bool isOptSet;
  bool required;
  CommandLineOptDefaultValue defaultValue;
};

static Usage _usage = nullptr;
static std::map<std::string, CommandLineOpt> _opts;

static bool isInvalidName(std::string name) {
  if (name == "") {
    return false;
  }
  if (name[0] == '-') {
    printf("option %s begins with -\n", name.c_str());
    return true;
  }
  if (name.find("=") != name.npos) {
    printf("option %s contains =\n", name.c_str());
    return true;
  }
  return false;
}

static ParseOptResult ParseOpt(int argc, char * argv[], int &parseIndex) {
  char * opt = argv[parseIndex];
  int optLen = strlen(opt);
  if (optLen <= 1) { // ?????????????????????>=2
    printf("option's len must greater than or equal to 2\n");
    return FAIL;
  }
  if (opt[0] != '-') { // ???????????????'-'??????
    printf("option must begins with '-', %s is invalid option\n", opt);
    return FAIL;
  }
  opt++; // ???????????????'-'
  optLen--;
  if (*opt == '-') {
    opt++; // ???????????????'-'
    optLen--;
  }
  // ??????????????????'-'???????????????check??????????????????????????????
  if (optLen == 0 || *opt == '-' || *opt == '=') {
    printf("bad opt syntax:%s\n", argv[parseIndex]);
    return FAIL;
  }

  // ?????????????????????????????????????????????????????????????????????????????????
  bool hasArgument = false;
  std::string argument = "";
  for (int i = 1; i < optLen; i++) {
    if (opt[i] == '=') {
      hasArgument = true;
      argument = std::string(opt + i + 1); // ?????????????????????????????????argument
      opt[i] = 0; // ??????opt????????????????????????'='??????????????????
      break;
    }
  }

  std::string optName = std::string(opt);
  if (optName == "help" || optName == "h") { // ???help????????????????????????_usage??????????????????
    _usage();
    exit(0);
  }

  std::map<std::string, CommandLineOpt>::iterator iter = _opts.find(optName);
  // ???????????????
  if (iter == _opts.end()) {
    printf("option provided but not defined: -%s\n", optName.c_str());
    return FAIL;
  }
  iter->second.SetOptIsSet();
  if (iter->second.IsBoolOpt()) { // ??????????????????bool????????????
    iter->second.SetBoolValue(true);
    parseIndex++; // parseIndex?????????????????????
  } else { // ??????????????????????????????????????????????????????????????????
    if (hasArgument) {
      parseIndex++;
    } else {
      if (parseIndex + 1 < argc) { // ???????????????????????????????????????
        hasArgument = true;
        argument = std::string(argv[parseIndex + 1]);
        parseIndex += 2; // parseIndex?????????????????????
      }
    }
    if (not hasArgument) {
      printf("option needs an argument: -%s\n", optName.c_str());
      return FAIL;
    }
    iter->second.SetValue(argument);
  }
  return SUCC;
}

static void CheckRequiredAndSetDefault() {
  std::map<std::string, CommandLineOpt>::iterator iter = _opts.begin();
  while (iter != _opts.end()) {
    if (!iter->second.CheckRequired()) {
      exit(-1);
    }
    iter->second.SetDefaultValue();
    iter++;
  }
}

void BoolOpt(bool* value, std::string name) {
  if (_opts.find(name) != _opts.end()) {
    printf("%s opt already set\n", name.c_str());
    exit(-1);
  }
  if (isInvalidName(name)) {
    printf("%s is invalid name\n", name.c_str());
    exit(-2);
  }
  _opts[name] = CommandLineOpt(value, name, false, false);
}

void Int64Opt(int64_t* value, std::string name, int64_t defaultValue) {
  if (_opts.find(name) != _opts.end()) {
    printf("%s opt already set\n", name.c_str());
    exit(-1);
  }
  if (isInvalidName(name)) {
    printf("%s is invalid name\n", name.c_str());
    exit(-2);
  }
  _opts[name] = CommandLineOpt(value, name, defaultValue, false);
}

void StrOpt(std::string* value, std::string name, std::string defaultValue) {
  if (_opts.find(name) != _opts.end()) {
    printf("%s opt already set\n", name.c_str());
    exit(-1);
  }
  if (isInvalidName(name)) {
    printf("%s is invalid name\n", name.c_str());
    exit(-2);
  }
  _opts[name] = CommandLineOpt(value, name, defaultValue, false);
}

void Int64OptRequired(int64_t* value, std::string name) {
  if (_opts.find(name) != _opts.end()) {
    printf("%s opt already set\n", name.c_str());
    exit(-1);
  }
  if (isInvalidName(name)) {
    printf("%s is invalid name\n", name.c_str());
    exit(-2);
  }
  _opts[name] = CommandLineOpt(value, name, 0, true);
}

void StrOptRequired(std::string* value, std::string name) {
  if (_opts.find(name) != _opts.end()) {
    printf("%s opt already set\n", name.c_str());
    exit(-1);
  }
  if (isInvalidName(name)) {
    printf("%s is invalid name\n", name.c_str());
    exit(-2);
  }
  _opts[name] = CommandLineOpt(value, name, "", true);
}

void SetUsage(Usage usage) {
  _usage = usage;
}

void Parse(int argc, char * argv[]) {
  if (nullptr == _usage) {
    printf("usage function not set\n");
    exit(-1);
  }
  // ???????????????????????????????????????parseIndex???1??????
  int parseIndex = 1;
  while (parseIndex < argc){
    int ret = ParseOpt(argc, argv, parseIndex);
    if (SUCC == ret) {
      continue;
    }
    exit(-2);
  }
  CheckRequiredAndSetDefault();
}

}
