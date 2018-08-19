#include "dialog1.h"
#include "ui_dialog1.h"

Dialog1::Dialog1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog1)
{
    ui->setupUi(this);
}

Dialog1::~Dialog1()
{
    delete ui;
}

void Dialog1::setValue(int val)
{
    ui->progressBar->setValue( val );
}

void Dialog1::setFirst(const QString val)
{
    ui->label1->setText(val);
}

void Dialog1::setSecond(const QString val)
{
    ui->label2->setText(val);
}
