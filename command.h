//
// Created by 陶辰宁 on 2021/8/18.
//

#ifndef MYSHELL_COMMAND_H
#define MYSHELL_COMMAND_H
#include <iostream>
#include <unistd.h>
#include <string>
#include <pwd.h>
using namespace std;

extern string MyError;
extern string MyOutput;
void my_cd(const string& dir);

#endif //MYSHELL_COMMAND_H
