#include "parser.h"

#include <QUrl>
#include <QDebug>

Parser::Parser(IRouter* router)
{
    _id = 8001;
    pManager = new QNetworkAccessManager();

    latitude1 = 0;
    latitude2 = 0;
    longinude1 = 0;
    longinude2 = 0;

    _router = router;
    _subscriber = _router->get_subscriber();

    connect(pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(sGetQuery(QNetworkReply*)));
}

void Parser::ParserWork() {
    QString siteUrl = "http://bma.fr24.com/zones/";
    siteUrl.append(SelectZone());
    pManager->get(QNetworkRequest(QUrl(siteUrl)));
}

void Parser::sGetQuery(QNetworkReply *reply) {
    QString answer = QString::fromUtf8(reply->readAll());
    if (answer.size() == 0) {
        emit ErrorParser();
    } else {
        FilterData(answer);
    }
}

void Parser::FilterData(QString data) {
    int c = 0;
    bool inList = false;
    QMap<int, QVector<QString> > map1;
    //! получение координат кавычек и запятых и занесение их в список listBr
    for (int i = 0; i < data.size(); i++) {
        Bracketed br;
        if (data.mid(i, 1) == "[") {
            br.br1 = i;
            inList = true;
        }
        if ((data.mid(i, 1) == ",") && (inList == true)) {
            br.comma[c] = i;
            c++;
        }
        if (data.mid(i, 1) == "]") {
            br.br2 = i;
            listBr << br;
            c = 0;
            inList = false;
        }
    }

    //! парсинг данных
    for (int i = 0; i < listBr.size(); i++) {
        int x1;
        int x2;
        QString latitude;
        QString longinude;
        QString height;
        QString speed;
        QString course;
        QString namePlane;

        //! получение широты расположения самолета
        x1 = listBr[i].comma[0] + 1;
        x2 = listBr[i].comma[1];
        latitude = data.mid(x1, x2 - x1);

        if (latitude1 >= latitude.toDouble() && latitude.toDouble() >= latitude2)
        {
            //! получение долготы расположения самолета
            x1 = listBr[i].comma[1] + 1;
            x2 = listBr[i].comma[2];
            longinude = data.mid(x1, x2 - x1);

            if (longinude1 <= longinude.toDouble() && longinude.toDouble() <= longinude2) {
                //! получение курса самолета
                x1 = listBr[i].comma[2] + 1;
                x2 = listBr[i].comma[3];
                course = data.mid(x1, x2 - x1);

                //! получение высоты расположения самолета
                x1 = listBr[i].comma[3] + 1;
                x2 = listBr[i].comma[4];
                height = data.mid(x1, x2 - x1);
                double h = height.toDouble() * 0.3048;
                height = QString().number(h);

                //! получение скорости самолета
                x1 = listBr[i].comma[4] + 1;
                x2 = listBr[i].comma[5];
                speed = data.mid(x1, x2 - x1);

                //! получение названия самолета
                x1 = listBr[i].comma[15] + 2;
                x2 = listBr[i].comma[16] - 1;
                namePlane = data.mid(x1, x2 - x1);

                //! внесение полученных данных в список listDataFly
                DataFly data;
                data.namePlane = namePlane;
                data.latitute = latitude;
                data.longinude = longinude;
                data.speed = speed;
                data.height = height;
                data.course = course;
                QVector<QString> vec_str;
                vec_str.push_back(namePlane);
                vec_str.push_back(longinude);
                vec_str.push_back(latitude);
                vec_str.push_back(speed);
                vec_str.push_back(height);
                vec_str.push_back(course);


                map1.insert(i, vec_str);



                listDataFly << data;
            }
        }
    }
    listBr.clear();

    QByteArray *ba = new QByteArray();
//    ba->append(ba1);
    QDataStream ds(ba, QIODevice::WriteOnly);
    ds << _id;
    ds << map1;


	QSharedPointer<IMessageOld> msg(new MessageOld(_id, AIS_DATA, ba));
    _subscriber->data_ready(AIS_DATA, msg);

    emit NewListParser(listDataFly);
    listDataFly.clear();
}

void Parser::NewSquere(double lat1, double lon1, double lat2, double lon2) {
    latitude1 = lat1;
    latitude2 = lat2;
    longinude1 = lon1;
    longinude2 = lon2;
}

QString Parser::SelectZone() {
    QString zone = "full_all.js";
    if (72.57 > latitude1 && latitude2 > 33.57) {
        if (-16.96 < longinude1 && longinude2 < 55.05) {
            zone = "europe_all.js";
        }
    }
    if (75.0 > latitude1 && latitude2 > 3.0) {
        if (-180.0 < longinude1 && longinude2 < -52.0) {
            zone = "northamerica_all.js";
        }
    }
    if (16.0 > latitude1 && latitude2 > -57.0) {
        if (-96.0 < longinude1 && longinude2 < -31.0) {
            zone = "southamerica_all.js";
        }
    }
    if (19.62 > latitude1 && latitude2 > -55.08) {
        if (88.40 < longinude1 && longinude2 < 180.0) {
            zone = "oceania_all.js";
        }
    }
    if (79.98 > latitude1 && latitude2 > 12.48) {
        if (40.91 < longinude1 && longinude2 < 179.77) {
            zone = "asia_all.js";
        }
    }
    if (39.0 > latitude1 && latitude2 > -39.0) {
        if (-29.0 < longinude1 && longinude2 < 55.0) {
            zone = "africa_all.js";
        }
    }
    if (52.69 > latitude1 && latitude2 > 15.62) {
        if (-50.90 < longinude1 && longinude2 < -4.75) {
            zone = "atlantic_all.js";
        }
    }
    if (10.72 > latitude1 && latitude2 > -6.08) {
        if (63.10 < longinude1 && longinude2 < 86.53) {
            zone = "maldives_all.js";
        }
    }
    if (82.62 > latitude1 && latitude2 > 59.02) {
        if (-84.53 < longinude1 && longinude2 < 4.45) {
            zone = "northatlantic_all.js";
        }
    }

    return zone;
}
