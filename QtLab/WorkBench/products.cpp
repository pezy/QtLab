#include "products.h"

void Products::SetCurrentProducts(const QStringList & productsName)
{
	m_currentProduct = 0;

	for (auto iter = m_mapAllProducts.cbegin(); iter != m_mapAllProducts.cend(); ++iter)
	{
		if (productsName.contains(iter.value().m_name))
			m_currentProduct |= iter.key();
	}
}

void Products::SetCurrentProducts(EProducts product)
{
	m_currentProduct = product;
}

bool Products::ContainsInProducts(int productIndex)
{
	if (m_currentProduct == 0)
		return true;
	else
		return m_currentProduct & productIndex;
}

Products::Products()
{
	m_mapAllProducts.insert(ENotePad, SProducts("notepad", "notepad", "notepad_gray"));
	m_mapAllProducts.insert(EWordPad, SProducts("wordpad", "wordpad", "wordpad_gray"));
	m_mapAllProducts.insert(ECalculator, SProducts("calc", "calc", "calc_gray"));
	m_mapAllProducts.insert(EPaint, SProducts("mspaint", "paint", "paint_gray"));
}
