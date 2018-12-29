#include "fiszkimainwindow.h"
#include "ui_fiszkimainwindow.h"
#include "userlistwindow.h"
#include "addnewuserwindow.h"
#include "session.h"

FiszkiMainWindow::FiszkiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FiszkiMainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->availableUsersComboBox->addItems(dbmanager->displayAllUsers());
    connect(ui->enterAnwserLineEdit,SIGNAL(returnPressed()),ui->checkBtn,SIGNAL(clicked()));
}

FiszkiMainWindow::~FiszkiMainWindow()
{
    delete ui;
}

void FiszkiMainWindow::on_checkBtn_clicked()
{
    if(ui->enterAnwserLineEdit->text()==q_pl)
    {
        ui->progressBar->setValue(10);
    }
}

void FiszkiMainWindow::on_startBtn_clicked()
{
    QString xD="zakupy";
    if(ui->availableUsersComboBox->count()==0)
    {
        AddNewUserWindow addnewuserwindow;
        addnewuserwindow.exec();
    }
    ui->stackedWidget->setCurrentIndex(1);
    Session session = *new Session(ui->availableUsersComboBox->currentText());

    ui->activeUser->setText(ui->availableUsersComboBox->currentText());
    ui->questionTextBrowser->setFontPointSize(48);
    ui->questionTextBrowser->setAlignment(Qt::AlignRight);
    dbmanager->returnQuestion(xD,noQ,q_en,e_en,q_pl);
    ui->questionTextBrowser->setText(q_en);
    ui->questionTextBrowser_2->setText(e_en);
}

void FiszkiMainWindow::on_exitBtn_clicked()
{
    close();
}

void FiszkiMainWindow::on_userListBtn_clicked()
{
    UserListWindow userlistwindow;
    userlistwindow.exec();
    if(&UserListWindow::destroyed)
    {
        ui->availableUsersComboBox->clear();
        ui->availableUsersComboBox->addItems(dbmanager->displayAllUsers());
    }
}
