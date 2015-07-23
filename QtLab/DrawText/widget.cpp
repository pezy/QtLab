#include "widget.h"

#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}

Widget::~Widget()
{

}

void Widget::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);

    QString left_str("Time(ms)");
    QString top_str("Inline: 526");

    QFont title_font("Arial", 12, QFont::Bold);
    setFont(title_font);

    QFontMetrics font_metrics(title_font);
    int left_title_height = font_metrics.height();
    int left_title_width = font_metrics.width(left_str);

    QPainter painter(this);

    painter.translate(left_title_height/2, height()/2);
    painter.rotate(-90);

    QRectF left_title_rect(-left_title_width/2, -left_title_height/2, left_title_width, left_title_height);
    painter.drawText( left_title_rect, Qt::AlignLeft, left_str);

    painter.rotate(90);
    painter.translate(-left_title_height/2, -height()/2);

    painter.drawText( rect(), Qt::AlignHCenter, top_str);
}
