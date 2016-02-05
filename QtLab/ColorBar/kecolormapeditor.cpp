#include "kecolormapeditor.h"

#include <QPainter>

#include "kecolormap.h"

CKEColormapEditor::CKEColormapEditor(QWidget *parent)
    : QWidget(parent)
{
    
}

CKEColormapEditor::~CKEColormapEditor()
{

}

void CKEColormapEditor::slotReset()
{
    for (int i = 0; i != 256; ++i)
    {
        m_pColormap->SetColorAt(i, m_pTemplateColormap->GetColorAt(i));
    }

    update();
}

void CKEColormapEditor::slotFlip()
{
    int first = 0, last = 256;
    while (first != last && first != --last)
    {
        QColor temp = m_pColormap->GetColorAt(first);
        m_pColormap->SetColorAt(first, m_pColormap->GetColorAt(last));
        m_pColormap->SetColorAt(last, temp);
        ++first;
    }

    update();
}

void CKEColormapEditor::slotTemplateChanged(const QString& templateName)
{
    if (!m_pColormap)
    {
        m_pColormap = new CKEColormap(*CKEColormap::GetColormap(templateName));
    }
    else
    {
        m_pTemplateColormap = CKEColormap::GetColormap(templateName);
        for (int i = 0; i != 256; ++i)
        {
            m_pColormap->SetColorAt(i, m_pTemplateColormap->GetColorAt(i));
        }
    }

    update();
}

void CKEColormapEditor::paintEvent(QPaintEvent* event)
{
    QPixmap tile = QPixmap(16, 16);
    tile.fill(Qt::white);
    QPainter pt(&tile);
    QColor color(230, 230, 230);
    pt.fillRect(0, 0, 8, 8, color);
    pt.fillRect(8, 8, 8, 8, color);
    pt.end();

    QPainter painter(this);
    painter.save();
    painter.drawTiledPixmap(rect(), tile);
    painter.restore();
}