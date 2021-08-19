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
using namespace std;

#define YELLOW "\e[1;33m"
#define CYAN "\e[0;36m"
#define WHITE "\e[1;37m"
#define RED "\e[0;31m"


#define HOST_NAME_LENGTH 100

// 重定向的结构
struct RedirectStructure{
    ifstream inFile;
    ofstream outFile;
    streambuf *oldIn{};
    streambuf *oldOut{};
    string inFileName;
    string outFileName;
};



#endif //MYSHELL_HEAD_H
