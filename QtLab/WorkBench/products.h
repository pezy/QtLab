#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <QString>
#include <QMap>

class Products
{
public:
	static Products& Instance()
	{
		static Products products;
		return products;
	}

	enum EProducts {ENotePad = 1, EWordPad = 2, ECalculator = 4, EPaint = 8};
	struct SProducts
	{
		SProducts(const QString& strName, const QString& strIcon, const QString& strIconDisabled) : m_name(strName), m_icon(strIcon), m_iconDisabled(strIconDisabled) {}
		QString m_name;
		QString m_icon;
		QString m_iconDisabled;
	};

	QList<SProducts> GetAllProducts() { return m_mapAllProducts.values(); }
	void SetCurrentProducts(const QStringList& productsName);
	void SetCurrentProducts(EProducts product);
	bool ContainsInProducts(int productIndex);

	Products(const Products&) = delete;
	Products& operator=(const Products&) = delete;

private:
	Products();

private:
	QMap<int, SProducts> m_mapAllProducts;
	int m_currentProduct = 0;
};

#endif // PRODUCTS_H
