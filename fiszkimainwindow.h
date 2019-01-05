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
    void on_exitBtn_clicked();
    void on_userListBtn_clicked();
    void on_learnBtn_clicked();
    void on_endLearnBtn_clicked();
    void on_nextFlashcardBtn_clicked();
    void on_backFlashcardBtn_clicked();
    void on_rememberBtn_clicked();
    void on_stopBtn_clicked();
    void on_testBtn_clicked();
    void on_availableUsersComboBox_currentIndexChanged(const QString &arg1);



private:
    Ui::FiszkiMainWindow *ui;
    DbManager *dbmanager = new DbManager("database.db");

    void Test(int i);
    void LoadQuestion(int i, int noBox);
    void RecalculateQuestions();
    void LockBtns();
    void ChceckUserList();

    QString user;

    int q_id;
    QString q_en;
    QString e_en;
    QString q_pl;
    QString e_pl;

    int unknownQuestions;
    int testQuestions;
    int learnQuestions;
    int testCounterQuestions;
    int progressPercent;
};

#endif // FISZKIMAINWINDOW_H
