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

    ui->questionTextBrowser->setFontPointSize(48);
    ui->questionEnTextBrowser->setFontPointSize(48);
    ui->questionPlTextBrowser->setFontPointSize(48);
    ui->explanationEnTextBrowser->setFontPointSize(10);
    ui->explanationPlTextBrowser->setFontPointSize(10);
    ui->explanationTextBrowser->setFontPointSize(10);
}

FiszkiMainWindow::~FiszkiMainWindow()
{
    delete ui;
    dbmanager->closeUserDB();
}


/////////////////////////////////////////////MAIN MENU PAGE
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


void FiszkiMainWindow::on_availableUsersComboBox_currentIndexChanged(const QString &selectedUser)
{
    user=selectedUser;
    ui->activeUser->setText(user);
    ui->activeUser_2->setText(user);
}


void FiszkiMainWindow::on_learnBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    counterOfQuestions=0;
    on_nextFlashcardBtn_clicked();

    ui->nextFlashcardBtn->setEnabled(true);
    ui->nextFlashcardBtn->setText("Następna fiszka");
}


void FiszkiMainWindow::on_testBtn_clicked()
{
    if(ui->availableUsersComboBox->count()==0)
    {
        AddNewUserWindow addnewuserwindow;
        addnewuserwindow.exec();
    }
    ui->stackedWidget->setCurrentIndex(2);
    Test();
}


////////////////////////////////////LEARN PAGE
void FiszkiMainWindow::Learn()
{
    dbmanager->returnQuestion(counterOfQuestions,q_en,e_en,q_pl,e_pl);

    ui->questionEnTextBrowser->setText(q_en);
    ui->explanationEnTextBrowser->setText(e_en);

    ui->questionPlTextBrowser->setText(q_pl);
    ui->explanationPlTextBrowser->setText(e_pl);

    counterOfQuestions++;
}

void FiszkiMainWindow::RecalculateQuestions()
{
    numberOfQuestions=dbmanager->countQuestions();
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

void FiszkiMainWindow::on_stopBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

/////////////////////////////////////////TEST PAGE
void FiszkiMainWindow::Test()
{
    //ui->questionTextBrowser->
}


void FiszkiMainWindow::on_checkBtn_clicked()
{
    if(ui->enterAnwserLineEdit->text()==q_pl)
    {
        ui->progressBar->setValue(10);
    }
}
