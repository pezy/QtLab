#ifndef ICONEDITORPLUGIN_H
#define ICONEDITORPLUGIN_H

#include <QtDesigner/QDesignerCustomWidgetInterface>

class IconEditorPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	IconEditorPlugin(QObject *parent = 0);
	
	QString name() const;
	QString includeFile() const;
	QString group() const;
	QIcon icon() const;
	QString toolTip() const;
	QString whatsThis() const;
	bool isContainer() const;
	QWidget *createWidget(QWidget *parent);
};

#endif // ICONEDITORPLUGIN_H
