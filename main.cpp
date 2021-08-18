#include <unistd.h>
#include "Head.h"

void DisplayPrompt();

void InitEnv();

using namespace std;
int main(int argc, char **argv) {
    // 读入文件的指针
    ifstream file;
    FILE *fp;
    // 用来判断是否存在脚本
    bool script_flag = false;
    // 如果有文件参数传入
    if(argc > 1){
        // 打开文件
        file.open(argv[1], ios::in);
        fp = fopen(argv[1], "r");
        // 打开文件失败
        if(!file){
            fprintf(stderr, RED "[MyShell] Error: Cannot open file named \"%s\"!\n", argv[1]);
        }
        // 设置从脚本读入
        else script_flag = true;
    }

    // 初始化环境变量
    InitEnv();

    // 主程序循环
    while(1){
        DisplayPrompt();
        string raw;
        if(script_flag){
            // 从脚本读入
            getline(file, raw);
        }
        else{
            // 从命令行读入
            getline(cin, raw);
        }

        break;
        // 对命令进行分割


        // 如果用户输入quit
        if(command[0] == "quit"){
            break;
        }
    }
}

void InitEnv() {
    // 声明结构变量pwd来保存含有用户名信息的passwd结构
    struct passwd *pwd;
    // getuid()函数获取用户id，getpwuid()函数根据用户id获取用户信息，保存到pwd中
    pwd = getpwuid(getuid());
    // 设置路径为home路径
    setenv("pwd", pwd->pw_dir, 1);
    setenv("path", pwd->pw_dir, 1);
    // 设置用户名
    setenv("username", pwd->pw_name, 1);
    char hostname[HOST_NAME_LENGTH];
    // 获取主机名
    gethostname(hostname, HOST_NAME_LENGTH);
    // 设置主机名
    setenv("hostname", hostname, 1);
}

void DisplayPrompt() {
    //控制颜色，输出命令提示符到屏幕
    printf(YELLOW "[MyShell]%s@%s", getenv("username"), getenv("hostname"));
    printf(WHITE ":");
    printf(CYAN "%s", getenv("path"));
    printf(WHITE "$ ");
}
