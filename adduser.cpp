#include "adduser.h"
#include "ui_adduser.h"

AddUser::AddUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);
    this->setModal(true);
    ui->Addess->hide();
    ui->label_3->hide();
    qRegisterMetaType<PersonInfo>("PersonInfo");
}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::on_SaveButton_clicked()
{
    QByteArray be;
    char *mm;
    PInfo.id = this->ui->Addess->text().toInt();

    be = this->ui->NameEdit->text().toLatin1();
    mm = be.data();
    strcpy(PInfo.name,mm);

    be = this->ui->PhoneEdit->text().toLatin1();
    mm = be.data();
    strcpy(PInfo.phone,mm);

    emit SendNewUser(PInfo);
}

void AddUser::on_CancelButton_clicked()
{
    this->close();
}
