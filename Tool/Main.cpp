//
// Created by ACb0y on 2022/3/20.
//

#include <string>
#include <iostream>
#include "CmdLine.h"
#include "Config.h"

using namespace std;

string iniFile;     // ini格式的配置文件
string customClass; // 用户自定义的配置文件类名
string outputDir;   // 输出目录

void usage() {
  cout << "usage: configTool -iniFile cfgFile -customClass className" << endl;
  cout << "options:" << endl;
  cout << "   -h,--help       print usage" << endl;
  cout << "   -iniFile        config file" << endl;
  cout << "   -customClass    custom config class name" << endl;
  cout << "   -output         output dir" << endl;
  cout << endl;
}

int main(int argc, char* argv[]) {
  CmdLine::StrOptRequired(&iniFile, "iniFile");
  CmdLine::StrOptRequired(&customClass, "customClass");
  CmdLine::StrOpt(&outputDir, "output", "./");
  CmdLine::SetUsage(usage);
  CmdLine::Parse(argc, argv);

  Config::Ini iniConfig;
  if (!iniConfig.Load(iniFile)) {
    return -1;
  }
  iniConfig.Dump();

  return 0;
}
