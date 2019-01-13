#include "addnewuserwindow.h"
#include "ui_addnewuserwindow.h"
#include "errorcreatinguserwindow.h"

AddNewUserWindow::AddNewUserWindow(Session *session, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewUserWindow),
    session(session)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}

AddNewUserWindow::~AddNewUserWindow()
{
    delete ui;
}

void AddNewUserWindow::on_buttonBox_accepted()
{
    if(!session->addUser(ui->lineEdit->text()))
    {
        ErrorCreatingUserWindow errorcreatinguserwindow;
        errorcreatinguserwindow.exec();
    }
}
