#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QtDebug>
#include <QUrl>
#include <QByteArray>
#include <QVariant>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QPainter>

#define SPAN_INDEX 3 // 温度曲线间隔指数
#define ORIGIN_SIZE 4 // 温度曲线原点大小
#define TEMPERATURE_STARTING_COORDINATE 30 // 高温平均值起始坐标

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    while(QMessageBox::information(this,"版权","本程序由草泥马铁憨憨制作",QMessageBox::Yes | QMessageBox::No,QMessageBox::No) != QMessageBox::Yes);
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);//设置无边框
    SetExit();//设置退出按钮
    initLabel();//初始化预测天气标签


    // 请求天气API信息
       //url = URL1;
       city = CITYNAME;
       citytemp = city;
       manager = new QNetworkAccessManager(this);
       connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replayFinished(QNetworkReply*)));
       getWeatherInfo(manager);

       ui->curveLb->installEventFilter(this);
       //ui->cityLineEdit->installEventFilter(this);
//    https://api88.net/api/tianqi/?name=%E5%90%88%E8%82%A5&key=a339ad9ad4eb9f4d68cb7568165399e9
//    QByteArray a = QUrl::toPercentEncoding((QString)"马");
//    qDebug()<<a.data();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::ExitFun()
{
    qApp->exit(0);
}

void Dialog::contextMenuEvent(QContextMenuEvent * event)
{
    exitMenu->exec(QCursor::pos());
    event->accept();
}

void Dialog::mousePressEvent(QMouseEvent *event)//鼠标点击事件
{
    point = event->globalPos() - this->pos();
}

void Dialog::mouseMoveEvent(QMouseEvent *event)//鼠标移动事件
{
    move(event->globalPos() - point);
}

//初始化预测天气控件
void Dialog::initLabel()
{
    foreachDayLB<<ui->day_LB1<<ui->day_LB2<<ui->day_LB3<<ui->day_LB4<<ui->day_LB5;
    foreachDataLB<<ui->data_LB1<<ui->data_LB2<<ui->data_LB3<<ui->data_LB4<<ui->data_LB5;
    foreachGradeLB<<ui->grade_LB1<<ui->grade_LB2<<ui->grade_LB3<<ui->grade_LB4<<ui->grade_LB5;
    foreachWeatherLB<<ui->weather_LB1<<ui->weather_LB2<<ui->weather_LB3<<ui->weather_LB4<<ui->weather_LB5;
    foreachIconLB<<ui->icon_LB1<<ui->icon_LB2<<ui->icon_LB3<<ui->icon_LB4<<ui->icon_LB5;
    foreachDaytempLB<<ui->daytemp_LB1<<ui->daytemp_LB2<<ui->daytemp_LB3<<ui->daytemp_LB4<<ui->daytemp_LB5;
    //foreachPaintLB<<ui->paint_LB1<<ui->paint_LB2<<ui->paint_LB3<<ui->paint_LB4<<ui->paint_LB5;
    foreachnighttempLB<<ui->nighttemp_LB1<<ui->nighttemp_LB2<<ui->nighttemp_LB3<<ui->nighttemp_LB4<<ui->nighttemp_LB5;
}

void Dialog::SetExit()
{
    //设置退出按钮
    exitMenu = new QMenu(this);
    exitAct = new QAction;
    exitAct->setText(u8"退出");
    exitAct->setIcon(QIcon(":/icons/close.ico"));
    exitMenu->addAction(exitAct);
    connect(exitAct,SIGNAL(triggered(bool)),this,SLOT(ExitFun()));
}

void Dialog::replayFinished(QNetworkReply *reply)
{
    /* 获取响应的信息，状态码为200表示正常 --comment by wsg 2017/12/11 */
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if(reply->error() != QNetworkReply::NoError || status_code != 200)
    {
        QMessageBox::warning(this, "错误", "天气：请求数据错误，检查网络连接！", QMessageBox::Ok);
        return;
    }

    QByteArray bytes = reply->readAll();
    qDebug()<<bytes.toStdString().c_str();
//    QString result = QString::fromLocal8Bit(bytes);
    parseJson(bytes);
}

/* 请求数据 */
void Dialog::getWeatherInfo(QNetworkAccessManager *manager)
{
    QString str1(URL1 + city + "&key=" + KEY);
    QString str2(URL2 + city + "&key=" + KEY);
    QUrl jsonUrl(str1);
    manager->get( QNetworkRequest(jsonUrl));//此刻访问url
    QUrl jsonUrl1(str2);
    manager->get( QNetworkRequest(jsonUrl1));//此刻预测url
}




//解析Json数据*
void Dialog::parseJson(QByteArray bytes)
{
    QJsonParseError err;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes, &err); // 检测json格式
    if (err.error != QJsonParseError::NoError) // Json格式错误
    {
        return;
    }
    QJsonObject jsObj = jsonDoc.object();
    QString message = jsObj.value("msg").toString();
    if (message != "查询成功" && message != "请求成功")
    {
        city = citytemp;
        if(bl == 1)
            return;
        QMessageBox::information(this, tr("The information of Json_desc"),tr("天气：城市错误！"), QMessageBox::Ok );
        return;
    }
    //QString dateStr = jsObj.value("date").toString();
    //today.date = QDate::fromString(dateStr, "yyyyMMdd").toString("yyyy-MM-dd");

    //today.city = jsObj.value("city").toString();
    //today.date = jsObj.value("update").toString();
    // 解析data
    if(message == "请求成功"){//预测
        today = jsObj;
        bl++;
    }
    //QJsonObject dataObj =
//jsObj.value("data").toObject().value("weather").toObject().value("result").toObject().value("forecasts").toArray()
    //today.shidu = dataObj.value("shidu").toString();
    //today.pm25 = QString::number( dataObj.value("pm25").toDouble() );
    //today.quality = dataObj.value("quality").toString();
    //today.wendu = dataObj.value("wendu").toString() + "°";
    //today.ganmao = dataObj.value("ganmao").toString();

    // 解析data中的yesterday
    //QJsonObject yestObj = dataObj.value("yesterday").toObject();
    //forecast[0].date = yestObj.value("date").toString();
    //forecast[0].high = yestObj.value("high").toString();
    //forecast[0].low = yestObj.value("low").toString();
    //forecast[0].aqi = QString::number( yestObj.value("aqi").toDouble() );
    //forecast[0].type = yestObj.value("type").toString();

    // 解析data中的forecast
    if(message == "查询成功"){
        QJsonArray forecastArr = jsObj.value("data").toObject().value("weather").toObject().value("result").toObject().value("forecasts").toArray();
        for (int i = 0; i < 5; i++)
        {
           forecast[i] = forecastArr.at(i).toObject();
        }
        bl++;
    }

    // 取得今日数据
//    QJsonObject todayObj = forecastArr.at(0).toObject();
//    today.fx = todayObj.value("fx").toString();
//    today.fl = todayObj.value("fl").toString();
//    today.type = todayObj.value("type").toString();
//    today.sunrise = todayObj.value("sunrise").toString();
//    today.sunset = todayObj.value("sunset").toString();
//    today.notice = todayObj.value("notice").toString();
    if(bl == 2){
        bl = 0;
        citytemp = city;
        //paintCurve();
        setLabelContent();
    }
    //setLabelContent();
}

void Dialog::setLabelContent()
{
    paintCurve();
    ui->curveLb->update();
    //布局实时
    ui->real_temp_LB->setText(today.wendu + "°");
    ui->date_label->setText(today.date);
    ui->cityname_LB->setText(today.city);
    ui->update_time_LB->setText("实时数据更新时间:" + today.updata);
    ui->real_prompt_LB->setText("天气提示：" + today.infor);
    ui->text_label->setText(today.notice);//注意信息
    ui->tempLB->setText(today.shidu + "%");
    ui->pmLB->setText(today.pm25);
    ui->FengXiang_LB->setText(today.fx);
    ui->FengLi_LB->setText(today.fs);
    ui->AQI_LB->setText(today.quallity);
    ui->real_weather_label->setText(today.temp);
    //ui->real_icon_label->setStyleSheet("border-image: url(:/yahoo/duoyun.ico);")

    if(today.temp.indexOf("多云") != -1){
                ui->real_icon_label->setStyleSheet("border-image: url(:/yahoo/duoyun.ico);");
     }else if(today.temp.indexOf("雷") != -1){
                ui->real_icon_label->setStyleSheet("border-image: url(:/yahoo/lei.ico);");
     }else if(today.temp.indexOf("晴") != -1){
                ui->real_icon_label->setStyleSheet("border-image: url(:/yahoo/qing.ico);");
     }else if(today.temp.indexOf("沙尘") != -1){
               ui->real_icon_label->setStyleSheet("border-image: url(:/yahoo/shachen.ico);");
            }else if(today.temp.indexOf("雾") != -1){
               ui->real_icon_label->setStyleSheet("border-image: url(:/yahoo/wu.ico);");
            }else if(today.temp.indexOf("雪") != -1){
               ui->real_icon_label->setStyleSheet("border-image: url(:/yahoo/xue.ico);");
            }else if(today.temp.indexOf("阴") != -1){
               ui->real_icon_label->setStyleSheet("border-image: url(:/yahoo/yin.ico);");
            }else if(today.temp.indexOf("雨") != -1){
               ui->real_icon_label->setStyleSheet("border-image: url(:/yahoo/yu.ico);");
            }else  ui->real_icon_label->setStyleSheet("border-image: url(:/weatherIcons/day/undefined.png);");

    if(today.quallity.toInt() < 50){
         ui->label_51->setStyleSheet("color: rgb(0, 255, 0); background-color: argb(60, 60, 60, 0);");
         ui->label_51->setText(u8"优质");
    }else if(today.quallity.toInt() < 100){
        ui->label_51->setStyleSheet("color: rgb(255, 255, 0); background-color: argb(60, 60, 60, 0);");
        ui->label_51->setText(u8"良好");
    }else if(today.quallity.toInt() < 150){
        ui->label_51->setStyleSheet("color: rgb(255, 128, 128); background-color: argb(60, 60, 60, 0);");
        ui->label_51->setText(u8"轻度污染");
    }else{
        ui->label_51->setStyleSheet("color: rgb(255, 0, 0); background-color: argb(60, 60, 60, 0);");
        ui->label_51->setText(u8"重度污染");
    }


    ui->text_label->setText(today.ganmao);
    ui->YeWan_LB->setText(forecast[0].night);
    //预测布局
    for(int i = 0;i < 5;i++){
        if(i == 0)
            foreachDayLB.at(i)->setText(u8"今天");
        else if(i == 1)
            foreachDayLB.at(i)->setText(u8"明天");
        else
        foreachDayLB.at(i)->setText(forecast[i].week);//星期
        foreachDataLB.at(i)->setText(forecast[i].date.split("-").at(2));//号
        if(forecast[i].AQI.toInt() < 50){
             foreachGradeLB[i]->setStyleSheet("color: rgb(0, 255, 0); background-color: argb(60, 60, 60, 0);");
             foreachGradeLB[i]->setText(u8"优质");
        }else if(forecast[i].AQI.toInt() < 100){
            foreachGradeLB[i]->setStyleSheet("color: rgb(255, 255, 0); background-color: argb(60, 60, 60, 0);");
            foreachGradeLB[i]->setText(u8"良好");
        }else if(forecast[i].AQI.toInt() < 150){
            foreachGradeLB[i]->setStyleSheet("color: rgb(255, 128, 128); background-color: argb(60, 60, 60, 0);");
            foreachGradeLB[i]->setText(u8"轻度污染");
        }else{
            foreachGradeLB[i]->setStyleSheet("color: rgb(255, 0, 0); background-color: argb(60, 60, 60, 0);");
            foreachGradeLB[i]->setText(u8"重度污染");
        }
        foreachWeatherLB.at(i)->setText(forecast[i].weather);
        if(forecast[i].weather.indexOf("多云") != -1){
            foreachIconLB[i]->setStyleSheet("border-image: url(:/yahoo/duoyun.ico);");
        }else if(forecast[i].weather.indexOf("雷") != -1){
            foreachIconLB[i]->setStyleSheet("border-image: url(:/yahoo/lei.ico);");
        }else if(forecast[i].weather.indexOf("晴") != -1){
            foreachIconLB[i]->setStyleSheet("border-image: url(:/yahoo/qing.ico);");
        }else if(forecast[i].weather.indexOf("沙尘") != -1){
            foreachIconLB[i]->setStyleSheet("border-image: url(:/yahoo/shachen.ico);");
        }else if(forecast[i].weather.indexOf("雾") != -1){
            foreachIconLB[i]->setStyleSheet("border-image: url(:/yahoo/wu.ico);");
        }else if(forecast[i].weather.indexOf("雪") != -1){
            foreachIconLB[i]->setStyleSheet("border-image: url(:/yahoo/xue.ico);");
        }else if(forecast[i].weather.indexOf("阴") != -1){
            foreachIconLB[i]->setStyleSheet("border-image: url(:/yahoo/yin.ico);");
        }else if(forecast[i].weather.indexOf("雨") != -1){
            foreachIconLB[i]->setStyleSheet("border-image: url(:/yahoo/yu.ico);");
        }else foreachIconLB[i]->setStyleSheet("border-image: url(:/weatherIcons/day/undefined.png);");
        foreachDaytempLB.at(i)->setText(forecast[i].high + "℃");
        //foreachPaintLB绘图
        foreachnighttempLB.at(i)->setText(forecast[i].low + "℃");
    }
}



void Dialog::on_update_toolButton_clicked()
{
    getWeatherInfo(manager);
}

void Dialog::on_search_toolButton_clicked()
{
    QString str = ui->citylineEdit->text();
    ui->citylineEdit->setText("");
    str = QUrl::toPercentEncoding(str);
    qDebug()<<str.data();
    city = str;
    getWeatherInfo(manager);
}

/* 温度曲线绘制 */
void Dialog::paintCurve()
{
    QPainter painter(ui->curveLb);
    painter.setRenderHint(QPainter::Antialiasing, true); // 反锯齿

    // 将温度转换为int类型，并计算平均值，平均值作为curveLb曲线的参考值，参考Y坐标为45
    int tempTotal = 0;
    int high[5] = {};
    int low[5] = {};

    //int h, l;
    for (int i = 0; i < 5; i++)
    {
        //h = forecast[i].high.split(" ").at(1);
        //h = h.left(h.length() - 1);
        //h = forecast[i].high.toInt();
        high[i] = forecast[i].high.toInt();
        tempTotal += high[i];

        //l = forecast[i].low.split(" ").at(1);
        //l = l.left(h.length() - 1);
        //l = forecast[i].low.toInt();
        low[i] = forecast[i].low.toInt();
    }
    int tempAverage = (int)(tempTotal / 5); // 最高温平均值

    // 算出温度对应坐标
    int pointX[5] = {40, 120, 220, 290,380}; // 点的X坐标
    int pointHY[5] = {0};
    int pointLY[5] = {0};
    for (int i = 0; i < 5; i++)
    {
        pointHY[i] = TEMPERATURE_STARTING_COORDINATE - ((high[i] - tempAverage) * SPAN_INDEX);
        pointLY[i] = TEMPERATURE_STARTING_COORDINATE + ((tempAverage - low[i]) * SPAN_INDEX);
    }

    QPen pen = painter.pen();
    pen.setWidth(2);

    // 高温曲线绘制
    painter.save();
    pen.setColor(QColor(255, 170, 0));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.setBrush(QColor(255, 170, 0));
    painter.drawEllipse(QPoint(pointX[0], pointHY[0]), ORIGIN_SIZE, ORIGIN_SIZE);
    painter.drawEllipse(QPoint(pointX[1], pointHY[1]), ORIGIN_SIZE, ORIGIN_SIZE);
    painter.drawLine(pointX[0], pointHY[0], pointX[1], pointHY[1]);

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    painter.setPen(pen);

    for (int i = 1; i < 4; i++)
    {
        painter.drawEllipse(QPoint(pointX[i+1], pointHY[i+1]), ORIGIN_SIZE, ORIGIN_SIZE);
        painter.drawLine(pointX[i], pointHY[i], pointX[i+1], pointHY[i+1]);
    }
    painter.restore();

    // 低温曲线绘制
    pen.setColor(QColor(0, 255, 255));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.setBrush(QColor(0, 255, 255));
    painter.drawEllipse(QPoint(pointX[0], pointLY[0]), ORIGIN_SIZE, ORIGIN_SIZE);
    painter.drawEllipse(QPoint(pointX[1], pointLY[1]), ORIGIN_SIZE, ORIGIN_SIZE);
    painter.drawLine(pointX[0], pointLY[0], pointX[1], pointLY[1]);

    pen.setColor(QColor(0, 255, 255));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    for (int i = 1; i < 4; i++)
    {
        painter.drawEllipse(QPoint(pointX[i+1], pointLY[i+1]), ORIGIN_SIZE, ORIGIN_SIZE);
        painter.drawLine(pointX[i], pointLY[i], pointX[i+1], pointLY[i+1]);
    }
}
bool Dialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->curveLb && event->type() == QEvent::Paint)
    {
        paintCurve();
    }
    return QWidget::eventFilter(watched,event);
}
