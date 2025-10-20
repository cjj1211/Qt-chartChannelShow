///****************************************************************************
//**
//** Copyright (C) 2016 The Qt Company Ltd.
//** Contact: https://www.qt.io/licensing/
//**
//** This file is part of the Qt Charts module of the Qt Toolkit.
//**
//** $QT_BEGIN_LICENSE:GPL$
//** Commercial License Usage
//** Licensees holding valid commercial Qt licenses may use this file in
//** accordance with the commercial license agreement provided with the
//** Software or, alternatively, in accordance with the terms contained in
//** a written agreement between you and The Qt Company. For licensing terms
//** and conditions see https://www.qt.io/terms-conditions. For further
//** information use the contact form at https://www.qt.io/contact-us.
//**
//** GNU General Public License Usage
//** Alternatively, this file may be used under the terms of the GNU
//** General Public License version 3 or (at your option) any later version
//** approved by the KDE Free Qt Foundation. The licenses are as published by
//** the Free Software Foundation and appearing in the file LICENSE.GPL3
//** included in the packaging of this file. Please review the following
//** information to ensure the GNU General Public License requirements will
//** be met: https://www.gnu.org/licenses/gpl-3.0.html.
//**
//** $QT_END_LICENSE$
//**
//****************************************************************************/

//#include "datasource.h"
//#include <QtWidgets/QApplication>
//#include <QtWidgets/QMainWindow>
//#include <QtCharts/QChartView>
//#include <QtCharts/QLineSeries>
//#include <QtCharts/QScatterSeries>
//#include <QtCharts/QValueAxis>
//#include <QtCharts/QLogValueAxis>
//#include <QtWidgets/QLabel>

//// Uncomment to use logarithmic axes instead of regular value axes
////#define USE_LOG_AXIS

//// Uncomment to use regular series instead of OpenGL accelerated series
////#define DONT_USE_GL_SERIES

//// Uncomment to add a simple regular series (thick line) and a matching OpenGL series (thinner line)
//// to verify the series have same visible geometry.
//#define ADD_SIMPLE_SERIES

//QT_CHARTS_USE_NAMESPACE

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    QStringList colors;
//    colors << "red" << "blue" << "green" << "black";

//    QChart *chart = new QChart();
//    chart->legend()->hide();

//#ifdef USE_LOG_AXIS
//    QLogValueAxis *axisX = new QLogValueAxis;
//    QLogValueAxis *axisY = new QLogValueAxis;
//#else
//    QValueAxis *axisX = new QValueAxis;
//    QValueAxis *axisY = new QValueAxis;
//#endif

//    chart->addAxis(axisX, Qt::AlignBottom);
//    chart->addAxis(axisY, Qt::AlignLeft);

//    const int seriesCount = 30 ;
//#ifdef DONT_USE_GL_SERIES
//    const int pointCount = 100;
//    chart->setTitle("Unaccelerated Series");
//#else
//    const int pointCount = 10000;
//    chart->setTitle("OpenGL Accelerated Series");
//#endif

//    QList<QXYSeries *> seriesList;
//    for (int i = 0; i < seriesCount; i++) {
//        QXYSeries *series = 0;
//        int colorIndex = i % colors.size();
//        if (i % 2) {
//            series = new QScatterSeries;
//            QScatterSeries *scatter = static_cast<QScatterSeries *>(series);
//            scatter->setColor(QColor(colors.at(colorIndex)));
//            scatter->setMarkerSize(qreal(colorIndex + 2) / 2.0);
//            // Scatter pen doesn't have affect in OpenGL drawing, but if you disable OpenGL drawing
//            // this makes the marker border visible and gives comparable marker size to OpenGL
//            // scatter points.
//            scatter->setPen(QPen("black"));
//        } else {
//            series = new QLineSeries;
//            series->setPen(QPen(QBrush(QColor(colors.at(colorIndex))),
//                                qreal(colorIndex + 2) / 2.0));
//        }
//        seriesList.append(series);
//#ifdef DONT_USE_GL_SERIES
//        series->setUseOpenGL(false);
//#else
//        //![1]
//        series->setUseOpenGL(true);
//        //![1]
//#endif
//        chart->addSeries(series);
//        series->attachAxis(axisX);
//        series->attachAxis(axisY);
//    }

//    if (axisX->type() == QAbstractAxis::AxisTypeLogValue)
//        axisX->setRange(0.1, 20.0);
//    else
//        axisX->setRange(0, 20.0);

//    if (axisY->type() == QAbstractAxis::AxisTypeLogValue)
//        axisY->setRange(0.1, 10.0);
//    else
//        axisY->setRange(0, 10.0);

//#ifdef ADD_SIMPLE_SERIES
//    QLineSeries *simpleRasterSeries = new QLineSeries;
//    *simpleRasterSeries << QPointF(0.001, 0.001)
//                 << QPointF(2.5, 8.0)
//                 << QPointF(5.0, 4.0)
//                 << QPointF(7.5, 9.0)
//                 << QPointF(10.0, 0.001)
//                 << QPointF(12.5, 2.0)
//                 << QPointF(15.0, 1.0)
//                 << QPointF(17.5, 6.0)
//                 << QPointF(20.0, 10.0);
//    simpleRasterSeries->setUseOpenGL(false);
//    simpleRasterSeries->setPen(QPen(QBrush("magenta"), 8));
//    chart->addSeries(simpleRasterSeries);
//    simpleRasterSeries->attachAxis(axisX);
//    simpleRasterSeries->attachAxis(axisY);

//    QLineSeries *simpleGLSeries = new QLineSeries;
//    simpleGLSeries->setUseOpenGL(true);
//    simpleGLSeries->setPen(QPen(QBrush("black"), 2));
//    simpleGLSeries->replace(simpleRasterSeries->points());
//    chart->addSeries(simpleGLSeries);
//    simpleGLSeries->attachAxis(axisX);
//    simpleGLSeries->attachAxis(axisY);
//#endif

//    QChartView *chartView = new QChartView(chart);

//    QMainWindow window;
//    window.setCentralWidget(chartView);
//    window.resize(600, 400);
//    window.show();

//    DataSource dataSource;
//    dataSource.generateData(seriesCount, 10, pointCount);

//    QLabel *fpsLabel = new QLabel(&window);
//    QLabel *countLabel = new QLabel(&window);
//    QString countText = QStringLiteral("Total point count: %1");
//    countLabel->setText(countText.arg(pointCount * seriesCount));
//    countLabel->adjustSize();
//    fpsLabel->move(10, 2);
//    fpsLabel->adjustSize();
//    fpsLabel->raise();
//    fpsLabel->show();
//    countLabel->move(10, fpsLabel->height());
//    fpsLabel->raise();
//    countLabel->show();

//    // We can get more than one changed event per frame, so do async update.
//    // This also allows the application to be responsive.
//    QObject::connect(chart->scene(), &QGraphicsScene::changed,
//                     &dataSource, &DataSource::handleSceneChanged);

//    dataSource.startUpdates(seriesList, fpsLabel);

//    return a.exec();
//}
#include "datasource.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const int channelCount = 64;  // 64通道
    const int pointCount = 1000;  // 每个通道1000个数据点
    const int rowsPerTab = 8;     // 每个标签页显示8个通道

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("64通道正弦波实时显示系统");

    QTabWidget *tabWidget = new QTabWidget(&mainWindow);
    mainWindow.setCentralWidget(tabWidget);

    // 使用一个数据源管理所有通道
    DataSource *dataSource = new DataSource();

    // 预先生成数据
    dataSource->generateData(channelCount, 10, pointCount);

    QList<QXYSeries*> allSeries;  // 存储所有系列

    // 计算需要多少个标签页
    int tabCount = (channelCount + rowsPerTab - 1) / rowsPerTab;

    for (int tabIndex = 0; tabIndex < tabCount; tabIndex++) {
        QWidget *tabWidgetContent = new QWidget();
        QVBoxLayout *tabLayout = new QVBoxLayout(tabWidgetContent);

        // 当前标签页的通道范围
        int startChannel = tabIndex * rowsPerTab;
        int endChannel = qMin(startChannel + rowsPerTab, channelCount);
        int channelsInThisTab = endChannel - startChannel;

        // 为当前标签页创建网格布局
        QGridLayout *gridLayout = new QGridLayout();

        for (int i = 0; i < channelsInThisTab; i++) {
            int channelIndex = startChannel + i;

            // 创建分组框
            QGroupBox *groupBox = new QGroupBox(QString("通道 %1").arg(channelIndex + 1));
            QVBoxLayout *groupLayout = new QVBoxLayout(groupBox);

            // 创建图表
            QChart *chart = new QChart();
            chart->setTitle("");
            chart->legend()->hide();
            chart->setMargins(QMargins(0, 0, 0, 0));

            // 创建坐标轴
            QValueAxis *axisX = new QValueAxis;
            QValueAxis *axisY = new QValueAxis;
            axisX->setRange(0, 20.0);
            axisY->setRange(-2, 2);

            // 隐藏坐标轴标签以节省空间
            axisX->setLabelsVisible(false);
            axisY->setLabelsVisible(false);
            axisX->setGridLineVisible(false);
            axisY->setGridLineVisible(false);

            chart->addAxis(axisX, Qt::AlignBottom);
            chart->addAxis(axisY, Qt::AlignLeft);

            // 创建正弦波系列
            QLineSeries *series = new QLineSeries();

            // 为不同通道设置不同颜色
            QColor color;
            int colorIndex = channelIndex % 12;
            switch (colorIndex) {
                case 0: color = Qt::red; break;
                case 1: color = Qt::blue; break;
                case 2: color = Qt::green; break;
                case 3: color = Qt::magenta; break;
                case 4: color = Qt::cyan; break;
                case 5: color = Qt::yellow; break;
                case 6: color = Qt::darkRed; break;
                case 7: color = Qt::darkBlue; break;
                case 8: color = Qt::darkGreen; break;
                case 9: color = Qt::darkMagenta; break;
                case 10: color = Qt::darkCyan; break;
                case 11: color = Qt::darkYellow; break;
            }

            series->setPen(QPen(color, 1.5));
            series->setUseOpenGL(true);

            chart->addSeries(series);
            series->attachAxis(axisX);
            series->attachAxis(axisY);

            // 创建图表视图
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            chartView->setMinimumSize(200, 150);

            groupLayout->addWidget(chartView);
            groupBox->setLayout(groupLayout);

            // 添加到网格布局
            int row = i / 2;
            int col = i % 2;
            gridLayout->addWidget(groupBox, row, col);

            // 保存系列引用
            allSeries.append(series);
        }

        tabLayout->addLayout(gridLayout);
        tabWidgetContent->setLayout(tabLayout);
        tabWidget->addTab(tabWidgetContent, QString("通道 %1-%2").arg(startChannel + 1).arg(endChannel));
    }

    // 添加控制面板
    QWidget *controlTab = new QWidget();
    QVBoxLayout *controlLayout = new QVBoxLayout(controlTab);

    QLabel *titleLabel = new QLabel("64通道正弦波实时显示系统");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QLabel *infoLabel = new QLabel(
        "系统信息:\n"
        "• 总通道数: 64\n"
        "• 采样点数: 1000\n"
        "• 显示方式: 8通道/页\n"
        "• 波形类型: 正弦波 + 随机噪声\n"
        "• 更新频率: 实时"
    );

    QLabel *fpsLabel = new QLabel("全局FPS: 0.0");
    fpsLabel->setAlignment(Qt::AlignCenter);

    controlLayout->addWidget(titleLabel);
    controlLayout->addWidget(infoLabel);
    controlLayout->addStretch();
    controlLayout->addWidget(fpsLabel);

    tabWidget->addTab(controlTab, "控制面板");

    mainWindow.resize(1200, 800);
    mainWindow.show();

    // 启动数据更新
    dataSource->startUpdates(allSeries, fpsLabel);

    // 连接场景变化信号 - 只连接第一个图表即可，因为数据源是共享的
    if (tabWidget->count() > 0) {
        QWidget *firstTab = tabWidget->widget(0);
        QList<QChartView*> chartViews = firstTab->findChildren<QChartView*>();
        if (!chartViews.isEmpty()) {
            QObject::connect(chartViews.first()->chart()->scene(), &QGraphicsScene::changed,
                            dataSource, &DataSource::handleSceneChanged);
        }
    }

    return a.exec();
}
