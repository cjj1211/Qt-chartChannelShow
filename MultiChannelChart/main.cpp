#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QTimer>
#include <QElapsedTimer>
#include <QtMath>
#include <QRandomGenerator>

#include "chartManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle(QString::fromLocal8Bit("多通道正弦波实时显示系统 - 等分布局"));
    mainWindow.resize(1200, 800);

    // 中央部件
    QWidget *centralWidget = new QWidget();
    mainWindow.setCentralWidget(centralWidget);

    // 创建控制面板
    QWidget *controlPanel = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlPanel);

    // 通道数选择
    QLabel *channelLabel = new QLabel("显示通道:");
    QComboBox *channelCombo = new QComboBox();
    channelCombo->addItems({"4通道", "8通道", "16通道", "32通道", "64通道"});
    channelCombo->setCurrentIndex(2); // 默认16通道

    // 点数选择
    QLabel *pointLabel = new QLabel("显示点数:");
    QComboBox *pointCombo = new QComboBox();
    pointCombo->addItems({"100", "200", "500"});
    pointCombo->setCurrentIndex(1); // 默认200点

    // 统计信息
    QLabel *fpsLabel = new QLabel("FPS: 0.0");
    fpsLabel->setStyleSheet("font-weight: bold; color: blue;");

    QLabel *visibleLabel = new QLabel("可见: 0/0");
    visibleLabel->setStyleSheet("color: green;");

    QLabel *chartSizeLabel = new QLabel("图表高度: 计算中...");
    chartSizeLabel->setStyleSheet("color: darkorange;");

    controlLayout->addWidget(channelLabel);
    controlLayout->addWidget(channelCombo);
    controlLayout->addWidget(pointLabel);
    controlLayout->addWidget(pointCombo);
    controlLayout->addWidget(fpsLabel);
    controlLayout->addWidget(visibleLabel);
    controlLayout->addWidget(chartSizeLabel);
    controlLayout->addStretch();

    controlPanel->setMaximumHeight(60);

    // 创建图表区域
    QScrollArea *scrollArea = new QScrollArea();

    // 创建图表管理器
    ChartManager *chartManager = new ChartManager(scrollArea);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(controlPanel);
    mainLayout->addWidget(scrollArea);

    // 当前设置
    int currentChannels = 16;
    int currentPoints = 200;

    // 数据生成和更新定时器
    QTimer *dataTimer = new QTimer();
    QElapsedTimer fpsTimer;
    int frameCount = 0;

    // 正弦波参数
    QVector<double> channelFrequencies;
    QVector<double> channelPhases;
    QVector<double> channelAmplitudes;

    // 更新通道显示函数
    auto updateChannels = [&]() {
        chartManager->setupChannels(currentChannels);

        // 初始化通道参数
        channelFrequencies.resize(currentChannels);
        channelPhases.resize(currentChannels);
        channelAmplitudes.resize(currentChannels);

        for (int i = 0; i < currentChannels; i++) {
            channelFrequencies[i] = 1.0 + (i * 0.3);
            channelPhases[i] = (i * M_PI) / currentChannels;
            channelAmplitudes[i] = 0.8 + (i * 0.02);
        }

        // 更新图表大小显示
        if (chartManager->getChartWidget(0)) {
            chartSizeLabel->setText(QString("图表高度: %1px").arg(chartManager->getChartWidget(0)->height()));
        }
    };

    // 数据生成函数
    auto generateChannelData = [&](int channelId) -> QVector<QPointF> {
        QVector<QPointF> data;
        data.reserve(currentPoints);

        double frequency = channelFrequencies[channelId];
        double phase = channelPhases[channelId];
        double amplitude = channelAmplitudes[channelId];

        static double timeOffset = 0.0;
        timeOffset += 0.05;

        for (int j = 0; j < currentPoints; j++) {
            double x = 20.0 * (double(j) / currentPoints);
            double y = amplitude * qSin(2 * M_PI * frequency * (x / 20.0) + phase + timeOffset)
                     + 0.05 * QRandomGenerator::global()->generateDouble();
            data.append(QPointF(x, y));
        }

        return data;
    };

    // 定时器槽函数 - 只更新可见通道数据
    QObject::connect(dataTimer, &QTimer::timeout, [&]() {
        chartManager->updateVisibleChannels();

        QSet<int> visibleChannels = chartManager->getVisibleChannels();
        for (int channelId : visibleChannels) {
            QVector<QPointF> data = generateChannelData(channelId);
            chartManager->updateChannelData(channelId, data);
        }

        visibleLabel->setText(QString("可见: %1/%2").arg(visibleChannels.size()).arg(currentChannels));

        frameCount++;
        if (frameCount >= 10) {
            qreal fps = 10000.0 / fpsTimer.restart();
            fpsLabel->setText(QString("FPS: %1").arg(fps, 0, 'f', 1));
            frameCount = 0;
        }
    });

    // 幅值统计定时器
    QTimer *statsTimer = new QTimer();
    QObject::connect(statsTimer, &QTimer::timeout, [&]() {
        chartManager->updateAllAmplitudeDisplays();
    });

    // 连接控制信号
    QObject::connect(channelCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [&](int index) {
            static const int channelCounts[] = {4, 8, 16, 32, 64};
            currentChannels = channelCounts[index];
            updateChannels();
        });

    QObject::connect(pointCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [&](int index) {
            static const int pointCounts[] = {100, 200, 500};
            currentPoints = pointCounts[index];
        });

    // 初始显示
    updateChannels();

    // 启动定时器
    fpsTimer.start();
    dataTimer->start(33); // ~30 FPS
    statsTimer->start(1000); // 1秒更新一次统计

    mainWindow.show();

    return a.exec();
}
