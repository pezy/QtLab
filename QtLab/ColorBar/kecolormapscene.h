#ifndef KECOLORMAPSCENE_H
#define KECOLORMAPSCENE_H

#include <QGraphicsScene>

/*!
*  @class  CKEColormapScene in "kecolormapscene.h"
*  @brief  rainbow scene.
*  @author pezy
*  @date   2016-01-22
*/

class CKEColormapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CKEColormapScene(QObject *parent);
    ~CKEColormapScene();

private:
    
};

#endif // KECOLORMAPSCENE_H
