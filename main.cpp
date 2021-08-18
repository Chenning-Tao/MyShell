#include <iostream>
#include "Head.h"

int main(int argc, char **argv) {

    // 读入文件的指针
    FILE *fp;
    // 用来判断是否存在脚本
    bool script_flag = false;
    // 如果有文件参数传入
    if(argc > 1){
        fp = fopen(argv[1], "r");
        if(!fp){
            fprintf(stderr, )
        }
    }

//    // 主程序循环
//    while(1){
//
//    }
}
