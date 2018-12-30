#include "fiszkimainwindow.h"
#include "ui_fiszkimainwindow.h"
#include "userlistwindow.h"
#include "addnewuserwindow.h"

QString xD="";

FiszkiMainWindow::FiszkiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FiszkiMainWindow),
    numberOfQuestions(dbmanager->countQuestions())
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->availableUsersComboBox->addItems(dbmanager->displayAllUsers());
    connect(ui->enterAnwserLineEdit,SIGNAL(returnPressed()),ui->checkBtn,SIGNAL(clicked()));

    ui->questionEnTextBrowser->setFontPointSize(48);
    ui->questionPlTextBrowser->setFontPointSize(48);
    qDebug()<<"ilsoc pytan:"<<numberOfQuestions;

}

FiszkiMainWindow::~FiszkiMainWindow()
{
    delete ui;
    dbmanager->closeUserDB();
}

void FiszkiMainWindow::on_checkBtn_clicked()
{
    if(ui->enterAnwserLineEdit->text()==q_pl)
    {
        ui->progressBar->setValue(10);
    }
}

void FiszkiMainWindow::on_startBtn_clicked()
{
    if(ui->availableUsersComboBox->count()==0)
    {
        AddNewUserWindow addnewuserwindow;
        addnewuserwindow.exec();
    }
    ui->activeUser->setText(ui->availableUsersComboBox->currentText());
    ui->stackedWidget->setCurrentIndex(2);
    Session();
}

void FiszkiMainWindow::on_exitBtn_clicked()
{
    close();
}

void FiszkiMainWindow::on_userListBtn_clicked()
{
    UserListWindow userlistwindow;
    userlistwindow.exec();
    if(&UserListWindow::destroyed)
    {
        ui->availableUsersComboBox->clear();
        ui->availableUsersComboBox->addItems(dbmanager->displayAllUsers());
    }
}

void FiszkiMainWindow::Session()
{
    int i=0;

    while(i<10)
    {

        ui->questionTextBrowser->setFontPointSize(48);
        ui->questionTextBrowser->setAlignment(Qt::AlignRight);
        dbmanager->returnQuestion(xD,i,q_en,e_en,q_pl,e_pl);
        ui->questionTextBrowser->setText(q_en);
        ui->questionTextBrowser_2->setText(e_en);
        i++;
    }
}

void FiszkiMainWindow::Learn()
{
    dbmanager->returnQuestion(counterOfQuestions,q_en,e_en,q_pl,e_pl);

    ui->questionEnTextBrowser->setText(q_en);
    ui->explanationEnTextBrowser->setText(e_en);

    ui->questionPlTextBrowser->setText(q_pl);
    ui->explanationPlTextBrowser->setText(e_pl);

    counterOfQuestions++;
}

void FiszkiMainWindow::on_learnBtn_clicked()
{
    ui->activeUser_2->setText(ui->availableUsersComboBox->currentText());
    ui->stackedWidget->setCurrentIndex(1);

    counterOfQuestions=0;
    on_nextFlashcardBtn_clicked();

    ui->nextFlashcardBtn->setEnabled(true);
    ui->nextFlashcardBtn->setText("Następna fiszka");
}

void FiszkiMainWindow::on_endLearnBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void FiszkiMainWindow::on_nextFlashcardBtn_clicked()
{
    if(counterOfQuestions<numberOfQuestions)
    {
        Learn();
    }
    else
    {
        ui->nextFlashcardBtn->setEnabled(false);
        ui->nextFlashcardBtn->setText("Brak fiszek");
    }
}

void FiszkiMainWindow::on_rememberBtn_clicked()
{
    dbmanager->markAsKnown(q_en);
}
