#ifndef UPORDERFORM_H
#define UPORDERFORM_H

#include <QWidget>

namespace Ui {
class UporderForm;
}

class UporderForm : public QWidget
{
    Q_OBJECT

public:
    explicit UporderForm(QWidget *parent = 0);
    ~UporderForm();

private:
    Ui::UporderForm *ui;
};

#endif // UPORDERFORM_H
