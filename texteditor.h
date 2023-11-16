#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include "linkedlist.h"
#include "spellchecker.h"
#include "undostack.h"
#include "redostack.h"

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include  "lineswap.h"
#include <QRegularExpression>
#include <QTextCursor>
#include <QKeyEvent>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class TextEditor; }
QT_END_NAMESPACE

class TextEditor : public QMainWindow
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent = nullptr);
    ~TextEditor();
    Lineswap *childWindow;
    UndoStack undostack;
    RedoStack redostack;
    void onDataEntered(int m, int n);



private:
    LinkedList stored_data;
    SpellChecker* spellCheckerTrie;
    QString lstword;
    QComboBox *fontFamilyComboBox;
    QComboBox *fontSizeComboBox;
    QCheckBox *boldCheckBox;
    QCheckBox *italicCheckBox;

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionLine_swap_triggered();

  //  void onDataEntered(int m, int n);


    void buildTrieFromDictionary(SpellChecker* , const QString& );
    void on_actionUndo_triggered();

    void onTextModified(const QString& newText);

    void on_actionFormat_triggered();

    void onTextHovered();

    void onupdateSuggestions(const QString& word);

    void on_actionRedo_triggered();

    void onTextRedo(const QString& prevtext);

    void changeFontFamily(int index);
    void changeFontSize(int index);
    void toggleBold(bool isChecked);
    void toggleItalic(bool isChecked);

signals:
    void textModified(const QString& newText);
    void textRedo(const QString& newText);
    void textHovered();
    void updateSuggestions(const QString& word);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

private:
    Ui::TextEditor *ui;
    Lineswap *lineswap;
    QString currfile="";

    QString hoveredWord;
    QStringList suggestionList;

};
#endif // TEXTEDITOR_H
