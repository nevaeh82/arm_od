#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QList>
#include <QVector>

#include "../Common/IMessage.h"
#include "../Common/IRouter.h"
#include "../Common/ISubscriber.h"
#include "../RPC/Message.h"

/*!
        DataFly содержит характеристики самолета
 */
struct DataFly {
    QString height;
    QString latitute;
    QString longinude;
    QString speed;
    QString namePlane;
    QString course;
};

/*!
        Parser класс для парсинга данных
 */

class Parser : public QObject {
    Q_OBJECT
public:
    Parser(IRouter* router);
    /*!
            Задание координат квадрата
     */
    void NewSquere(double lat1, double lon1, double lat2, double lon2);
    /*!
            Отправка данных на сервер
     */
    void ParserWork();

private:
    int             _id;
    IRouter*        _router;
    ISubscriber*    _subscriber;

private:
    QNetworkAccessManager *pManager;

    //! Координаты квадрата
    double latitude1;
    double latitude2;
    double longinude1;
    double longinude2;

    //! Координаты кавычек и запятых в полученных данных
    struct Bracketed {
        int br1;
        int br2;
        int comma[17];
    };

    QList<Bracketed> listBr;
    QVector<DataFly> listDataFly;

    /*!
            Фильтрация полученных данных
     */
    void FilterData(QString data);

    /*!
            Определение оптимальной географической зоны
     */
    QString SelectZone();

private slots:

    /*!
            Получение данных от сервера
    */
    void sGetQuery(QNetworkReply *reply);

signals:

    /*!
            Сигнал посылает данные после завершения их обработки
     */
    void NewListParser(QVector<DataFly>);

    /*!
            Сигнал который посылается системой при ошибке
            \получения данных с сервера
    */
    void ErrorParser();
};

#endif
