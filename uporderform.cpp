#include "uporderform.h"
#include "ui_uporderform.h"

UporderForm::UporderForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UporderForm)
{
    ui->setupUi(this);
}

UporderForm::~UporderForm()
{
    delete ui;
}
