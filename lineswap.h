#ifndef LINESWAP_H
#define LINESWAP_H

#include <QDialog>

namespace Ui {
class Lineswap;
}

class Lineswap : public QDialog
{
    Q_OBJECT

public:
    explicit Lineswap(QWidget *parent = nullptr);
    ~Lineswap();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Lineswap *ui;

signals:
    void dataEntered(int m, int n);
};

#endif // LINESWAP_H
