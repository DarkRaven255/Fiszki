#include "fiszkimainwindow.h"
#include "ui_fiszkimainwindow.h"
#include "userlistwindow.h"
#include "addnewuserwindow.h"

FiszkiMainWindow::FiszkiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FiszkiMainWindow),
    unknownQuestions(dbmanager->countQuestions(-1)),
    testQuestions(dbmanager->countQuestions(6))
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

    qDebug()<<"Liczba pytan:"<<testQuestions+unknownQuestions;
    qDebug()<<"Liczba nieznanych pytan:"<<unknownQuestions;
    qDebug()<<"Liczba znanych pytan:"<<testQuestions;
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

    learnQuestions=0;
    LoadQuestion(0,-1);

    if(unknownQuestions>0) ui->nextFlashcardBtn->setEnabled(true);
    else LockBtns();
}


void FiszkiMainWindow::on_testBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    Test();
}


void FiszkiMainWindow::ChceckUserList()
{
    if(ui->availableUsersComboBox->count()==0)
    {
        AddNewUserWindow addnewuserwindow;
        addnewuserwindow.exec();
    }
}


////////////////////////////////////LEARN PAGE
void FiszkiMainWindow::LoadQuestion(int i, int noBox)
{
    learnQuestions+=i;
    if(learnQuestions==unknownQuestions-1)
    {
        ui->nextFlashcardBtn->setEnabled(false);
    }
    else if(learnQuestions==0)
    {
        ui->backFlashcardBtn->setEnabled(false);
    }
    else if(learnQuestions>unknownQuestions-1)
    {
        learnQuestions=unknownQuestions-1;
        ui->nextFlashcardBtn->setEnabled(false);
    }
    else if(learnQuestions<0)
    {
        learnQuestions=0;
        ui->backFlashcardBtn->setEnabled(false);
    }

    if (unknownQuestions==1)
    {
        ui->backFlashcardBtn->setEnabled(false);
        ui->nextFlashcardBtn->setEnabled(false);
    }

    if(unknownQuestions==0)
    {
        LockBtns();
    }
    else
    {
        dbmanager->returnQuestion(learnQuestions,noBox,q_en,e_en,q_pl,e_pl);

        ui->questionEnTextBrowser->setText(q_en);
        ui->explanationEnTextBrowser->setText(e_en);

        ui->questionPlTextBrowser->setText(q_pl);
        ui->explanationPlTextBrowser->setText(e_pl);
    }
}

void FiszkiMainWindow::RecalculateQuestions()
{
    unknownQuestions=dbmanager->countQuestions(-1);
}

void FiszkiMainWindow::LockBtns()
{
    ui->questionEnTextBrowser->setText("Zobaczyłeś");
    ui->explanationEnTextBrowser->setText("wszystkie");
    ui->questionPlTextBrowser->setText("fiszki");
    ui->explanationPlTextBrowser->setText("w bazie!");
    ui->nextFlashcardBtn->setEnabled(false);
    ui->backFlashcardBtn->setEnabled(false);
    ui->rememberBtn->setEnabled(false);
}

void FiszkiMainWindow::on_endLearnBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void FiszkiMainWindow::on_nextFlashcardBtn_clicked()
{
    LoadQuestion(1,-1);
    if(!ui->backFlashcardBtn->isEnabled())ui->backFlashcardBtn->setEnabled(true);
}


void FiszkiMainWindow::on_backFlashcardBtn_clicked()
{
    LoadQuestion(-1,-1);
    if(!ui->nextFlashcardBtn->isEnabled())ui->nextFlashcardBtn->setEnabled(true);
}


void FiszkiMainWindow::on_rememberBtn_clicked()
{
    dbmanager->markAsKnown(q_en);
    RecalculateQuestions();
    LoadQuestion(0,-1);
}

void FiszkiMainWindow::on_stopBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

/////////////////////////////////////////TEST PAGE
void FiszkiMainWindow::Test()
{
    int noBox=6;
    dbmanager->returnQuestion(learnQuestions,noBox,q_en,e_en,q_pl,e_pl);

    ui->questionTextBrowser->setText(q_en);
    ui->explanationTextBrowser->setText(e_en);
}


void FiszkiMainWindow::on_checkBtn_clicked()
{
    if(ui->enterAnwserLineEdit->text()==q_pl)
    {
        ui->progressBar->setValue(10);
    }
}
