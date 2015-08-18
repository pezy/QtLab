#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <QObject>
#include <Qt3DCore/QEntity>
#include <Qt3DRenderer/QTorusMesh>

class SceneModifier : public QObject
{
	Q_OBJECT

public:
	explicit SceneModifier(Qt3D::QEntity *rootEntity);
	~SceneModifier();

public slots:
	void enableTorus(bool enabled);
	void enableCylinder(bool enabled);
	void enableCuboid(bool enabled);
	void enableSphere(bool enabled);

private:
	Qt3D::QEntity *m_rootEntity;
	Qt3D::QEntity *m_cylinderEntity;
	Qt3D::QEntity *m_torusEntity;
	Qt3D::QEntity *m_cuboidEntity;
	Qt3D::QEntity *m_sphereEntity;
	
};

#endif // SCENEMODIFIER_H
