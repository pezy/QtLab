#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QDebug>

class Widget : public QWidget
{
public:
    Widget()
    {
        startTimer(500);
    }

    void timerEvent(QTimerEvent *)
    {
        // Here values are different
        qDebug() << "geometry:(" << geometry().left() << "," << geometry().right() << ")";
		qDebug() << "rect:(" << rect().left() << "," << rect().right() << ")";
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Widget widget;
    widget.show();

    return app.exec();
}
