#ifndef CHARTDATA_H
#define CHARTDATA_H

#include "Common.h"

class ChartData
{
private:
    QString State_DT; //������
    QString Decide_CNT; //Ȯ���� ��
    QString Clear_CNT; //�ݸ�������
    int Daydecide; //�Ϻ� Ȯ���ڼ�
    int Dayclear; //�Ϻ� �ݸ����� ��
public:
    ChartData();
    ~ChartData();
    void DataInit(QString tagname, QString text);
    //GET
    QString GetStateDT() const;
    QString GetDecideCNT() const;
    QString GetClearCNT() const;
    int GetDaydecide() const;
    int GetDayclear() const;

    //SET
    void SetStateDT(const QString statedt);
    void SetDecideCNT(const QString decidecnt);
    void SetClearCNT(const QString clearcnt);
    void SetDaydecide(const int daydecide);
    void SetDayclear(const int dayclear);
};

#endif // CHARTDATA_H
