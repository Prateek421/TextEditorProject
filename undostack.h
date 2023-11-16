#ifndef UNDOSTACK_H
#define UNDOSTACK_H


#include <QStack>
#include <QString>

class UndoStack
{
public:
    UndoStack();
    //int start;  //Store cursor start position for undo operation
    //int end;

    void push(const QString& text);
    QString undo();

private:
    QStack<QString> stack;
};

#endif // UNDOSTACK_H
