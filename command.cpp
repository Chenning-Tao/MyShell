//
// Created by 陶辰宁 on 2021/8/18.
//

#include "Head.h"
#include "command.h"
using namespace std;

extern RedirectStructure MyRedirect;
extern struct sigaction act;
extern ProcessStruct MyProcess;
extern union sigval MySigval;

string output_status(int status){
    if (status == STOP) return "STOP";
    if (status == SUSPEND) return "SUSPEND";
    if (status == DONE) return "DONE";
    if (status == RUNNING) return "RUNNING";
}

void my_exec_outside(vector<string> command){
    // 按照要求创建argument list
    char* argument_list[command.size()+1];
    for(int i = 0; i < command.size(); ++i){
        // 分配空间
        argument_list[i] = new char[command[1].length()];
        // 进行拷贝
        strcpy(argument_list[i], command[i].c_str());
    }
    // 最后一个是nullptr
    argument_list[command.size()] = nullptr;
    // 创建子进程
    pid_t pid = fork();
    // 如果是子进程
    if (pid == 0)
    {
        int fd_in, fd_out;
        // 打开重定向输入文件
        fd_in = open(MyRedirect.inFileName.c_str(), O_RDONLY);
        dup2(fd_in, STDIN_FILENO);
        // 打开重定向输出文件
        fd_out = open(MyRedirect.outFileName.c_str(), O_WRONLY);
        dup2(fd_out, STDOUT_FILENO);
        // 关闭文件
        close(fd_in);
        close(fd_out);
        // 如果执行不成功
        if(execvp(argument_list[0], argument_list) == -1)
            MyError = "Illegal instruction!";
        exit(0);
    }
    // 如果是父进程
    else if (pid > 0){
        // 加入进程
        MyProcess.pid.push_back(pid);
        MyProcess.instruction.push_back(command[0]);
        MyProcess.status.push_back(RUNNING);
        MyProcess.type.emplace_back("FG");
        int status;
        // 找到对应编号
        int i = 0;
        while (MyProcess.pid[i++] != pid);
        --i;
        // 等待进程结束并汇报状态
        waitpid(pid, &status, 0);
    }
}

void my_shift(int n){
    // 最多10位
    if (n > 10) arg->clear();
    else {
        // 移位
        for(int i = 0; i < 10-n; ++i){
            arg[i] = arg[n+i];
        }
    }
}

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

void my_more(const string& words){
    vector<string> lines;
    // 按照行分割
    split(words, lines, '\n');
    int pos = 1;
    while(pos <= lines.size()){
        // 输出
        cout << lines[pos++] << endl;
        // 每10行需要用户输入
        if (pos % 10 == 0)
        {
            cout << "more";
            getchar();
            cout << endl;
        }
    }
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

void my_umask(const string& mode){
    // 输出掩码
    if(mode.empty()){
        //定义pre_mask保存当前掩码
        mode_t pre_mask;
        //umask()函数返回当前掩码，保存到pre_mask中
        pre_mask = umask(0);
        //重新设置掩码为pre_mask
        umask(pre_mask);
        //输出当前掩码
        char b[100];
        sprintf(b, "%04d", pre_mask);
        MyOutput += b;
    }
    // 修改掩码
    else MyOutput = to_string(umask(atoi(mode.c_str())));
}

void my_handler(int sig, siginfo_t *info, void *p){
    int i;
    bool find = false;
    switch (sig) {
        // 处理中断
        case SIGTSTP:
             // 寻找前台应用
            i = MyProcess.pid.size() - 1;
            for(; i >= 0; --i){
                if (MyProcess.status[i] != COMPLETE && MyProcess.type[i] == "FG") {
                    find = true;
                    break;
                }
            }
            if (find){
                // 更改状态
                MyProcess.status[i] = SUSPEND;
                MyProcess.type[i] = "BG";
                // 将其挂起
                kill(MyProcess.pid[i], SIGSTOP);
                cout << endl;
            }
            break;
        // 子进程结束信号
        case SIGCHLD:
            // 找到序号
            for (i = 0; i < MyProcess.pid.size(); ++i){
                if(MyProcess.pid[i] == info->si_pid){
                    find = true;
                    break;
                }
            }
            if (find && info->si_status == 0){
                // 更改状态
                MyProcess.status[i] = DONE;
            }
            break;
    }
}

void my_fg(int pid){
    // 找到对应序号
    int i = 0;
    while (MyProcess.pid[i++] != pid);
    --i;
    // 如果进程不在运行
    if (MyProcess.status[i] != RUNNING){
        // 让进程继续运行
        kill(MyProcess.pid[i], SIGCONT);
        // 修改状态
        MyProcess.status[i] = RUNNING;
    }
    MyProcess.type[i] = "FG";
    // 输出状态
    cout << MyProcess.pid[i] << "  " << MyProcess.instruction[i] << "  " << output_status(MyProcess.status[i]) << endl;
    // 等待进程结束并汇报状态
    waitpid(pid, nullptr, 0);
}

void my_jobs(){
    for (int i = 0; i < MyProcess.pid.size(); ++i)
    {
        if (MyProcess.status[i] != COMPLETE)
            MyOutput += (to_string(MyProcess.pid[i]) + "  " + MyProcess.instruction[i] + "  " + output_status(MyProcess.status[i]) + "\n");
        // 已经完成的下次不显示
        if (MyProcess.status[i] == DONE) MyProcess.status[i] = COMPLETE;
    }
    // 去掉最后一个回车
    MyOutput = MyOutput.substr(0, MyOutput.size() - 1); //去掉最后一个回车
}

void my_bg(int pid){
    // 找到对应序号
    int i = 0;
    while (MyProcess.pid[i++] != pid);
    --i;
    // 发送继续运行信号
    kill(MyProcess.pid[i], SIGCONT);
    // 修改状态
    MyProcess.status[i] = RUNNING;
    MyProcess.type[i] = "BG";
    // 输出状态
    cout << MyProcess.pid[i] << "  " << MyProcess.instruction[i] << "  " << output_status(MyProcess.status[i]) << endl;
    waitpid(MyProcess.pid[i], nullptr, WNOHANG);
}