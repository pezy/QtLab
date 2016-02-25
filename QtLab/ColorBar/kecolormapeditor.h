#ifndef KECOLORMAPEDITOR_H
#define KECOLORMAPEDITOR_H

/*!
*  @class  CKEColormapEditor in kecolormapeditor.h
*  @brief  widget for editing color map.
*  @author pezy
*  @date   2016-02-05
*/

#include <QWidget>
#include <QMenu>

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
    void slotAddControlPoint();
    void slotAddGeologicMask();
    void slotDeleteControlPointOrMask();
	void slotAllOpaque();
	void slotDeleteOpacityPoint();
	void slotAddOpacityPoint();
	void slotSetOpacityFreeDrawing(bool bChecked);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QPointF _ColorIndexToControlPos(int index);
	QPointF _IndexAlphaToOpacityPos(int index, int alpha);
    int _PosToColorIndex(const QPointF& pos);
	int _PosToAlpha(const QPointF& pos);

    void _DrawControlPoint(int index);
    void _DrawGeologicMask();
	void _DrawOpacityPoint();
    void _OpenControlMenu(QMenu& menu, const QPoint& pos);
	void _OpenOpacityMenu(QMenu& menu, const QPoint& pos);
    bool _IsInControlPoint(const QPoint& pos, int& controlIndex);
    bool _IsInGeologicMask(const QPoint& pos, int& maskIndex);
	bool _IsInOpacityPoint(const QPoint& pos, int& controlIndex, int& alpha);

    void _ShowMenu(const QPoint& position, const QPoint& cursorPos);
    void _SetControlPointColor(const QPoint& position);
    void _ShowColorInfo(const QPoint& position);
    void _SelectControlPointOrMask(const QPoint& pos);
	void _BeginFreeDrawing(const QPoint& pos);

private:
    const qreal m_controlRectHeight = 15.0;

    CKEColormap* m_pColormap = nullptr;
    CKEColormap* m_pTemplateColormap = nullptr;

    QRectF m_colorBarRect;
    QRectF m_controlRect;

    qreal m_singleColorBarWidth = 0.0;

    int m_selectedColorIndex = -1;

    // change control point position
    bool m_bControlIndexChanged = false;
    QRgb m_selectedControlPointRgb = 0u;
    QList<int> m_listFixedControlPoints;

    // geologic mask
    QMap<int, QRgb> m_mapGeologicMask;
    bool m_bGeologicMaskChanged = false;
    QRgb m_selectedMaskRgb = 0u;
    QList<int> m_listFixedMask;

	// opacity
	int m_selectedAlpha = 255;
	bool m_bFreeDrawing = false;
	bool m_bDrawingStart = false;
};

#endif // KECOLORMAPEDITOR_H
