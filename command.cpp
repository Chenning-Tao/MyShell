//
// Created by 陶辰宁 on 2021/8/18.
//

#include "command.h"
using namespace std;

void my_cd(const string& dir){
    // 如果切换成功返回0
    if (chdir(dir.c_str())) MyError = "Dir doesn't exist!";
}

