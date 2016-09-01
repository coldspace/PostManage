#include "addpost.h"
#include "ui_addpost.h"

AddPost::AddPost(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPost)
{
    ui->setupUi(this);
    this->setModal(true);
    ui->address->hide();
    ui->label_2->hide();
    qRegisterMetaType<PersonInfo>("PersonInfo");
    qRegisterMetaType<PostInfo>("PostInfo");
}

AddPost::~AddPost()
{
    delete ui;
}

void AddPost::AddPostInit(const QMap<int, PersonInfo> &p)
{
    PnInfodemo = p;
    QStringList lists;

    QMap<int, PersonInfo>::const_iterator i;
    for (i = PnInfodemo.cbegin();i != PnInfodemo.cend();i ++)
    {
        lists << QString(QLatin1String(i.value().name));
    }
    ui->admin->addItems(lists);
    ui->friend1->addItems(lists);
    ui->friend2->addItems(lists);
    ui->friend3->addItems(lists);
}

void AddPost::on_Enter_clicked()
{
    QMap<int, PersonInfo>::const_iterator i;
    QByteArray be;
    char *mm;
    i = PnInfodemo.cbegin();
    i += ui->admin->currentIndex();
    PoInfo.Adminname = i.key();

    i = PnInfodemo.cbegin();
    i += ui->friend1->currentIndex();
    PoInfo.Friend1 = i.key();

    i = PnInfodemo.cbegin();
    i += ui->friend2->currentIndex();
    PoInfo.Friend2 = i.key();

    i = PnInfodemo.cbegin();
    i += ui->friend3->currentIndex();
    PoInfo.Friend3 = i.key();

    be = this->ui->goods->text().toLatin1();
    mm = be.data();
    strcpy(PoInfo.goods,mm);

    PoInfo.id = ui->address->text().toInt();

    qDebug()<<PoInfo.Adminname;
    qDebug()<<PoInfo.goods;
    qDebug()<<PoInfo.Friend1;
    qDebug()<<PoInfo.Friend2;
    qDebug()<<PoInfo.Friend3;
    emit SendNewUser(PoInfo);
}

void AddPost::on_cancel_clicked()
{
    this->close();
}

void AddPost::on_admin_activated(const QString &arg1)
{
    ui->admincheck->setChecked(true);
}

void AddPost::on_friend1_activated(const QString &arg1)
{
    ui->friend1check->setChecked(true);
}

void AddPost::on_friend2_activated(const QString &arg1)
{
    ui->friend2check->setChecked(true);
}

void AddPost::on_friend3_activated(const QString &arg1)
{
    ui->friend3check->setChecked(true);
}
