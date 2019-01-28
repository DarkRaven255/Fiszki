#ifndef MISTAKEWINDOW_H
#define MISTAKEWINDOW_H

#include <QDialog>
#include "session.h"

namespace Ui {
class MistakeWindow;
}

class MistakeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MistakeWindow(const QString &yourAnwser, const QString &rightAnwser, Session *session);
    ~MistakeWindow();

private slots:
    void on_yesPushButton_clicked();
    void on_noPushButton_clicked();

private:
    Ui::MistakeWindow *ui;
    Session *session;
};

#endif // MISTAKEWINDOW_H
