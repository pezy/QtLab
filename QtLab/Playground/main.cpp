#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

class Widget : public QWidget
{
public:
    Widget()
    {
		setMouseTracking(true);
    }

	virtual void paintEvent(QPaintEvent *)
	{
		QPainter painter(this);
		painter.setPen(QPen(QBrush(Qt::red), 3));
		painter.drawLine(QPoint(rect().left()+100, rect().top()), QPoint(rect().left()+100, rect().bottom()));
	}

    virtual void mouseMoveEvent(QMouseEvent *event)
    {
        if (event->pos().x() >= rect().left()+99 && event->pos().x() <= rect().left()+101)
			setCursor(Qt::SizeHorCursor);
		else
			setCursor(QCursor());
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Widget widget;
    widget.show();

    return app.exec();
}
