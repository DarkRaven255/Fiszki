#include "errorcreatinguserwindow.h"
#include "ui_errorcreatinguserwindow.h"

ErrorCreatingUserWindow::ErrorCreatingUserWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorCreatingUserWindow)
{
    ui->setupUi(this);
}

ErrorCreatingUserWindow::~ErrorCreatingUserWindow()
{
    delete ui;
}
