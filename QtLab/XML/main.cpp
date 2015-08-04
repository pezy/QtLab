#include <iostream>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QDir>

int main()
{
	QDomDocument doc("mydocument");

	std::cout << qPrintable(QDir::currentPath()) << std::endl;
	QFile file("../../QtLab/XML/mydocument.xml");
	if (!file.open(QIODevice::ReadOnly| QIODevice::Text))
		return -1;
	if (!doc.setContent(&file)) {
		file.close();
		return -2;
	}
	file.close();

	QDomElement docElem = doc.documentElement();

	QDomNode n = docElem.firstChild();
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()) {
			std::cout << qPrintable(e.tagName()) << std::endl; // the node really is an element.
		}
		n = n.nextSibling();
	}

	// Here we append a new element to the end of the document
	QDomElement elem = doc.createElement("img");
	elem.setAttribute("src", "myimage.png");
	docElem.appendChild(elem);

	return 0;
}
