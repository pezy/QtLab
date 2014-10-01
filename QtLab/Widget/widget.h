#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

/*!
 * \class Widget
 *
 * \brief Make clear the different of some public function of QWidget.
 *
 * \author PEZY
 * \date Sept. 2014
 */

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
	virtual void paintEvent(QPaintEvent *event);
};

#endif // WIDGET_H
