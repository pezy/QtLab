#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class QCheckBox;
class QComboBox;
class QDateTimeEdit;
class QDial;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QScrollBar;
class QSlider;
class QSpinBox;
class QTabWidget;
class QTableWidget;
class QTextEdit;

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    Dialog(QWidget *parent = 0);

private slots:
    void changeStyle(const QString &styleName);

private:
    void createTopLeftGroupBox();
    void createTopRightGroupBox();
    void createBottomLeftTabWidget();
    void createBottomRightGroupBox();

    QLabel *styleLabel;
    QComboBox *styleComboBox;
    QCheckBox *useStylePaletteCheckBox;
    QCheckBox *disableWidgetsCheckBox;

    QGroupBox *topLeftGroupBox;
    QRadioButton *radioButton1;
    QRadioButton *radioButton2;
    QRadioButton *radioButton3;
    QCheckBox *checkBox;

    QGroupBox *topRightGroupBox;
    QPushButton *defaultPushButton;
    QPushButton *togglePushButton;
    QPushButton *flatPushButton;

    QTabWidget *bottomLeftTabWidget;
    QTableWidget *tableWidget;
    QTextEdit *textEdit;

    QGroupBox *bottomRightGroupBox;
    QLineEdit *lineEdit;
    QSpinBox *spinBox;
    QDateTimeEdit *dateTimeEdit;
    QSlider *slider;
    QScrollBar *scrollBar;
    QDial *dial;
};

#endif // DIALOG_H
