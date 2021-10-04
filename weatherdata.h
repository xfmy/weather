#include <QObject>
#include <QWidget>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

class weatherData//实时数据
{
public:
    weatherData();
    ~weatherData();
    weatherData& operator=(const QJsonObject&);
    QString date;//日期
    QString updata;//数据更新时间
    QString wendu;//温度
    QString city;//城市
    QString shidu;//湿度
    QString pm25;//pm2.5
    QString quallity;//空气质量指数
    QString ganmao;//感冒指数
    QString fx;//风向
    QString fs;//风速
    QString infor;//舒适度
    QString notice;//注意信息
    QString temp;//天气
    class name
    {
    public:
        name() {}
    };
};


class Forscase{
public:
    Forscase();
    ~Forscase();
    Forscase& operator=(const QJsonObject& obj);
    QString date;//日期
    QString weather;//天气
    QString high;//最高温
    QString low;//最低温
    //QString fs;//风速
    //QString fx;//风向
    QString AQI;
    QString week;//星期
    QString night;//夜晚天气
};
/*
#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#endif // WEATHERDATA_H
*/
