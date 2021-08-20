//
// Created by 陶辰宁 on 2021/8/18.
//

#ifndef MYSHELL_HEAD_H
#define MYSHELL_HEAD_H
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <pwd.h>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <csignal>
#include <sys/wait.h>
using namespace std;

#define YELLOW "\e[1;33m"
#define CYAN "\e[0;36m"
#define WHITE "\e[1;37m"
#define RED "\e[0;31m"

#define HOST_NAME_LENGTH 100
#define RUNNING 1
#define DONE 2
#define STOP 3
#define COMPLETE 4
#define SUSPEND 5

// 重定向的结构
struct RedirectStructure{
    int in;
    int out;
    int in_backup;
    int out_backup;
    ifstream inFile;
    ofstream outFile;
    streambuf *oldIn{};
    streambuf *oldOut{};
    string inFileName;
    string outFileName;
};

// 进程结构
struct ProcessStruct{
    vector<pid_t> pid;
    vector<string> instruction;
    vector<int> status;
    vector<string> type;
};

#endif //MYSHELL_HEAD_H