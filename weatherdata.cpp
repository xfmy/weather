#include "weatherdata.h"
#include <QDateTime>


weatherData::weatherData()
{
    date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").split(" ").at(0);
    updata = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").split(" ").at(1);
    wendu = u8"无数据";
    city = u8"无数据";
    shidu = u8"无数据";
    pm25 = u8"无数据";
    quallity = u8"无数据";
    ganmao = u8"无数据";
    fx = u8"无数据";
    fs = u8"无数据";
    infor = u8"无数据";
    notice = u8"无数据";
}
weatherData& weatherData::operator=(const QJsonObject& object)
{
    QJsonObject obj = object.value("data").toObject();
    date = obj.value("update").toString().split(" ").at(0);//2021-08-19
    updata = obj.value("update").toString().split(" ").at(1);//20:00:08
    wendu = obj.value("temperature").toString();
    city = obj.value("city").toString();
    shidu = obj.value("humidity").toString();
    pm25 = QString::number(obj.value("pm25").toInt());
    quallity = QString::number(obj.value("aqi").toInt());
    ganmao = obj.value("shushidu_info").toString();
    fx = obj.value("direct").toString();
    fs = obj.value("power").toString();
    infor = obj.value("shushidu").toString();
    notice = obj.value("shushidu_info").toString();
    temp = obj.value("weather").toString();
    return *this;
}

weatherData::~weatherData()
{
}


Forscase::Forscase()
{
    date = "无数据";
    weather = "无数据";
    high = "无数据";
    low = "无数据";
    //fs = "无数据";
    //fx = "无数据";
    week = "无数据";
    AQI = "无数据";
    night = "无数据";
}

Forscase::~Forscase()
{

}


Forscase &Forscase::operator=(const QJsonObject &obj)
{
    date = obj.value("date").toString();
    weather = obj.value("text_day").toString();
    high = QString::number(obj.value("high").toInt());
    low = QString::number(obj.value("low").toInt());
    //fs = obj.value("wc_day").toString();
    //fx = obj.value("wd_day").toString();
    week = obj.value("week").toString();
    night = obj.value("text_night").toString();
    AQI = QString::number(obj.value("aqi").toInt());
    return *this;
}
