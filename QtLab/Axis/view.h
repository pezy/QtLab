#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

class QGraphicsScene;

class View : public QGraphicsView
{
	Q_OBJECT

public:
	View(QWidget *parent);
	~View();

protected:
	virtual void scrollContentsBy(int dx, int dy) override;

private:
	
};

#endif // VIEW_H
