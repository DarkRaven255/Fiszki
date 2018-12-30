#ifndef FISZKIMAINWINDOW_H
#define FISZKIMAINWINDOW_H

#include <QMainWindow>
#include <dbmanager.h>

namespace Ui {
class FiszkiMainWindow;
}

class FiszkiMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FiszkiMainWindow(QWidget *parent = nullptr);
    ~FiszkiMainWindow();

private slots:
    void on_checkBtn_clicked();
    void on_startBtn_clicked();
    void on_exitBtn_clicked();
    void on_userListBtn_clicked();
    void on_learnBtn_clicked();
    void on_endLearnBtn_clicked();
    void on_nextFlashcardBtn_clicked();

private:
    Ui::FiszkiMainWindow *ui;
    DbManager *dbmanager = new DbManager("database.db");

    void Session();
    void Learn();

    QString user;
    QString q_en;
    QString e_en;
    QString q_pl;
    QString e_pl;

    int numberOfQuestions;
};

#endif // FISZKIMAINWINDOW_H
