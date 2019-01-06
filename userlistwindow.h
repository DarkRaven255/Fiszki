#ifndef SELECTUSERLISTWINDOW_H
#define SELECTUSERLISTWINDOW_H

#include <QDialog>
#include "dbmanager.h"

namespace Ui {
class UserListWindow;
}

class UserListWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UserListWindow(QWidget *parent = nullptr);
    ~UserListWindow();
    inline void refresh();
    DbManager *dbmanager;

private slots:
    void on_okBtn_clicked();
    void on_addNewUserBtn_clicked();
    void on_deleteAllUsersBtn_clicked();
    void on_deleteUser_clicked();

private:
    Ui::UserListWindow *ui;

};

#endif // SELECTUSERLISTWINDOW_H
