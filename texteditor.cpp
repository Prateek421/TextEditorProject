#include "texteditor.h"
#include "./ui_texteditor.h"
#include <iostream>
#include "undostack.h"
#include <QClipboard>
#include <QComboBox>
#include <QCheckBox>
#include <QDockWidget>
#include "suggestionshover.h"
#include <QLabel>
#include <QPushButton>>

TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextEditor),
spellCheckerTrie(nullptr)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->plainTextEdit);
    childWindow=new Lineswap;

    lstword="";

    //connect(&*childWindow, SIGNAL(dataEntered(int,int)), this, SLOT(on_DataEntered(int,int)));



    spellCheckerTrie = new SpellChecker();
    this->buildTrieFromDictionary(spellCheckerTrie, "C:\\Users\\Prateek gupta\\dictionary.txt");

    //connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &TextEditor::on_textChanged);
    ui->plainTextEdit->setFocusPolicy(Qt::StrongFocus);
    connect(this, &TextEditor::textModified, this, &TextEditor::onTextModified);
    connect(this, &TextEditor::textRedo, this, &TextEditor::onTextRedo);
    //this->grabKeyboard();
    connect(this, &TextEditor::updateSuggestions, this, &TextEditor::onupdateSuggestions);

    // Connect the onTextHovered signal to the onTextHovered slot
    connect(this, &TextEditor::textHovered, this, &TextEditor::onTextHovered);


    //connect(&*childWindow, &Lineswap::dataEntered, this, &TextEditor::onDataEntered);

    fontFamilyComboBox = new QComboBox();
    fontSizeComboBox = new QComboBox();
    boldCheckBox = new QCheckBox("Bold");
    italicCheckBox = new QCheckBox("Italic");

    // Connect the UI elements to slots here.
    connect(fontFamilyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFontFamily(int)));
    connect(fontSizeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFontSize(int)));
    connect(boldCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleBold(bool)));
    connect(italicCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleItalic(bool)));



}

TextEditor::~TextEditor()
{
    delete ui;
}

void TextEditor::onTextModified(const QString& newText) {
    undostack.push(newText);
}

void TextEditor::onTextRedo(const QString& prevtext)
{
    redostack.push(prevtext);
}

void TextEditor::on_actionNew_triggered()
{
    std::cout<<"This func is ok";
    currfile.clear();
    ui->plainTextEdit->setPlainText(QString());
}



void TextEditor::on_actionOpen_triggered()
{
    QString file_name=QFileDialog::getOpenFileName(this,"Open File");
    if(!file_name.isEmpty())
    {
        currfile=file_name;
        QFile file(file_name);
        if(!file.open(QIODevice::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this,"Warning!!!","This file cannot be Opened\n\nError: "+file.errorString());
            return;
        }
        setWindowTitle(file_name);
        QTextStream in(&file);
        QString filetext=in.readAll();
        ui->plainTextEdit->setPlainText(filetext);
        file.close();
    }
    else
    {
        QMessageBox::warning(this,"Warning!!!","Please Enter a file name");

    }
}


void TextEditor::on_actionSave_triggered()
{
    QString file_name=QFileDialog::getSaveFileName(this,"Save as");
    if(!file_name.isEmpty())
    {
        QFile file(file_name);
        if(!file.open(QIODevice::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(this,"Warning!!!","file cannot be Saved\n\nError: "+file.errorString());
            return;
        }
        currfile=file_name;
        setWindowTitle(file_name);
        QTextStream out(&file);
        QString filetext=ui->plainTextEdit->toPlainText();
        out<<filetext;
        file.close();
    }

}



void TextEditor::on_actionCut_triggered()
{
    ui->plainTextEdit->cut();
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString cutText = clipboard->text();
    emit textModified(cutText);
}


void TextEditor::on_actionCopy_triggered()
{
    ui->plainTextEdit->copy();
}


void TextEditor::on_actionPaste_triggered()
{
    QString prevtext=ui->plainTextEdit->toPlainText();
    emit textModified(prevtext);
    ui->plainTextEdit->paste();
    QString newtext=ui->plainTextEdit->toPlainText();
    emit textRedo(newtext);

}


void TextEditor::on_actionLine_swap_triggered()
{
    lineswap=new Lineswap(this);
    lineswap->show();
}

void TextEditor::onDataEntered(int a,int b)
{
    qDebug()<<"Connected to function";

    QString curr_text=ui->plainTextEdit->toPlainText();
    QStringList words=curr_text.split('.');

    for(const QString &line : words)
    {
        stored_data.insert(line.toStdString());
    }
    stored_data.swap(a,b);
    if (stored_data.swap(a, b) == 1)
    {
        const QString updated = QString::fromStdString(stored_data.getData());
        ui->plainTextEdit->setPlainText(updated);
    }
    else
    {
        QMessageBox::warning(this, "Warning!!!", "Line number does not exist");
    }
}


void TextEditor::buildTrieFromDictionary(SpellChecker* trie, const QString& dictionaryFile)
{
    QFile file(dictionaryFile);
    qDebug() << "File path: " << dictionaryFile;
    if (file.open(QIODevice::ReadOnly | QFile::Text)) {
        qDebug() << "File opened successfully.";
        QTextStream in(&file);
        qDebug() << "Inside build";
        while (!in.atEnd()) {
            QString word = in.readLine().trimmed();
            if (!word.isEmpty()) {
                if (trie) {
                    trie->insert(word.toStdString());
                } else {
                    qDebug() << "Trie pointer is nullptr!";
                }
            }
        }
        file.close();
    } else {
        qDebug() << "File could not be opened. Error: " << file.errorString();
    }
}


void TextEditor::keyPressEvent(QKeyEvent *event)
{
   // grabKeyboard();
   // qDebug() << "Space key pressed" <<" "<<event->key();

    if (event->key() == Qt::Key_Shift)
    {
       // releaseKeyboard();
        qDebug() << "Space key pressed";

        QString currentText = ui->plainTextEdit->toPlainText();

        QStringList words = currentText.split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);

        QString lastWord = words.isEmpty() ? "" : words.last();

        bool state=spellCheckerTrie->search(lastWord.toStdString());
        qDebug()<<state;

        if (!lastWord.isEmpty() && !spellCheckerTrie->search(lastWord.toStdString()))
        {
            QTextCharFormat errorFormat;
            errorFormat.setForeground(Qt::red);

            int lastWordPosition = currentText.lastIndexOf(lastWord);

            QTextCursor cursor(ui->plainTextEdit->document());
            cursor.setPosition(lastWordPosition, QTextCursor::MoveAnchor);
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, lastWord.length());
            cursor.setCharFormat(errorFormat);
            qDebug() << lastWord;
            lstword = lastWord;

            cursor.movePosition(QTextCursor::End);

            QTextCharFormat defaultFormat;
            defaultFormat.setForeground(Qt::black);

            ui->plainTextEdit->setCurrentCharFormat(defaultFormat);

        }


    }
    QMainWindow::keyPressEvent(event);

}


void TextEditor::on_actionUndo_triggered()
{
    qDebug() << "Undo button clicked.";
    QString previousText = undostack.undo();
    qDebug() << "Previous Text: " << previousText;
    if (!previousText.isEmpty()) {
        ui->plainTextEdit->setPlainText(previousText);
    }
}



//void TextEditor::on_textChanged()
//{
    /*QTextCursor cursor(ui->plainTextEdit->document());
    if (ui->plainTextEdit->textCursor().atBlockEnd() == false){
        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    }*/

    //  cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    // cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

    /*QTextCharFormat defaultFormat;
    defaultFormat.setForeground(Qt::black);
    cursor.setCharFormat(defaultFormat);*/
    /*QTextCursor cursor(ui->plainTextEdit->document());
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

    QTextCharFormat defaultFormat;
    defaultFormat.setForeground(Qt::black);
    cursor.setCharFormat(defaultFormat);
    QString currentText = ui->plainTextEdit->toPlainText();
    QStringList words = currentText.split(QRegularExpression ("\\W+"), Qt::SkipEmptyParts);
    QString lastWord = words.isEmpty() ? "" : words.last().toLower(); // Convert to lowercase for case-insensitive checking

    if (!lastWord.isEmpty() && !spellCheckerTrie->search(lastWord.toStdString()) && lastWord!=lstword) {
        /*QTextCharFormat errorFormat;
        errorFormat.setForeground(Qt::red);
        cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::MoveAnchor, lastWord.length());
        cursor.setCharFormat(errorFormat);*/
    /* QTextCharFormat format;
        format.setForeground(Qt::red);

        // Find the position of the last word in the text
        int lastWordPosition = currentText.lastIndexOf(lastWord);

        // Select the last word's text range using the cursor
        cursor.setPosition(lastWordPosition, QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, lastWord.length());
        lstword=lastWord;
        // Apply the format to the selected text range
        cursor.setCharFormat(format);
    }*/
    /* QTextCursor cursor(ui->plainTextEdit->document());
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);

    QTextCharFormat defaultFormat;
    defaultFormat.setForeground(Qt::black);
    cursor.setCharFormat(defaultFormat);*/

    /*QString currentText = ui->plainTextEdit->toPlainText();
    QStringList words = currentText.split(" ", Qt::SkipEmptyParts);
    QString lastWord = words.isEmpty() ? "" : words.last().toLower();

    if (!lastWord.isEmpty() && lastWord != lstword) {

        if(!spellCheckerTrie->search(lastWord.toStdString())){

            qDebug() << "Space key pressed";
            QTextCharFormat format;
            format.setForeground(Qt::red);
            //ui->plainTextEdit->setPlainText(QString());
            int lastWordPosition = currentText.lastIndexOf(lastWord);//+1;
            lstword= lastWord;
            QTextCursor cursor(ui->plainTextEdit->document());
            cursor.setPosition(lastWordPosition, QTextCursor::MoveAnchor);
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, lastWord.length());
            cursor.setCharFormat(format);
        }/*

        /* else
        {
        int lastWordPosition = currentText.lastIndexOf(lastWord);//+1;
        QTextCharFormat defaultFormat;
        defaultFormat.setForeground(Qt::black);
        QTextCursor cursor(ui->plainTextEdit->document());
        lstword= lastWord;
        cursor.setPosition(lastWordPosition, QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, lastWord.length());
        cursor.setCharFormat(defaultFormat);
        }*/

  //  }
    /*else
    {
        int lastWordPosition = currentText.lastIndexOf(lastWord);//+1;
        QTextCharFormat defaultFormat;
        defaultFormat.setForeground(Qt::black);

        cursor.setCharFormat(defaultFormat);
    }*/
//}

void TextEditor::on_actionFormat_triggered()
{
    QDialog formatDialog(this);
    formatDialog.setWindowTitle("Format Options");

    QVBoxLayout dialogLayout(&formatDialog);

    QLabel fontFamilyLabel("Font Family:");
    dialogLayout.addWidget(&fontFamilyLabel);

    // Set font family options in the comboBox
    fontFamilyComboBox->clear();
    fontFamilyComboBox->addItem("Arial");
    fontFamilyComboBox->addItem("Times New Roman");
    fontFamilyComboBox->addItem("Courier New");

    dialogLayout.addWidget(fontFamilyComboBox);

    QLabel fontSizeLabel("Font Size:");
    dialogLayout.addWidget(&fontSizeLabel);

    // Set font size options in the comboBox
    fontSizeComboBox->clear();
    fontSizeComboBox->addItem("10");
    fontSizeComboBox->addItem("12");
    fontSizeComboBox->addItem("14");
    fontSizeComboBox->addItem("16");

    dialogLayout.addWidget(fontSizeComboBox);

    QCheckBox boldCheckBox("Bold", &formatDialog);
    dialogLayout.addWidget(&boldCheckBox);

    QCheckBox italicCheckBox("Italic", &formatDialog);
    dialogLayout.addWidget(&italicCheckBox);

    // Add an "Apply" button to apply the selected format
    QPushButton applyButton("Apply", &formatDialog);
    QObject::connect(&applyButton, &QPushButton::clicked, [&]() {
        // Apply the selected font family, font size, bold, and italic settings
        QString fontFamily = fontFamilyComboBox->currentText();
        int fontSize = fontSizeComboBox->currentText().toInt();
        bool isBold = boldCheckBox.isChecked();
        bool isItalic = italicCheckBox.isChecked();

        // Apply these formatting options to your text editor.
        // You'll need to implement this part based on your text editor's design.

        // Close the format dialog
        formatDialog.accept();
    });
    dialogLayout.addWidget(&applyButton);

    formatDialog.setLayout(&dialogLayout);

    // Show the format dialog
    formatDialog.exec();




    /*QTabWidget *tabWidget = new QTabWidget(this);

    QWidget *fontTab = new QWidget(this);
    QVBoxLayout *fontLayout = new QVBoxLayout(fontTab);

    QComboBox *fontFamilyComboBox = new QComboBox(fontTab);
    QComboBox *fontSizeComboBox = new QComboBox(fontTab);
    QCheckBox *boldCheckBox = new QCheckBox("Bold", fontTab);
    QCheckBox *italicCheckBox = new QCheckBox("Italic", fontTab);

    fontLayout->addWidget(fontFamilyComboBox);
    fontLayout->addWidget(fontSizeComboBox);
    fontLayout->addWidget(boldCheckBox);
    fontLayout->addWidget(italicCheckBox);

    fontTab->setLayout(fontLayout);

    tabWidget->addTab(fontTab, "Font");

    QDockWidget *dock = new QDockWidget("Font Options", this);
    dock->setWidget(tabWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);*/
}


void TextEditor::onupdateSuggestions(const QString& word)
{
    std::vector<std::string> generated=spellCheckerTrie->getSuggestions(word.toStdString());
    for (const std::string& suggestion : generated)
    {
        suggestionList << QString::fromStdString(suggestion);
    }
    emit textHovered();

}

void TextEditor::onTextHovered()
{
    qDebug()<<"inside hover";

    SuggestionsHover::showSuggestions(ui->plainTextEdit, suggestionList, QCursor::pos());
}

void TextEditor::mouseMoveEvent(QMouseEvent* event)
{
    qDebug()<<"inside mouse move event";
    QApplication::sendEvent(ui->plainTextEdit, event);

}

void QPlainTextEdit::mouseMoveEvent(QMouseEvent* event)
{
    // Handle hover events specifically within your QPlainTextEdit widget
    QTextCursor cursor = cursorForPosition(event->pos());
    cursor.select(QTextCursor::WordUnderCursor);
    const QString word = cursor.selectedText();

    if (!word.isEmpty() ) {
    }
}

void TextEditor::on_actionRedo_triggered()
{
    qDebug() << "Redo button clicked.";
    QString previousText = redostack.redo();
    qDebug() << "Redo Text: " << previousText;
    if (!previousText.isEmpty()) {
        ui->plainTextEdit->setPlainText(previousText);
    }
}

void TextEditor::changeFontFamily(int index)
{}

void TextEditor::changeFontSize(int index){}

void TextEditor::toggleBold(bool isChecked){}

void TextEditor::toggleItalic(bool isChecked){}


