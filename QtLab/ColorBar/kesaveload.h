#ifndef KESAVELOAD_H
#define KESAVELOAD_H

#include <qdatastream.h>
#include <qvariant.h>
#include <qbytearray.h>

template<typename T>
QVariant SaveDataToBLOB(T* pData, int n)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);

	for (int i=0; i<n; ++i)
		dataStream<<pData[i];

	return QVariant(byteArray);
};


template<typename T>
void LoadDataFromBLOB(const QVariant& vtBlob, T* pData, int n)
{
	QByteArray BlobArray = vtBlob.toByteArray();
	QDataStream dataStream(&BlobArray, QIODevice::ReadOnly);

	for (int i=0; i<n; ++i)
	{
		dataStream>>pData[i];
	}
};

#endif // RLSAVELOAD_H
