#include "addnewuserwindow.h"
#include "ui_addnewuserwindow.h"
#include "errorcreatinguserwindow.h"

AddNewUserWindow::AddNewUserWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewUserWindow)
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
    if(!dbmanager->addUser(ui->lineEdit->text()))
    {
        ErrorCreatingUserWindow errorcreatinguserwindow;
        errorcreatinguserwindow.exec();
    }
}
