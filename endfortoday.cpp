#include "endfortoday.h"
#include "ui_endfortoday.h"

EndForToday::EndForToday(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EndForToday)
{
    ui->setupUi(this);
}

EndForToday::~EndForToday()
{
    delete ui;
}

void EndForToday::on_buttonBox_accepted()
{
    close();
}
