#ifndef ERRORCREATINGUSERWINDOW_H
#define ERRORCREATINGUSERWINDOW_H

#include <QDialog>

namespace Ui {
class ErrorCreatingUserWindow;
}

class ErrorCreatingUserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorCreatingUserWindow(QWidget *parent = nullptr);
    ~ErrorCreatingUserWindow();

private:
    Ui::ErrorCreatingUserWindow *ui;
};

#endif // ERRORCREATINGUSERWINDOW_H
