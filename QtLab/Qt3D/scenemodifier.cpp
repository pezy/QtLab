#include "scenemodifier.h"

#include <Qt3DCore/qscaletransform.h>
#include <Qt3DCore/qrotatetransform.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/QTranslateTransform>

#include <Qt3DRenderer/QCylinderMesh>
#include <Qt3DRenderer/QCuboidMesh>
#include <Qt3DRenderer/QSphereMesh>
#include <Qt3DRenderer/QPhongMaterial>

SceneModifier::SceneModifier(Qt3D::QEntity* rootEntity)
	: m_rootEntity(rootEntity)
{
	// Torus shape data
	Qt3D::QTorusMesh *torus = new Qt3D::QTorusMesh();
	torus->setRadius(1.0f);
	torus->setMinorRadius(0.4f);
	torus->setRings(100);
	torus->setSlices(20);

	// TorusMesh Transform
	Qt3D::QScaleTransform *torusScale = new Qt3D::QScaleTransform();
	Qt3D::QTranslateTransform *torusTranslation = new Qt3D::QTranslateTransform();
	Qt3D::QRotateTransform *torusRotation = new Qt3D::QRotateTransform();
	Qt3D::QTransform *torusTransforms = new Qt3D::QTransform();

	torusScale->setScale3D(QVector3D(2.0f, 2.0f, 2.0f));
	torusTranslation->setTranslation(QVector3D(1.7f, 1.7f, 0.0f));
	torusRotation->setAngleDeg(25.0f);
	torusRotation->setAxis(QVector3D(0, 1, 0));

	torusTransforms->addTransform(torusRotation);
	torusTransforms->addTransform(torusTranslation);
	torusTransforms->addTransform(torusScale);

	Qt3D::QPhongMaterial *torusMaterial = new Qt3D::QPhongMaterial();
	torusMaterial->setDiffuse(QColor(QRgb(0xbeb32b)));

	// Torus
	m_torusEntity = new Qt3D::QEntity(m_rootEntity);
	m_torusEntity->addComponent(torus);
	m_torusEntity->addComponent(torusMaterial);
	m_torusEntity->addComponent(torusTransforms);

	// Cylinder shape data
	Qt3D::QCylinderMesh *cylinder = new Qt3D::QCylinderMesh();
	cylinder->setRadius(1);
	cylinder->setLength(3);
	cylinder->setRings(100);
	cylinder->setSlices(20);

	// CylinderMesh Transform
	Qt3D::QScaleTransform *cylinderScale = new Qt3D::QScaleTransform();
	Qt3D::QRotateTransform *cylinderRotation = new Qt3D::QRotateTransform();
	Qt3D::QTranslateTransform *cylinderTranslation = new Qt3D::QTranslateTransform();
	Qt3D::QTransform *cylinderTransforms = new Qt3D::QTransform();

	cylinderScale->setScale3D(QVector3D(1.5f, 1.5f, 1.5f));
	cylinderTranslation->setTranslation(QVector3D(-2.6f, 2.4f, -1.5));
	cylinderRotation->setAngleDeg(45.0f);
	cylinderRotation->setAxis(QVector3D(1, 0, 0));

	cylinderTransforms->addTransform(cylinderTranslation);
	cylinderTransforms->addTransform(cylinderRotation);
	cylinderTransforms->addTransform(cylinderScale);

	Qt3D::QPhongMaterial *cylinderMaterial = new Qt3D::QPhongMaterial();
	cylinderMaterial->setDiffuse(QColor(QRgb(0x928327)));

	// Cylinder
	m_cylinderEntity = new Qt3D::QEntity(m_rootEntity);
	m_cylinderEntity->addComponent(cylinder);
	m_cylinderEntity->addComponent(cylinderMaterial);
	m_cylinderEntity->addComponent(cylinderTransforms);

	// Cuboid shape data
	Qt3D::QCuboidMesh *cuboid = new Qt3D::QCuboidMesh();

	// CuboidMesh Transform
	Qt3D::QScaleTransform *cuboidScale = new Qt3D::QScaleTransform();
	Qt3D::QTranslateTransform *cuboidTranslation = new Qt3D::QTranslateTransform();
	Qt3D::QTransform *cuboidTransforms = new Qt3D::QTransform();

	cuboidScale->setScale3D(QVector3D(4.0f, 4.0f, 4.0f));
	cuboidTranslation->setTranslation(QVector3D(0.9f, -0.9f, 0.0f));

	cuboidTransforms->addTransform(cuboidTranslation);
	cuboidTransforms->addTransform(cuboidScale);

	Qt3D::QPhongMaterial *cuboidMaterial = new Qt3D::QPhongMaterial();
	cuboidMaterial->setDiffuse(QColor(QRgb(0x665423)));

	//Cuboid
	m_cuboidEntity = new Qt3D::QEntity(m_rootEntity);
	m_cuboidEntity->addComponent(cuboid);
	m_cuboidEntity->addComponent(cuboidMaterial);
	m_cuboidEntity->addComponent(cuboidTransforms);

	// Sphere shape data
	Qt3D::QSphereMesh *sphereMesh = new Qt3D::QSphereMesh();
	sphereMesh->setRings(20);
	sphereMesh->setSlices(20);
	sphereMesh->setRadius(2);

	// Sphere mesh transform
	Qt3D::QScaleTransform *sphereScale = new Qt3D::QScaleTransform();
	Qt3D::QTranslateTransform *sphereTranslation = new Qt3D::QTranslateTransform();
	Qt3D::QTransform *sphereTransforms = new Qt3D::QTransform();

	sphereScale->setScale3D(QVector3D(1.3f, 1.3f, 1.3f));
	sphereTranslation->setTranslation(QVector3D(-2.9f, -2.9f, 0.0f));

	sphereTransforms->addTransform(sphereTranslation);
	sphereTransforms->addTransform(sphereScale);

	Qt3D::QPhongMaterial *sphereMaterial = new Qt3D::QPhongMaterial();
	sphereMaterial->setDiffuse(QColor(QRgb(0xa69929)));

	// Sphere
	m_sphereEntity = new Qt3D::QEntity(m_rootEntity);
	m_sphereEntity->addComponent(sphereMesh);
	m_sphereEntity->addComponent(sphereMaterial);
	m_sphereEntity->addComponent(sphereTransforms);
}

SceneModifier::~SceneModifier()
{

}

void SceneModifier::enableTorus(bool enabled)
{
	m_torusEntity->setParent(enabled ? m_rootEntity : Q_NULLPTR);
}

void SceneModifier::enableCylinder(bool enabled)
{
	m_cylinderEntity->setParent(enabled ? m_rootEntity : Q_NULLPTR);
}

void SceneModifier::enableCuboid(bool enabled)
{
	m_cuboidEntity->setParent(enabled ? m_rootEntity : Q_NULLPTR);
}

void SceneModifier::enableSphere(bool enabled)
{
	m_sphereEntity->setParent(enabled ? m_rootEntity : Q_NULLPTR);
}