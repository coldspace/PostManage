#ifndef DATATYPE_H
#define DATATYPE_H
#include <QDebug>
#include <QMetaType>
typedef struct{
    int Adminname;
    char goods[20];
    int Friend1;
    int Friend2;
    int Friend3;
    int id;
    int status;
}PostInfo;
Q_DECLARE_METATYPE(PostInfo)

typedef struct{
    char name[20];
    char phone[20];
    int id;
    int status;
}PersonInfo;
Q_DECLARE_METATYPE(PersonInfo)

typedef struct{
    int id;
    int Adminname;
    int Picker;
    char goods[20];
    char time[30];
    int status;
}HistoryInfo;
Q_DECLARE_METATYPE(HistoryInfo)
#endif // DATATYPE_H
