#ifndef SELECTUSERLISTWINDOW_H
#define SELECTUSERLISTWINDOW_H

#include <QDialog>
#include "session.h"

namespace Ui {
class UserListWindow;
}

class UserListWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UserListWindow(Session *session, QWidget *parent = nullptr);
    ~UserListWindow();
    inline void refresh();

private slots:
    void on_okBtn_clicked();
    void on_addNewUserBtn_clicked();
    void on_deleteAllUsersBtn_clicked();
    void on_deleteUser_clicked();

private:
    Ui::UserListWindow *ui;
    Session *session;
    DbManager *dbmanager;

};

#endif // SELECTUSERLISTWINDOW_H
