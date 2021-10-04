#ifndef DIALOG_H
#define DIALOG_H

#define CITYNAME "%E8%A5%BF%E5%AE%89"
#define URL1 "https://api88.net/api/tianqi/?name="
#define URL2 "https://api88.net/api/weather/?name="
#define KEY "a339ad9ad4eb9f4d68cb7568165399e9"

#include <QDialog>
#include <QWidget>
#include "weatherdata.h"
#include <QList>
#include <QLabel>
#include <QPoint>
#include <QRect>
#include <QTimer>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QNetworkAccessManager>
#include <QByteArray>



QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
private slots:
    void ExitFun();//退出槽函数
    void replayFinished(QNetworkReply*);
    void on_update_toolButton_clicked();

    void on_search_toolButton_clicked();

private:
    Ui::Dialog *ui;
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void initLabel();//初始化标签
    void SetExit();//设置退出按钮
    void getWeatherInfo(QNetworkAccessManager *manager);
    void parseJson(QByteArray obj);
    void setLabelContent();//布局
    void paintCurve();
    bool eventFilter(QObject *watched, QEvent *event);
    QMenu* exitMenu;//退出菜单
    QAction *exitAct;//退出工具
    QPoint point;
    //ui
    QList<QLabel*>foreachDayLB;
    QList<QLabel*>foreachDataLB;
    QList<QLabel*>foreachGradeLB;
    QList<QLabel*>foreachWeatherLB;
    QList<QLabel*>foreachIconLB;
    QList<QLabel*>foreachDaytempLB;
    //QList<QLabel*>foreachPaintLB;
    QList<QLabel*>foreachnighttempLB;
    //网络请求
    //QString url;//网址链接
    QString city;//城市名
    QString citytemp;//上一次的城市名


    weatherData today;
    Forscase forecast[6];
    int bl = 0;
    QNetworkAccessManager* manager;
protected:

};
#endif // DIALOG_H





