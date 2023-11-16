#include "lineswap.h"
#include "ui_lineswap.h"
#include <QMessageBox>
#include <QTextStream>
#include "texteditor.h"

Lineswap::Lineswap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lineswap)
{
    ui->setupUi(this);
}

Lineswap::~Lineswap()
{
    delete ui;
}

void Lineswap::on_pushButton_clicked()
{
    QString line_m = ui->line1->text();
    QString line_n = ui->lineEdit_2->text();
    bool conversionOk1;
    bool conversionOk2;
    int int_m = line_m.toInt(&conversionOk1);
    int int_n = line_n.toInt(&conversionOk2);


    if (conversionOk1 && conversionOk2) {
        qDebug()<<"Signal emitted";
        emit dataEntered(int_m,int_n);

        close();
    } else {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer.");
    }
}

