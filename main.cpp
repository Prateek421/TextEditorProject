#include "texteditor.h"
#include "lineswap.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextEditor w;
    Lineswap l;
   // Lineswap childWindow=new Lineswap();
   // QObject::connect(&childWindow, SIGNAL(dataEntered(int,int)), &w,SLOT(on_DataEntered(int,int)));
   // QObject::connect(&childWindow, SIGNAL(dataEntered(int,int)), &w,SLOT(show()));
    QObject::connect(&l, &Lineswap::dataEntered, &w, &TextEditor::onDataEntered);
    w.show();
    return a.exec();
}
