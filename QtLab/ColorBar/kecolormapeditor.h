#ifndef KECOLORMAPEDITOR_H
#define KECOLORMAPEDITOR_H

/*!
*  @class  CKEColormapEditor in kecolormapeditor.h
*  @brief  widget for editing color map.
*  @author pezy
*  @date   2016-02-05
*/

#include <QWidget>

class CKEColormap;

class CKEColormapEditor : public QWidget
{
    Q_OBJECT

public:
    CKEColormapEditor(QWidget *parent = 0);
    ~CKEColormapEditor();

signals:
    void ShowMsg(const QString& msg);

public slots:
    void slotReset();
    void slotFlip();
    void slotTemplateChanged(const QString& templateName);
    void slotSaveAll();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QPointF _ColorIndexToControlPos(int index);
    int _PosToColorIndex(const QPointF& pos);

    void _DrawControlPoint(int index);

private:
    const qreal m_controlRectHeight = 15.0;

    CKEColormap* m_pColormap = nullptr;
    CKEColormap* m_pTemplateColormap = nullptr;

    QRectF m_colorBarRect;
    QRectF m_controlRect;

    qreal m_singleColorBarWidth = 0.0;
};

#endif // KECOLORMAPEDITOR_H
