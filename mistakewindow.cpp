#include "mistakewindow.h"
#include "ui_mistakewindow.h"

MistakeWindow::MistakeWindow(const QString &yourAnwser, const QString &rightAnwser, Session *session):
    ui(new Ui::MistakeWindow),
    session(session)
{
    ui->setupUi(this);
    ui->enteredTextBrowser->setText(yourAnwser);
    ui->rightTextBrowser->setText(rightAnwser);
}

MistakeWindow::~MistakeWindow()
{
    delete ui;
}

void MistakeWindow::on_yesPushButton_clicked()
{
    session->toggleMistake();
    close();
}

void MistakeWindow::on_noPushButton_clicked()
{
    close();
}
