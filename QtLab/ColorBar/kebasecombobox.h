/**
* @class CGmBaseComboBox gmbasecombobox.h
* @brief set pop up list view min width.
* 
* @author daily 
* @date 3/10/2009
* @see 
*/
#ifndef KEBASECOMBOBOX_H
#define KEBASECOMBOBOX_H

#include <QComboBox>
#include <QAbstractItemView>

class CKEBaseComboBox : public QComboBox
{
public:
	CKEBaseComboBox(QWidget * parent = 0):QComboBox(parent), m_bReadOnly(false)
	{
	}
	virtual ~CKEBaseComboBox(void)
	{
	}

    
    /**
    * @brief To sort the items in a combobox.
    * @return  void
    * @date 7/28/2009
    * @see 
    * @auther fanhua
    */
    void sort()
    {
        int iCount=count();
        bool bSorted=true;
        QString s1,s2;
        while(bSorted)
        {
            bSorted=false;
            int i=0;
            for(i=0;i<iCount-1;i++)
            {
                s1=itemText(i);
                s2=itemText(i+1);
                if(s2<s1)
                {
                    setItemText(i,s2);
                    setItemText(i+1,s1);
                    bSorted=true;
                    break;
                }
            }
        }
    }

    /**
    * @brief set pop up list view min width.
    * @return 
    * @date 3/10/2009
    * @see QComboBox::showPopup()
    */
	virtual void showPopup()
	{
		if(m_bReadOnly)
		{
			return ;
		}

		QComboBox::showPopup();
		QFont font = this->font();
		QFontMetrics fMertics(font);
		QString strCurText;
		int nTextWidth = width();
		for(int i = 0 ; i < this->count(); i++)
		{
			strCurText = this->itemText(i);
			int nCurTextWidth = fMertics.width(strCurText) * 1.2;
			if(nTextWidth < nCurTextWidth)
				nTextWidth = nCurTextWidth;

		}
		QAbstractItemView * pView = view();
		QSize size = this->iconSize();
		pView->setMinimumWidth(nTextWidth+size.width());
	}

	// set current text, instead of function setCurrentText support by Qt3.
	void SetCurrentText(const QString& text)
	{
        int i = findText(text);
        if (i != -1)
		{
			setCurrentIndex(i);
		}
        else if (isEditable())
		{
			setEditText(text);
		}
        else
		{
			setItemText(currentIndex(), text);
		}
    }

	void SetReadOnly(bool bReadOnly)
	{
		m_bReadOnly = bReadOnly ;
	}

private:
	bool m_bReadOnly ;
};

#endif // KEBASECOMBOBOX_H