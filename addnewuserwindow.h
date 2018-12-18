#ifndef ADDNEWUSERWINDOW_H
#define ADDNEWUSERWINDOW_H

#include <QDialog>
#include "dbmanager.h"


namespace Ui {
class AddNewUserWindow;
}

class AddNewUserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewUserWindow(QWidget *parent = nullptr);
    ~AddNewUserWindow();

public slots:
    void on_buttonBox_accepted();

private:
    Ui::AddNewUserWindow *ui;
    DbManager *dbmanager;
};

#endif // ADDNEWUSERWINDOW_H
