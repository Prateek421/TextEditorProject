#ifndef REDOSTACK_H
#define REDOSTACK_H
#include <QString>
#include <QStack>

class RedoStack
{
public:
    RedoStack();
    void push(const QString& text);
    QString redo();

private:
    QStack<QString> stack;
};

#endif // REDOSTACK_H
