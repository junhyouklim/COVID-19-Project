#ifndef INFECTINFO_H
#define INFECTINFO_H
#include "Common.h"

class InfectInfo
{
private:
    QString Create_DT; //����Ͻú���
    QString Death_CNT; //����� ��
    QString Def_CNT; //Ȯ���� ��
    QString Gubun; //�õ���
    QString Inc_DEC; //���ϴ�� ������
    QString Isol_Clear_CNT; //�ݸ� ���� ��
    QString Isol_Ing_CNT; //�ݸ��� ȯ�ڼ�
    QString Local_Occ_CNT; //�����߻� ��
    QString Over_Flow_CNT; //�ؿ����� ��
    QString Qur_Rate; //10����� �߻���
    QString Seq; //�Խñ۹�ȣ
    QString Std_Day; //�����Ͻ�
public:
    InfectInfo();
    InfectInfo(QString createDT,QString deathCNT,QString defCNT,QString gubun,QString incDEC,QString isolclearCNT,QString isolingCNT,QString localoccCNT,QString overflowCNT,QString qurrate,QString seq,QString stdday);
    ~InfectInfo();
    void InfectInfoInit(QString createDT,QString deathCNT,QString defCNT,QString gubun,QString incDEC,QString isolclearCNT,QString isolingCNT,QString localoccCNT,QString overflowCNT,QString qurrate,QString seq,QString stdday);
    void InfectInfoInit(QString tagname,QString text);
    //GET
    QString GetCreatDT() const;
    QString GetDeathCNT() const;
    QString GetDefCNT() const;
    QString GetGubun() const;
    QString GetIncDec() const;
    QString GetIsolClearCNT() const;
    QString GetIsolIngCNT() const;
    QString GetLoclOccCNT() const;
    QString GetOverFlowCNT() const;
    QString GetQurRate() const;
    QString GetSeq() const;
    QString GetStdDay() const;

    //SET
    void SetCreatDT(const QString createDT);
    void SetDeathCNT(const QString deathCNT);
    void SetDefCNT(const QString defCNT);
    void SetGubun(const QString gubun);
    void SetIncDec(const QString incDEC);
    void SetIsolClearCNT(const QString isolclearCNT);
    void SetIsolIngCNT(const QString isolingCNT);
    void SetLoclOccCNT(const QString localoccCNT);
    void SetOverFlowCNT(const QString overflowCNT);
    void SetQurRate(const QString qurrate);
    void SetSeq(const QString seq);
    void SetStdDay(const QString stdday);

};

#endif // INFECTINFO_H
