#include "Head.h"
#include "command.h"
using namespace std;

string MyError;
string MyOutput;
string MyInput;

// 重定向的结构
struct RedirectStructure{
    ifstream inFile;
    ofstream outFile;
    streambuf *oldIn{};
    streambuf *oldOut{};
    string inFileName;
    string outFileName;
};
RedirectStructure MyRedirect;

void DisplayPrompt();

void InitEnv();

void split(string raw, vector<string> &result, char sep);

int execute(const vector<string> &command);

void Redirect(vector<string> &command);

void ResumeRedirect();

int main(int argc, char **argv) {
    // 读入文件的指针
    ifstream file;
    // 用来判断是否存在脚本
    bool script_flag = false;
    // 如果有文件参数传入
    if(argc > 1){
        // 打开文件
        file.open(argv[1], ios::in);
        // 打开文件失败
        if(!file){
            fprintf(stderr, RED "[MyShell] Error: Cannot open file named \"%s\"!\n", argv[1]);
        }
        // 设置从脚本读入
        else script_flag = true;
    }

    // 初始化环境变量
    InitEnv();
    // 用来存储用｜分割的命令
    vector<string> command;
    // 用来存储非管道命令
    vector<string> inner_command;
    // 初始化重定向的输入输出流
    MyRedirect.oldIn = cin.rdbuf();
    MyRedirect.oldOut = cout.rdbuf();

    // 主程序循环
    while(true){
        string raw;
        if(script_flag){
            // 从脚本读入
            if(!getline(file, raw)){
                // 如果读完
                script_flag = false;
                continue;
            }
            else {
                // 将读入内容显示到命令窗口
                DisplayPrompt();
                cout << raw << endl;
            }
        }
        else{
            DisplayPrompt();
            // 从命令行读入
            getline(cin, raw);
        }

        // 先对命令按照管道划分
        split(raw, command, '|');

        // 处理上一轮留下的问题
        MyError.clear();

        for(int i = 0; i < command.size(); ++i){
            // 对命令按照空格划分
            split(command[i], inner_command, ' ');
            // 处理重定向
            Redirect(inner_command);
            // 调用函数执行命令 如果返回0代表函数结束
            if(!execute(inner_command)) exit(0);
            // 清理inner_command
            inner_command.clear();
            // 清理前一个的Input
            MyInput.clear();
            // 后一个的输入是前一个的输出
            MyInput = MyOutput;
            // 如果是最后一个进行输出
            if (i == command.size() - 1){
                if(!MyOutput.empty()) cout << MyOutput << endl;
                // 输出错误
                if(!MyError.empty()) cout << MyError << endl;
            }
            // 恢复重定向
            ResumeRedirect();
            MyOutput.clear();
        }
        command.clear();
    }
}

void ResumeRedirect() {
    // 关闭文件
    MyRedirect.inFile.close();
    MyRedirect.outFile.close();
    // 恢复旧的重定向
    cin.rdbuf(MyRedirect.oldIn);
    cout.rdbuf(MyRedirect.oldOut);
}

void Redirect(vector<string> &command) {
    int in = -1, out = -1;
    for(int i = 0; i < command.size(); ++i){
        // 如果不是最后一个指令
        if (i != command.size() - 1){
            // 输出重定向
            if (command[i] == ">" || command[i] == ">>"){
                // 存储旧的流
                MyRedirect.oldOut = cout.rdbuf(MyRedirect.outFile.rdbuf());
                // 设置输出内容的名字
                MyRedirect.outFileName = command[i+1];
                // 输出流设置
                if (command[i] == ">") MyRedirect.outFile.open(MyRedirect.outFileName, ios::out);
                if (command[i] == ">>") MyRedirect.outFile.open(MyRedirect.outFileName, ios::app);
                // 记录位置
                out = i;
            }
            // 输入重定向
            else if(command[i] == "<"){
                // 存储旧的流
                MyRedirect.oldIn = cin.rdbuf(MyRedirect.inFile.rdbuf());
                // 设置输入内容的名字
                MyRedirect.inFileName = command[i+1];
                // 输入流设置
                MyRedirect.inFile.open(MyRedirect.inFileName, ios::in);
                // 记录位置
                in = i;
            }
        }
        // 如果只有重定向没有文件名
        else if (command[i] == ">" || command[i] == ">>" || command[i] == "<"){
            MyError = "No redirect file specified!";
        }
    }
    if (in < out){
        // 只有输出需要重定向
        if(in == -1){
            command.erase(command.begin()+out);
            command.erase(command.begin()+out);
        }
        // 都需要重定向
        else {
            command.erase(command.begin()+in);
            command.erase(command.begin()+in);
            command.erase(command.begin()+out-2);
            command.erase(command.begin()+out-2);
        }
    }
    else if(out < in){
        // 只有输入需要重定向
        if(out == -1){
            command.erase(command.begin()+in);
            command.erase(command.begin()+in);
        }
        // 都需要重定向
        else {
            command.erase(command.begin()+out);
            command.erase(command.begin()+out);
            command.erase(command.begin()+in-2);
            command.erase(command.begin()+in-2);
        }
    }
}

// 需要执行下列命令：bg、cd 、clr、dir、echo 、exec 、exit 、fg 、help、jobs 、pwd 、set 、shift 、test 、time 、umask、unset
int execute(const vector<string> &command) {
    string main_command = command[0];
    if (main_command == "exit"){
        printf(CYAN "[MyShell] Good bye!\n");
        return 0;
    }
    else if (main_command == "bg"){

    }
    // 更改目录位置
    else if (main_command == "cd"){
        // 如果参数过多
        if(command.size() > 2) MyError = "Too many argument!";
        // 回到home目录
        else if(command.size() == 1) my_cd(getenv("home"));
        // 跳转到指定目录
        else my_cd(command[1]);
    }
    // 清除屏幕内容
    else if (main_command == "clr"){
        if (command.size() != 1) MyError = "Too many argument!";
        else printf ("\033c");
    }
    else if (main_command == "dir"){

    }
    else if (main_command == "echo"){

    }
    else if (main_command == "exec"){

    }
    else if (main_command == "fg"){

    }
    else if (main_command == "help"){

    }
    else if (main_command == "jobs"){

    }
    // 显示当前目录
    else if (main_command == "pwd"){
        if (command.size() != 1) MyError = "Too many argument!";
        else {
            // 获取当前目录位置
            char current_dir[300];
            getcwd(current_dir, 300);
            // 输出当前目录位置
            MyOutput = current_dir;
        }
    }
    else if (main_command == "set"){

    }
    else if (main_command == "shift"){

    }
    else if (main_command == "test"){

    }
    // 显示当前时间
    else if (main_command == "time"){
        if (command.size() != 1) MyError = "Too many argument!";
        else {
            // 基于当前系统的当前日期/时间
            time_t now = time(nullptr);
            // 把 now 转换为字符串形式
            MyOutput = ctime(&now);
        }
    }
    else if (main_command == "umask"){

    }
    else if (main_command == "unset"){

    }
    else {
        MyError = "Illegal instruction!";
    }
    return 1;
}

void split(string raw, vector<string> &result, char sep) {
    // 当前字符串的起始位置
    int begin = 0;
    // 在最后补上分隔符
    raw.push_back(sep);
    for (int i = 0; i < raw.size(); ++i)
    {
        if(raw[i] == sep){
            // 如果不是最后一个
            if(begin < i) result.push_back(raw.substr(begin, (i - begin)));
            // 更新位置
            begin = i + 1;
        }
    }
}

void InitEnv() {
    // 声明结构变量pwd来保存含有用户名信息的passwd结构
    struct passwd *pwd;
    // getuid()函数获取用户id，getpwuid()函数根据用户id获取用户信息，保存到pwd中
    pwd = getpwuid(getuid());
    // 设置路径为home路径
    setenv("home", pwd->pw_dir, 1);
    // 设置用户名
    setenv("username", pwd->pw_name, 1);
    char hostname[HOST_NAME_LENGTH];
    // 获取主机名
    gethostname(hostname, HOST_NAME_LENGTH);
    // 设置主机名
    setenv("hostname", hostname, 1);
}

void DisplayPrompt() {
    // 控制颜色，输出命令提示符到屏幕
    printf(YELLOW "[MyShell] %s@%s", getenv("username"), getenv("hostname"));
    printf(WHITE ":");
    // 获取当前目录位置
    char current_dir[300];
    getcwd(current_dir, 300);
    // 输出当前目录位置
    printf(CYAN "%s", current_dir);
    printf(WHITE "$ ");
}

