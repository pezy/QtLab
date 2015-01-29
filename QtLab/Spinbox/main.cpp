#include <QApplication>
#include <QDoubleSpinBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDoubleSpinBox doubleSpinBox;
    doubleSpinBox.setRange(0.f, 1000.f);
    doubleSpinBox.setValue(1.f);
    doubleSpinBox.setSpecialValueText("unset");

    doubleSpinBox.show();
    return a.exec();
}
