#include "googledialog.h"
#include "ui_googledialog.h"

GoogleDialog::GoogleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoogleDialog)
{
    ui->setupUi(this);
    setWindowTitle("Googling");
    qApp->installEventFilter(this);
    ui->label->installEventFilter(this);
    //
    manager = new QNetworkAccessManager(this);
    //connect(manager, SIGNAL(finished(QNetworkReply*)),
    //        this, SLOT(reply_finished(QNetworkReply*)));
    connect(manager, &QNetworkAccessManager::finished,
            this, &GoogleDialog::reply_finished);
    //connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);

    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();
    qDebug() << QSslSocket::sslLibraryVersionString();

    ui->listWidget->setStyleSheet("QListView::item:selected {background-color : rgb(204, 204, 255); border: 3px solid rgb(153, 153, 255);}");
    //{background-color : rgb(204, 204, 255); border: 2px dotted yellow;}
}

void GoogleDialog::TakeOverSock(QTcpSocket *sock)
{
    this->sock=sock;
}

GoogleDialog::~GoogleDialog()
{
    delete ui;
}

void GoogleDialog::on_pushButton_clicked()
{
    QString link;
    QString key;
    QString spot;
    QString combine_link;
    link="https://maps.googleapis.com/maps/api/geocode/json?address=";
    key="&key=AIzaSyD4KYKfd7YvfuGRfAzq24kxNt7B7C_ueGs";

    spot=ui->lineEdit->text().trimmed();
    if(spot.isEmpty()==1)
    {
        qDebug()<<QString::fromLocal8Bit("���Է�...");
        QMessageBox::information(this,QString::fromLocal8Bit("�޽���"),QString::fromLocal8Bit("��Ҹ� ���� �Է��ϼ���."),QMessageBox::Ok);
        return;
    }

    if(spot.contains(" "))
    {
        spot.replace(" ","+");
        spot.replace("_","+");
    }

    combine_link=link+spot+key;
    qDebug()<<QString::fromLocal8Bit("spot: ")<<spot;
    qDebug()<<QString::fromLocal8Bit("API ����...");
    request.setUrl(QUrl(combine_link));
    manager->get(request);
    //manager->get(QNetworkRequest(QUrl(combine_link)));
}

void GoogleDialog::reply_finished(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QByteArray array = reply->readAll();
    QJsonDocument document(QJsonDocument::fromJson(array));

    QJsonObject document_object = document.object(); // => ��üȭ...
    QJsonArray document_array = document_object["results"].toArray();
    QStringList document_key = document_object.keys();

    qDebug() << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&";
    qDebug() << ">>>>>> 1. document_object : " << document_object;
    qDebug() << ">>>>>> 2. document_key : " << document_key;
    qDebug() << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&";

    QJsonObject results_object = document_array[0].toObject();
    QJsonObject geometry_object = results_object["geometry"].toObject();
    QJsonObject location_object = geometry_object["location"].toObject();
    QJsonValue lat_value = location_object["lat"];
    QJsonValue lng_value = location_object["lng"];
    qDebug() << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&";
    qDebug() << ">>>>>> 1. geometry_object : " << geometry_object;
    qDebug() << ">>>>>> 2. location_object : " << location_object;
    qDebug() << ">>>>>> 3. lat_value : " << lat_value;
    qDebug() << ">>>>>> 4. lng_value : " << lng_value;
    qDebug() << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&";
    //QJsonValue lat_value = location_object.value(QString("lat"));
    //QJsonValue lng_value = location_object.value(QString("lng"));
    lat=lat_value.toDouble();
    qDebug()<<"[lat] => "<<lat;
    lng=lng_value.toDouble();
    qDebug()<<"[lng] => "<<lng;

    qDebug()<<QString::fromLocal8Bit("�ñ׳ΰ� ��ġ Ŭ���� ������ ��...");
    SendSignalAndPoint();
    qDebug()<<QString::fromLocal8Bit("��� Ŭ���� �޴� ��...");
    RecvLocation();
    qDebug()<<QString::fromLocal8Bit("���� ��� Ŭ���� ����� ��...");
    qDebug()<<"l_cnt: "<<l_cnt;
    if(l_cnt>0)
    {
        for(int i=0; i<l_cnt; i++)
        {
            l[i].ShowLocation();
        }
    }
    qDebug()<<QString::fromLocal8Bit("��� ��...");
    ArrangList();
    qDebug()<<QString::fromLocal8Bit("����...");

    /*
    if(l_cnt>0)
    {
        for(int i=0; i<l_cnt; i++)
        {
            QString address(l[i].GetAddress());
            Black_Sheep_Wall(address, l[i]);
        }
    }
    */
}

void GoogleDialog::SendSignalAndPoint()
{
    p.SetX(lat); p.SetY(lng); p.ShowXY();

    signal=SIGNAL_MAP;
    sock->waitForBytesWritten();
    sock->write((char *)&signal,sizeof(int));

    sock->waitForBytesWritten();
    sock->write((char*)&p,sizeof(Point));
}

void GoogleDialog::RecvLocation()
{
    sock->waitForReadyRead();
    sock->read((char*)&l_cnt, sizeof(int));
    if(l_cnt==0)
    {
        qDebug() <<QString::fromLocal8Bit("�˻� ��� => X");
    }
    else
    {
        QThread::sleep(2);
        sock->waitForReadyRead();
        sock->read((char*)&l, sizeof(Location)*10);
        qDebug()<<"l_cnt: "<<l_cnt;
        /*
        if(l_cnt>0)
        {
            for(int i=0; i<l_cnt; i++)
            {
                l[i].ShowLocation();
            }
        }
        */
    }
}

void GoogleDialog::ArrangList()
{
    ui->listWidget->clear();
    for(int i=0; i<l_cnt; i++)
    {
        QString province(l[i].GetProvince());
        QString city(l[i].GetCity());
        QString medical_institution(l[i].GetMedical());
        QString address(l[i].GetAddress());
        if(address.contains(" "))
        {
            address.replace("_"," ");
        }
        QString weekday_operating_hours(l[i].GetWeekday());
        QString saturday_operating_hours(l[i].GetSaturday());
        QString sunday_operating_hours(l[i].GetSunday());
        QString representative_phone(l[i].GetPhone());
        ui->listWidget->addItem(QString::fromLocal8Bit("%1��° ���\n�õ�: %2\t�ñ���: %3\t�Ƿ�����: %4\n�ּ�: %5\n���� ��ð�: %6\t����� ��ð�: %7\n�Ͽ���/������ ��ð�: %8\n��ǥ��ȭ: %9\n").
                                arg(i+1).arg(province).arg(city).arg(medical_institution).arg(address).arg(weekday_operating_hours).arg(saturday_operating_hours).arg(sunday_operating_hours).arg(representative_phone));
    }
}

void GoogleDialog::on_QWebBtn_clicked()
{
    qDebug()<<ui->listWidget->count();
    qDebug()<<ui->listWidget->currentRow();
    if(ui->listWidget->count() == 0)
    {
        return;
    }
    else if(ui->listWidget->currentRow() == -1)
    {
        qDebug()<<QString::fromLocal8Bit("�̼���...");
        QMessageBox::warning(this,QString::fromLocal8Bit("�޽���"),QString::fromLocal8Bit("���� ���� ��Ҹ� ���� �����ϼ���."),QMessageBox::Ok);
        return;
    }

    QString medical_institution(l[ui->listWidget->currentRow()].GetMedical());
    qDebug()<<medical_institution;
    g.Black_Sheep_Wall(medical_institution, l[ui->listWidget->currentRow()], QWEB);
}

void GoogleDialog::on_QDeskBtn_clicked()
{
    qDebug()<<ui->listWidget->count();
    qDebug()<<ui->listWidget->currentRow();
    if(ui->listWidget->count() == 0)
    {
        return;
    }
    else if(ui->listWidget->currentRow() == -1)
    {
        qDebug()<<QString::fromLocal8Bit("�̼���...");
        QMessageBox::information(this,QString::fromLocal8Bit("�޽���"),QString::fromLocal8Bit("���� ���� ��Ҹ� ���� �����ϼ���."),QMessageBox::Ok);
        return;
    }

    QString medical_institution(l[ui->listWidget->currentRow()].GetMedical());
    qDebug()<<medical_institution;
    g.Black_Sheep_Wall(medical_institution, l[ui->listWidget->currentRow()], QDESK);
}

void GoogleDialog::on_BackBtn_clicked()
{
    this->close();
}

void GoogleDialog::_handleWhatsThisEntry(QWidget * /*sender*/) {
    qDebug() << "QEvent::EnterWhatsThisMode";
    QMessageBox::information( this,
                   QString::fromLocal8Bit("����"),
                   QString::fromLocal8Bit("9��8���� �ڷ��Դϴ�."),QMessageBox::Ok );
}

bool GoogleDialog::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::EnterWhatsThisMode) {
        _handleWhatsThisEntry(QApplication::activeWindow());
        return true;
    }
    else if ((object == ui->label) && (event->type() == QEvent::MouseButtonPress))
    {
        QDesktopServices::openUrl(QUrl("https://www.data.go.kr/data/15043131/fileData.do"));
        return true;
    }
    return QObject::eventFilter(object, event);
}
