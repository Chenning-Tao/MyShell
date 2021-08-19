//
// Created by 陶辰宁 on 2021/8/18.
//


#include "command.h"
using namespace std;

void my_cd(const string& dir){
    // 如果切换成功返回0
    if (chdir(dir.c_str())) MyError = "Dir doesn't exist!";
}

void my_dir(const string& dir){
    // 打开目录
    DIR *d = opendir(dir.c_str());
    // 读入文件
    struct dirent *temp = readdir(d);
    while (temp != nullptr)
    {
        // 将内容加入
        string temp_result = temp->d_name;
        if(temp_result != "." && temp_result != "..") MyOutput += (temp_result + "  ");
        // 继续读取目录
        temp = readdir(d);
    }
    // 关闭目录
    closedir(d);
}

void my_cat(const string& file){
    ifstream temp(file.c_str());
    // 初始化流
    stringstream buffer;
    // 读取相应文件
    buffer << temp.rdbuf();
    // 添加结果进入
    MyOutput += (buffer.str());
}

void my_echo(const string& var){
    // 如果是$?进行变量替换
    if(var == "$?") MyOutput +=(argv + "  ");
    else MyOutput += (var + " ");
}

void my_test(const string& var1, const string& op, const string& var2){
    // 字符串相等
    if (op == "=") argv = (var1 == var2) ? "0" : "1";
    // 字符串不等
    if (op == "!=") argv = (var1 != var2) ? "0" : "1";
    // 数字相等
    if (op == "-eq") argv = (atoi(var1.c_str()) == atoi(var2.c_str())) ? "0" : "1";
    // 数字大于等于
    if (op == "-ge") argv = (atoi(var1.c_str()) >= atoi(var2.c_str())) ? "0" : "1";
    // 数字大于
    if (op == "-gt") argv = (atoi(var1.c_str()) > atoi(var2.c_str())) ? "0" : "1";
    // 数字小于等于
    if (op == "-le") argv = (atoi(var1.c_str()) <= atoi(var2.c_str())) ? "0" : "1";
    // 数字小于
    if (op == "-lt") argv = (atoi(var1.c_str()) < atoi(var2.c_str())) ? "0" : "1";
    // 数字不等
    if (op == "-ne") argv = (atoi(var1.c_str()) != atoi(var2.c_str())) ? "0" : "1";
}