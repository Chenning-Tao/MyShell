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
#include <sys/stat.h>
#include <cstring>
#include <fcntl.h>
using namespace std;

extern string MyError;
extern string MyOutput;
extern string argv;
extern string arg[10];

void my_cd(const string& dir);
void my_dir(const string& dir);
void my_cat(const string& file);
void my_echo(const string& var);
void my_test(const string& var1, const string& op, const string& var2);
void my_more(const string& words);
void split(string raw, vector<string> &result, char sep);
void my_umask(const string& mode);
void my_shift(int n);
void my_exec_outside(vector<string> command);
void my_handler(int sig, siginfo_t *info, void *p);
void my_fg(int pid);
void my_jobs();
void my_bg(int pid);
string output_status(int status);
void my_background(vector<string> command);


#endif //MYSHELL_COMMAND_H
