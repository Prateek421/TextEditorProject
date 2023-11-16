#include "undostack.h"

UndoStack::UndoStack() {}

void UndoStack::push(const QString& text) {
    stack.push(text);
}

QString UndoStack::undo() {
    if (!stack.isEmpty()) {
        return stack.pop();
    }
    return QString();
}
