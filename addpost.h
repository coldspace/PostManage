#ifndef ADDPOST_H
#define ADDPOST_H

#include <QDialog>
#include <QDebug>

/**/
#include "datatype.h"

namespace Ui {
class AddPost;
}

class AddPost : public QDialog
{
    Q_OBJECT

public:
    explicit AddPost(QWidget *parent = 0);
    ~AddPost();
    void AddPostInit(const QMap<int,PersonInfo> &);

signals:
    void SendNewUser(const PostInfo &);

private slots:
    void on_Enter_clicked();

    void on_cancel_clicked();

    void on_admin_activated(const QString &arg1);

    void on_friend1_activated(const QString &arg1);

    void on_friend2_activated(const QString &arg1);

    void on_friend3_activated(const QString &arg1);

private:
    Ui::AddPost *ui;
    QMap<int,PersonInfo> PnInfodemo;
    PostInfo PoInfo;
};

#endif // ADDPOST_H
