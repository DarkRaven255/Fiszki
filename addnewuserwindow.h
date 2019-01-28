#ifndef ADDNEWUSERWINDOW_H
#define ADDNEWUSERWINDOW_H

#include <QDialog>
#include "session.h"

namespace Ui {
class AddNewUserWindow;
}

class AddNewUserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewUserWindow(Session *session, QWidget *parent = nullptr);
    ~AddNewUserWindow();

public slots:
    void on_buttonBox_accepted();

private:
    Ui::AddNewUserWindow *ui;
    Session *session;
};

#endif // ADDNEWUSERWINDOW_H
