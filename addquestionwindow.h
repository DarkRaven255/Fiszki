#ifndef ADDQUESTIONWINDOW_H
#define ADDQUESTIONWINDOW_H

#include <QDialog>
#include "session.h"

namespace Ui {
class AddQuestionWindow;
}

class AddQuestionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddQuestionWindow(Session *session, QWidget *parent = nullptr);
    ~AddQuestionWindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddQuestionWindow *ui;
    Session *session;
};

#endif // ADDQUESTIONWINDOW_H
