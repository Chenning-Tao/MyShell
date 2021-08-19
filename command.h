//
// Created by 陶辰宁 on 2021/8/18.
//

#ifndef MYSHELL_COMMAND_H
#define MYSHELL_COMMAND_H
#include <iostream>
#include <unistd.h>
#include <string>
#include <pwd.h>
#include <vector>
#include <dirent.h>
#include <fstream>
#include <sstream>
using namespace std;

extern string MyError;
extern string MyOutput;
extern string argv;
void my_cd(const string& dir);
void my_dir(const string& dir);
void my_cat(const string& file);
void my_echo(const string& var);

#endif //MYSHELL_COMMAND_H
