#include "mywidget.h"
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <execinfo.h>
#include <unistd.h>
#include <signal.h>
#include <sys/reboot.h>

void dump(int signo)
{
    void *array[10];
    size_t size;
    char **strings;
    size_t i;
    ;
    size = backtrace (array, 10);
    strings = backtrace_symbols (array, size);
    printf ("Obtained %zd stack frames.\n", size);
    for(i = 0; i<=size; i++){
        printf ("%s\n", strings[i]);
    }
    free (strings);
    //system("reboot");
    reboot(0);
    exit(0);
}

int main(int argc, char *argv[])
{
    //signal(SIGSEGV, &dump);

    int status = -1;


    while(status != 0)
    {
        status = system("ifconfig eth0 up");
        if(status < 0)
        {
            printf("cmd: %s\t error: %s", "cmdstring", strerror(errno)); // 这里务必要把errno信息输出或记入Log
            return -1;
        }
        if(WIFEXITED(status))
        {
            printf("normal termination, exit status = %d\n", WEXITSTATUS(status)); //取得cmdstring执行结果
        }
        else if(WIFSIGNALED(status))
        {
            printf("abnormal termination,signal number =%d\n", WTERMSIG(status)); //如果cmdstring被信号中断，取得信号值
        }
        else if(WIFSTOPPED(status))
        {
            printf("process stopped, signal number =%d\n", WSTOPSIG(status)); //如果cmdstring被信号暂停执行，取得信号
        }
    }

    sleep(5);

    qDebug()<<"staring...................\r\n\r\n";
    QApplication a(argc, argv);
    QApplication::setOverrideCursor(Qt::BlankCursor);
    MyWidget w;
    w.show();

    return a.exec();
}
