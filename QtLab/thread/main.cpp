#include <QThread>
#include <QTextStream>
#include <QMutex>

QTextStream qout(stdout);

class Thread : public QThread
{
protected:
    void run()
    {
        qout << "hello world" << endl;
        sleep(5);
        qout << "hi again" << endl;
    }
};

int main()
{
    Thread thread;
    thread.start();
    thread.wait();

    return 0;
}
