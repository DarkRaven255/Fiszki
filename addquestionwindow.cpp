#include "addquestionwindow.h"
#include "ui_addquestionwindow.h"

AddQuestionWindow::AddQuestionWindow(Session *session, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddQuestionWindow),
    session(session)
{
    ui->setupUi(this);
}

AddQuestionWindow::~AddQuestionWindow()
{
    delete ui;
}

void AddQuestionWindow::on_buttonBox_accepted()
{
    session->addWordToDB(ui->inputWordEnTextEdit->toPlainText(),
                     ui->inputExplanationEnTextEdit->toPlainText(),
                     ui->inputWordPlTextEdit->toPlainText(),
                     ui->inputExplanationPlTextEdit->toPlainText());
}
