#include "userlistwindow.h"
#include "ui_userlistwindow.h"
#include "addnewuserwindow.h"

UserListWindow::UserListWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserListWindow)
{
    ui->setupUi(this);
    ui->okBtn->setFocus();
    refresh();
}

UserListWindow::~UserListWindow()
{
    delete ui;
}

void UserListWindow::on_okBtn_clicked()
{
    close();
}

void UserListWindow::on_addNewUserBtn_clicked()
{
    AddNewUserWindow addnewuserwindow;
    addnewuserwindow.exec();
    if(&AddNewUserWindow::destroyed)
    {
        this->refresh();
        ui->okBtn->setFocus();
    }
}

void UserListWindow::on_deleteAllUsersBtn_clicked()
{
    dbmanager->removeAllUsers();
    refresh();
}

void UserListWindow::refresh()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(dbmanager->returnUserList());
}

void UserListWindow::on_deleteUser_clicked()
{
    if(ui->listWidget->currentItem())
    {
        dbmanager->removeUser(ui->listWidget->currentItem()->text());
        refresh();
    }
}
