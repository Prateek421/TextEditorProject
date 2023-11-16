#include "redostack.h"

RedoStack::RedoStack()
{

}

void RedoStack::push(const QString& text) {
    stack.push(text);
}

QString RedoStack::redo() {
    if (!stack.isEmpty()) {
        return stack.pop();
    }
    return QString();
}
