#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

class Widget : public QWidget
{
public:
    Widget():m_pos(-1),m_dragging(false)
    {
		setMouseTracking(true);
    }

	virtual void paintEvent(QPaintEvent *)
	{
		QPainter painter(this);
		painter.setPen(QPen(QBrush(Qt::red), 3));
		if (m_pos == -1) m_pos = rect().left() + 100;
		painter.drawLine(QPoint(m_pos, rect().top()), QPoint(m_pos, rect().bottom()));
	}

    virtual void mouseMoveEvent(QMouseEvent *event)
    {
        if (event->pos().x() >= m_pos-2 && event->pos().x() <= m_pos+2 || m_dragging)
			setCursor(Qt::SizeHorCursor);
		else 
			setCursor(QCursor());

		if (m_dragging)
		{
			m_pos = event->pos().x();
			update();
		}
    }

	virtual void mousePressEvent(QMouseEvent *event)
	{
		if (event->pos().x() >= m_pos-1 && event->pos().x() <= m_pos+1)
			m_dragging = true;
	}

	virtual void mouseReleaseEvent(QMouseEvent *event)
	{
		if (m_dragging)
			m_dragging = false;
	}

private:
	int m_pos;
	bool m_dragging;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Widget widget;
    widget.show();

    return app.exec();
}
