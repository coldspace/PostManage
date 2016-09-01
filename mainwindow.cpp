#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->MSaveUser->hide();
    ui->SavePost->hide();
    ui->baud->setCurrentIndex(3);
    setStyleSheet("QTableWidget::item:selected { background-color: rgb(142, 229, 238) }");
    ui->PersonTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->PersonTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->PersonTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->PersonTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->PostTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->PostTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->PostTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->PostTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->HistoryPostInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->HistoryPostInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->HistoryPostInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->HistoryPostInfo->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setWindowIcon(QIcon(":/Mainwindow/icon.ico"));
    this->setWindowTitle("邮件管理系统-郑文201313010115");
    ser = new Serial(this);
    ser->SerialInit(ui->DebugBox);
    qRegisterMetaType<PersonInfo>("PersonInfo");
    qRegisterMetaType<PostInfo>("PostInfo");
    qRegisterMetaType<HistoryInfo>("HistoryInfo");
    connect(this,SIGNAL(PersonChange()),this,SLOT(UpdatePerson()));
    connect(ser,SIGNAL(ReceiveSerial(const QString &)),this,SLOT(DealSerialData(const QString &)));
    connect(this,SIGNAL(PostChange()),this,SLOT(UpdatePost()));
    connect(this,SIGNAL(HistoryChange()),this,SLOT(UpdateHistory()));
    connect(ser,SIGNAL(Openserial()),this,SLOT(on_SerialSwitch_clicked()));
    on_MLoadUser_clicked();
    on_LoadPost_clicked();
    HistoryLoad();
    on_seekserial_clicked();
    timeinterrept = new QTimer(this);

    connect(timeinterrept,SIGNAL(timeout()),this,SLOT(timeinterreptdone()));
//    timeinterrept->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::timeinterreptdone()
{
    QString str;
    str = "OK";
    ser->SerialSendStr(str);
}

bool MainWindow::SavetoFile()
{
    QMap<int, PersonInfo>::const_iterator i;
 //   PersonInfo PerInfo;
    int err;
    QFile file("./Data/in.txt");
    if (!file.open(QFile::WriteOnly))
    {
        //qDebug()<<"Open file failed.";
        ui->MUserInfo->append("Open file failed.");
        ui->MUserInfo->moveCursor(QTextCursor::End);
        return false;
    }
    for (i = PnInfo.constBegin();i!=PnInfo.constEnd();i++)
    {
        err = file.write((char*)&i.value(),sizeof(PersonInfo));
        if (err == -1)
        {
           // qDebug()<<"write file failed.";
            ui->MUserInfo->append("write file failed.");
            ui->MUserInfo->moveCursor(QTextCursor::End);
            file.close();
            return false;
        }
       // qDebug()<<"write file success.";
        ui->MUserInfo->append("write file success.");
        ui->MUserInfo->moveCursor(QTextCursor::End);
    }

    file.close();
    return true;
}

void MainWindow::on_MAddUser_clicked()
{
    adduser = new AddUser(this);
    adduser->setAttribute(Qt::WA_DeleteOnClose,true);
    adduser->setWindowTitle("添加用户");
    connect(adduser,\
            SIGNAL(SendNewUser(const PersonInfo &)),\
            this,SLOT(GetNewUser(const PersonInfo &)));
    adduser->show();
}

/*
 * 1、可以和子界面通信，并且Qmap可以保存结构体
*/
void MainWindow::GetNewUser(const PersonInfo &perinfo)
{
    QString str;
    Pinf = perinfo;

//    str = "01#"+QString("%1").arg(Pinf.id, 3, 10, QChar('0')) + "#";
    str = "01#" + QString(QLatin1String(Pinf.name)) + "#\r\n";
//    qDebug()<<str;
    ui->MUserInfo->append(str);
    ui->MUserInfo->moveCursor(QTextCursor::End);
    ser->SerialSendStr(str);
}

void MainWindow::UpdatePerson()
{
    QMap<int, PersonInfo>::const_iterator i;
    int index = 0;
    QStringList liast;
    liast<<"地址"<<"姓名"<<"电话";
    this->ui->PersonTable->clear();
    this->ui->PersonTable->setRowCount(PnInfo.size());
    this->ui->PersonTable->setHorizontalHeaderLabels(liast);
    for (i = PnInfo.constBegin();i!=PnInfo.constEnd();i++)
    {
        this->ui->PersonTable->setItem(index,0,new QTableWidgetItem(QString::number(i.key())));
        this->ui->PersonTable->setItem(index,1,new QTableWidgetItem(QString(QLatin1String(i.value().name))));
        this->ui->PersonTable->setItem(index,2,new QTableWidgetItem(QString(QLatin1String(i.value().phone))));
        index ++;
    }
}

void MainWindow::on_MSaveUser_clicked()
{
    SavetoFile();
}

bool MainWindow::on_MLoadUser_clicked()
{
    int err;
    PersonInfo hah;
    QFile file("./Data/in.txt");

    if (!file.open(QFile::ReadOnly))
    {
//        qDebug()<<"Open file failed.";
        ui->DebugBox->append("Open file failed.");
        ui->DebugBox->moveCursor(QTextCursor::End);
        return false;
    }

    if (file.isOpen())
    {
//        qDebug()<<"file has been opened";
        ui->DebugBox->append("file has been opened");
        ui->DebugBox->moveCursor(QTextCursor::End);
    }
    PnInfo.clear();
    while(!file.atEnd())
    {
        err = file.read((char*)&hah,sizeof(PersonInfo));
        if (err == -1)
        {
            ui->DebugBox->append("read file faile.");
            ui->DebugBox->moveCursor(QTextCursor::End);
            file.close();
            return false;
        }
        else
        {
            PnInfo.insert(hah.id,hah);
//            qDebug()<<hah.id<< "  "<<hah.name<<"  "<<hah.phone;
            ui->DebugBox->append("\nid: "+QString::number(hah.id));
            ui->DebugBox->append("name: "+QString(QLatin1String(hah.name)));
            ui->DebugBox->append("Phone: "+QString(QLatin1String(hah.phone)));
            ui->DebugBox->moveCursor(QTextCursor::End);
        }
    }
    emit PersonChange();
    file.close();
    return true;
}

void MainWindow::on_MDeleteUser_clicked()
{
    QString str;
    int row;
    row = this->ui->PersonTable->currentRow();
    if (row < 0)
        return;
    int id = this->ui->PersonTable->item(row,0)->text().toInt();
    Pinf.id = id;

    str = "02#"+QString("%1").arg(id, 3, 10, QChar('0')) + "#\r\n";
    ser->SerialSendStr(str);
}

void MainWindow::on_seekserial_clicked()
{
    //QMessageBox::information(this,"err","asdfa",QMessageBox::Ok);
    ser->SearchSerial(ui->Serial);
}

void MainWindow::on_SerialSwitch_clicked()
{
    if (!ser->GetSerialStatus())
    {
        ser->SerialOpen(ui->Serial->currentIndex());
        if (ser->GetSerialStatus())
        {
            ui->SerialSwitch->setText("关闭串口");
            ui->Serial->setEnabled(false);
        }
    }
    else
    {
        ser->SerialClose();
        if (!ser->GetSerialStatus())
        {
            ui->SerialSwitch->setText("打开串口");
            ui->Serial->setEnabled(true);
        }
    }
}

void MainWindow::on_DAddUser_clicked()
{
    QString str;
    adduser = new AddUser(this);
    char mm[20] = {"hahha"};
    char ph[20] = {"132456789"};
    strcpy(Pinf.name,mm);
    strcpy(Pinf.phone,ph);
    str = "01#001#";
    ser->SerialSendStr(str);
}

void MainWindow::on_DDeleteUser_clicked()
{
    QString str;
    str = "02#000#hahaha#";
    ser->SerialSendStr(str);
}

void MainWindow::DealSerialData(const QString &str)
{
    QString text;
    text = str;
    this->ui->MUserInfo->append(text);
    ui->MUserInfo->moveCursor(QTextCursor::End);
    switch(text.left(2).toInt())
    {
        case 1:qDebug()<<"01";addperson(text);break;
        case 2:qDebug()<<"02";deleteperson(text);break;
        case 3:qDebug()<<"03";addpost(text);break;
        case 4:qDebug()<<"04";break;
        case 5:qDebug()<<"05";deletepost(text);break;
        case 6:qDebug()<<"06";AddHistory(text);deletepost(text);break;
   // default:qDebug()<<"其他调试信息";break;
    }
}

void MainWindow::addperson(QString &str)
{
    QMap<int, PersonInfo>::const_iterator i;
    QString text = str.left(6);
    text = text.right(3);
    Pinf.id = text.toInt();

    ui->MUserInfo->append("Get New ID:" + QString::number(Pinf.id));
    ui->MUserInfo->moveCursor(QTextCursor::End);
//    qDebug()<<"Get New ID:"<<Pinf.id;
    QMessageBox::information(this,\
                             "Info","Get New ID:"+QString::number(Pinf.id),\
                             QMessageBox::Yes);
    Pinf.status = 1;
    PnInfo.insert(Pinf.id,Pinf);
    SavetoFile();

    for(i = PnInfo.constBegin();i!=PnInfo.cend();i ++)
    {
        ui->MUserInfo->append("\nid: "+QString::number(i.key()));
        ui->MUserInfo->append("name: "+QString(QLatin1String(i.value().name)));
        ui->MUserInfo->append("Phone: "+QString(QLatin1String(i.value().phone)));
        ui->MUserInfo->moveCursor(QTextCursor::End);
    }
    adduser->close();
    emit PersonChange();
}

void MainWindow::deleteperson(QString &str)
{
    int result;
    QString text = str.left(6);
    text = text.right(3);
    if(text.toInt() == Pinf.id)
    {
        result = QMessageBox::information(this,"warning","确定删除数据！",QMessageBox::Yes,QMessageBox::No);

        if (result == QMessageBox::Yes)
        {
            PnInfo.remove(text.toInt());

            SavetoFile();
            emit PersonChange();
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    QMap<int, PersonInfo>::const_iterator i;
    int index = 0;
    ui->SearchUser->text();

    for(i = PnInfo.constBegin();i!=PnInfo.cend();i ++)
    {
        if (QString(QLatin1String(i.value().name)) == ui->SearchUser->text())
        {
            ui->PersonTable->selectRow(index);
        }
        index ++;
    }
}

/*邮件部分*/
void MainWindow::on_AddPost_clicked()
{
    addpostdialog = new AddPost(this);
    addpostdialog->setAttribute(Qt::WA_DeleteOnClose,true);
    addpostdialog->setWindowTitle("添加邮件");
    connect(addpostdialog,\
            SIGNAL(SendNewUser(const PostInfo &)),\
            this,SLOT(GetNewPost(const PostInfo &)));
    addpostdialog->AddPostInit(PnInfo);
    addpostdialog->show();
}

void MainWindow::GetNewPost(const PostInfo &poInfo)
{
    QString str;
    poinf = poInfo;

    //str = "03#"+QString("%1").arg(poinf.id, 3, 10, QChar('0')) + "#";
    str = "03#" + QString("%1").arg(poinf.Adminname, 3, 10, QChar('0'))+"#"+\
            QString("%1").arg(poinf.Friend1, 3, 10, QChar('0'))+"#"+\
            QString("%1").arg(poinf.Friend2, 3, 10, QChar('0'))+"#"+\
            QString("%1").arg(poinf.Friend3, 3, 10, QChar('0'))+"#"+\
            QString(QLatin1String(poinf.goods))+"#\r\n";
    ser->SerialSendStr(str);
}

void MainWindow::addpost(QString &str)
{
    QMap<int, PostInfo>::const_iterator i;
    QString text = str.left(6);
    text = text.right(3);
    poinf.id = text.toInt();
    poinf.status = 1;

//    qDebug()<<"Get New ID:"<<poinf.id;
    ui->MPostInfo->append("Get New ID:" + QString::number(poinf.id));
    ui->MPostInfo->moveCursor(QTextCursor::End);

    poinf.status = 1;
    PoInfo.insert(poinf.id,poinf);
    PostSavetoFile();

    for(i = PoInfo.constBegin();i!=PoInfo.cend();i ++)
    {
        qDebug()<<i.key()<< "  "<<i.value().Adminname<<"  "<<i.value().goods;
    }
    addpostdialog->close();
    emit PostChange();
}

void MainWindow::deletepost(QString &str)
{
    QString text = str.left(6);
    text = text.right(3);
    if(text.toInt() == poinf.id)
    {
        //result = QMessageBox::information(this,"warning","确定删除数据！",QMessageBox::Yes,QMessageBox::No);
//        qDebug()<<"快递被取，或者被删除";
        ui->MPostInfo->append("快递被取，或者被删除");
        ui->MPostInfo->moveCursor(QTextCursor::End);
      //  if (result == QMessageBox::Yes)
        {
            PoInfo.remove(text.toInt());

            PostSavetoFile();
            emit PostChange();
        }
    }
}

void MainWindow::UpdatePost()
{
    QMap<int, PostInfo>::const_iterator i;
    int index = 0;
    QStringList liast;
    liast<<"地址"<<"邮件主人"<<"物品名称"<<"朋友1"<<"朋友2"<<"朋友3";
    this->ui->PostTable->clear();
    this->ui->PostTable->setRowCount(PoInfo.size());
    this->ui->PostTable->setHorizontalHeaderLabels(liast);
    for (i = PoInfo.constBegin();i!=PoInfo.constEnd();i++)
    {
        this->ui->PostTable->setItem(index,0,new QTableWidgetItem(QString::number(i.key())));
        this->ui->PostTable->setItem(index,1,new QTableWidgetItem(QString(QLatin1String(PnInfo.find(i.value().Adminname).value().name))));
        this->ui->PostTable->setItem(index,2,new QTableWidgetItem(QString(QLatin1String(i.value().goods))));
        this->ui->PostTable->setItem(index,3,new QTableWidgetItem(QString(QLatin1String(PnInfo.find(i.value().Friend1).value().name))));
        this->ui->PostTable->setItem(index,4,new QTableWidgetItem(QString(QLatin1String(PnInfo.find(i.value().Friend2).value().name))));
        this->ui->PostTable->setItem(index,5,new QTableWidgetItem(QString(QLatin1String(PnInfo.find(i.value().Friend3).value().name))));
        index ++;
    }
}

bool MainWindow::PostSavetoFile()
{
    QMap<int, PostInfo>::const_iterator i;
 //   PersonInfo PerInfo;
    int err;
    QFile file("./Data/postin.txt");
    if (!file.open(QFile::WriteOnly))
    {
        qDebug()<<"Open file failed.";
        return false;
    }
    for (i = PoInfo.constBegin();i!=PoInfo.constEnd();i++)
    {
        err = file.write((char*)&i.value(),sizeof(PostInfo));
        if (err == -1)
        {
            qDebug()<<"write file failed.";
            file.close();
            return false;
        }
 //       qDebug()<<"write file success.";
        ui->MPostInfo->append("write file success.");
        ui->MPostInfo->moveCursor(QTextCursor::End);
    }

    file.close();
    return true;
}

bool MainWindow::PostLoad()
{
    int err;
    PostInfo hah;
    QFile file("./Data/postin.txt");

    if (!file.open(QFile::ReadOnly))
    {
        qDebug()<<"Open file failed.";
        return false;
    }

    if (file.isOpen())
    {
        qDebug()<<"file has been opened";
    }
    PoInfo.clear();
    while(!file.atEnd())
    {
        err = file.read((char*)&hah,sizeof(PostInfo));
        if (err == -1)
        {
            qDebug()<<"read file faile.";
            file.close();
            return false;
        }
        else
        {
            PoInfo.insert(hah.id,hah);
            qDebug()<<hah.id<< "  "<<hah.Adminname<<"  "<<hah.goods;
        }
    }
    emit PostChange();
 //   emit PersonChange();
    file.close();
    return true;
}

void MainWindow::on_LoadPost_clicked()
{
    PostLoad();
}

void MainWindow::on_RemovePost_clicked()
{
    QString str;
    int row;
    row = this->ui->PostTable->currentRow();
    if (row < 0)
        return;
    int id = this->ui->PostTable->item(row,0)->text().toInt();
    poinf.id = id;

    str = "05#"+QString("%1").arg(id, 3, 10, QChar('0')) + "#\r\n";
    ser->SerialSendStr(str);
    qDebug()<<id;
}

void MainWindow::on_SearchPost_clicked()
{
    QMap<int, PostInfo>::const_iterator i;
    int index = 0;

    for (i = PoInfo.constBegin();i!=PoInfo.constEnd();i++)
    {
        if ((QString(QLatin1String(PnInfo.find(i.value().Adminname).value().name)) == ui->Searchpost->text()) ||
                (QString(QLatin1String(PnInfo.find(i.value().Friend1).value().name)) == ui->Searchpost->text()) ||
                (QString(QLatin1String(PnInfo.find(i.value().Friend2).value().name)) == ui->Searchpost->text()) ||
                (QString(QLatin1String(PnInfo.find(i.value().Friend3).value().name)) == ui->Searchpost->text()) ||
                (QString(QLatin1String(i.value().goods)) == ui->Searchpost->text())){
            ui->PostTable->selectRow(index);
        }
        index ++;
    }
}

/*历史记录部分*/
void MainWindow::on_PickPost_clicked()
{
    QString str;
    int row;
    row = this->ui->PostTable->currentRow();
    if (row < 0)
        return;
    int id = this->ui->PostTable->item(row,0)->text().toInt();
    poinf.id = id;

    str = "06#"+QString("%1").arg(id, 3, 10, QChar('0')) + "#"+"000#";
    ser->SerialSendStr(str);
    qDebug()<<id;
}

void MainWindow::AddHistory(QString &str)
{
    QMultiMap<int, HistoryInfo>::const_iterator i;
    QString text;
    QByteArray be;
    char *mm;
    QString current_date;
    QDateTime current_date_time = QDateTime::currentDateTime();
    current_date = current_date_time.toString("yyyy.MM.dd");
    be = current_date.toLatin1();
    mm = be.data();

    text = str.left(6);
    text = text.right(3);
    HInfo.id = text.toInt();
    HInfo.status = 1;
    HInfo.Adminname = PoInfo.find(HInfo.id).value().Adminname;

    text = str.right(6);
    text = text.left(3);
    HInfo.Picker = text.toInt();
    strcpy(HInfo.goods,PoInfo.find(HInfo.id).value().goods);
    strcpy(HInfo.time,mm);

    qDebug()<<"Pick ID:"<<HInfo.id;
    qDebug()<<"Picker ID:"<<PnInfo.find(HInfo.Picker).value().name;
    qDebug()<<"Pick time:"<<HInfo.time;
    HisInfo.insert(HInfo.id,HInfo);
    HistorySavetoFile();
    for(i = HisInfo.constBegin();i!=HisInfo.cend();i ++)
    {
        qDebug()<<i.key()<< "  "<<i.value().Adminname<<"  "<<i.value().goods << "  "<<i.value().Picker;
    }
    emit HistoryChange();
}

void MainWindow::UpdateHistory()
{
    QMultiMap<int, HistoryInfo>::const_iterator i;
    int index = 0;
    QStringList liast;
    liast<<"地址"<<"邮件主人"<<"物品名称"<<"取件人"<<"取件时间";
    this->ui->HistoryPostInfo->clear();
    this->ui->HistoryPostInfo->setRowCount(HisInfo.size());
    this->ui->HistoryPostInfo->setHorizontalHeaderLabels(liast);
    for (i = HisInfo.cbegin();i!=HisInfo.cend();i++)
    {
        this->ui->HistoryPostInfo->setItem(index,0,new QTableWidgetItem(QString::number(i.key())));
        this->ui->HistoryPostInfo->setItem(index,1,new QTableWidgetItem(QString(QLatin1String(PnInfo.find(i.value().Adminname).value().name))));
        this->ui->HistoryPostInfo->setItem(index,2,new QTableWidgetItem(QString(QLatin1String(i.value().goods))));
        this->ui->HistoryPostInfo->setItem(index,3,new QTableWidgetItem(QString(QLatin1String(PnInfo.find(i.value().Picker).value().name))));
        this->ui->HistoryPostInfo->setItem(index,4,new QTableWidgetItem(QString(QLatin1String(i.value().time))));
        index ++;
    }
}

bool MainWindow::HistorySavetoFile()
{
    QMultiMap<int, HistoryInfo>::const_iterator i;
    int err;
    QFile file("./Data/History.txt");
    if (!file.open(QFile::WriteOnly))
    {
        qDebug()<<"Open file failed.";
        return false;
    }
    for (i = HisInfo.constBegin();i!=HisInfo.constEnd();i++)
    {
        err = file.write((char*)&i.value(),sizeof(HistoryInfo));
        if (err == -1)
        {
            qDebug()<<"History.txt "<<"write file failed.";
            file.close();
            return false;
        }
        qDebug()<<"History.txt "<<"write file success.";
    }

    file.close();
    return true;
}

bool MainWindow::HistoryLoad()
{
    int err;
    HistoryInfo hah;
    QFile file("./Data/History.txt");

    if (!file.open(QFile::ReadOnly))
    {
        qDebug()<<"Open file failed.";
        return false;
    }

    if (file.isOpen())
    {
        qDebug()<<"file has been opened";
    }
    HisInfo.clear();
    while(!file.atEnd())
    {
        err = file.read((char*)&hah,sizeof(HistoryInfo));
        if (err == -1)
        {
            qDebug()<<"read file faile.";
            file.close();
            return false;
        }
        else
        {
            HisInfo.insert(hah.id,hah);
            qDebug()<<hah.id<< "  "<<hah.Adminname<<"  "<<hah.goods << "   "<<hah.Picker;
        }
    }
    emit HistoryChange();
    file.close();
    return true;
}

void MainWindow::on_ClearHistory_clicked()
{
    HisInfo.clear();
    HistorySavetoFile();
    emit HistoryChange();
}

void MainWindow::on_LoadHistory_clicked()
{
    HistoryLoad();
}

void MainWindow::on_baud_activated(int index)
{
    ser->setBaud(ui->baud->itemText(index).toLong());
}

void MainWindow::on_DClearUser_clicked()
{
    int err;
    QString str;
    err = QMessageBox::information(this,"warning","是否清除用户",QMessageBox::Yes,QMessageBox::No);
    if (err == QMessageBox::Yes)
    {
        str = "08#\r\n";
        if (ser->SerialSendStr(str))
        {
            PnInfo.clear();
            SavetoFile();
            emit PersonChange();
        }
    }
}

void MainWindow::on_DClearPost_clicked()
{
    int err;
    QString str;
    err = QMessageBox::information(this,"warning","是否清除邮件",QMessageBox::Yes,QMessageBox::No);
    if (err == QMessageBox::Yes)
    {
        str = "09#\r\n";
        if (ser->SerialSendStr(str))
        {
            PoInfo.clear();
            SavetoFile();
            emit PostChange();
        }
    }
}

void MainWindow::on_DclearFinger_clicked()
{
    int err;
    QString str;
    err = QMessageBox::information(this,"warning","是否清除指纹库",QMessageBox::Yes,QMessageBox::No);
    if (err == QMessageBox::Yes)
    {
        str = "10#\r\n";
        qDebug()<<str;
        ser->SerialSendStr(str);
    }
}
