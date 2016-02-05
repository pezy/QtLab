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

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    CKEColormap* m_pColormap = nullptr;
    CKEColormap* m_pTemplateColormap = nullptr;
};

#endif // KECOLORMAPEDITOR_H
