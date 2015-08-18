#ifndef WINDOW_H
#define WINDOW_H

#include <QWindow>

class Window : public QWindow
{
	Q_OBJECT

public:
	explicit Window(QScreen *screen = 0);
	~Window();

protected:
	//virtual void keyPressEvent(QKeyEvent *e) override;
};

#endif // WINDOW_H
