#ifndef DISASTER_HANDLER_H
#define DISASTER_HANDLER_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QTextBrowser>
#include "disaster_msg.h"
#include "disaster_api.h"

namespace Ui {
class Disaster_Handler;
}

class Disaster_Handler : public QDialog
{
    Q_OBJECT
private:
    Ui::Disaster_Handler *ui;
    QVector<Disaster_Msg> loc_msg; // ���ϴ� ����, ��¥ �����
    QString selected_loc; // ������ �����̸�
    QDate selected_date; // ������ ��¥

public:
    explicit Disaster_Handler(QWidget *parent = nullptr);
    ~Disaster_Handler();
    void Get_Msg_Data(); // APIȣ���ؼ� ���ϴ� ������ loc_msg ���Ϳ� ����
    void Set_Msg_UI(); // ȣ���� ������ UI�� ���

private slots:
    void Recv_Selected_Loc(QString selected_loc); // �θ� �������� ���� ���ý� �� �޾ƿ�
    void on_dateEdit_userDateChanged(const QDate &date); // ��¥ �����
};

#endif // DISASTER_HANDLER_H
