#include "chartWidget.h"
#include <QVBoxLayout>
#include <QtMath>

ChartWidget::ChartWidget(int channelId, QWidget *parent)
    : QWidget(parent)
    , m_channelId(channelId)
    , m_pointCount(100)
    , m_maxAmplitude(0)
    , m_minAmplitude(0)
    , m_rmsAmplitude(0)
    , m_dataUpdated(false)
{
    setupUI();
}

void ChartWidget::updateData(const QVector<QPointF> &data)
{
    if (!data.isEmpty()) {
        m_series->replace(data);
        calculateAmplitudeStats(data);
        m_dataUpdated = true;
    }
}
ChartWidget::~ChartWidget()
{
    // Qt图表对象会自动被父对象管理，不需要手动删除
}

void ChartWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(2);

    // 幅值显示标签
    m_amplitudeLabel = new QLabel(this);
    m_amplitudeLabel->setStyleSheet("font-weight: bold; color: darkgreen;");
    m_amplitudeLabel->setAlignment(Qt::AlignCenter);
    m_amplitudeLabel->setMaximumHeight(20);
    layout->addWidget(m_amplitudeLabel);

    // 创建图表
    m_chart = new QChart();
    m_chart->setTitle("");
    m_chart->legend()->hide();
    m_chart->setMargins(QMargins(0, 0, 0, 0));
    m_chart->setBackgroundRoundness(0);

    // 创建坐标轴
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_axisX->setRange(0, 20.0);
    m_axisY->setRange(-2.5, 2.5);

    // 优化性能：隐藏坐标轴细节
    m_axisX->setLabelsVisible(false);
    m_axisY->setLabelsVisible(false);
    m_axisX->setGridLineVisible(false);
    m_axisY->setGridLineVisible(false);
    m_axisX->setLineVisible(false);
    m_axisY->setLineVisible(false);

    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    // 创建数据系列
    m_series = new QLineSeries();
    m_series->setUseOpenGL(true); // 启用OpenGL加速
    setLineColor(QColor::fromHsv((m_channelId * 30) % 360, 200, 200));
    setLineWidth(1.2);

    m_chart->addSeries(m_series);
    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);

    // 创建图表视图
    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing, false);
    m_chartView->setMinimumSize(200, 150);

    layout->addWidget(m_chartView);

    updateTitle();
}


void ChartWidget::calculateAmplitudeStats(const QVector<QPointF> &data)
{
    if (data.isEmpty()) {
        m_maxAmplitude = m_minAmplitude = m_rmsAmplitude = 0;
        return;
    }

    m_maxAmplitude = -1e9;
    m_minAmplitude = 1e9;
    double sumSquares = 0.0;

    for (const QPointF &point : data) {
        double y = point.y();
        m_maxAmplitude = qMax(m_maxAmplitude, y);
        m_minAmplitude = qMin(m_minAmplitude, y);
        sumSquares += y * y;
    }

    m_rmsAmplitude = qSqrt(sumSquares / data.size());
}

void ChartWidget::updateAmplitudeDisplay()
{
    double peakToPeak = m_maxAmplitude - m_minAmplitude;
    m_amplitudeLabel->setText(
        QString("通道 %1: 峰值±%2V RMS:%3V P-P:%4V")
            .arg(m_channelId + 1)
            .arg(qMax(qAbs(m_maxAmplitude), qAbs(m_minAmplitude)), 0, 'f', 3)
            .arg(m_rmsAmplitude, 0, 'f', 3)
            .arg(peakToPeak, 0, 'f', 3)
    );
}

void ChartWidget::setPointCount(int count)
{
    m_pointCount = count;
}

void ChartWidget::setYRange(double min, double max)
{
    m_axisY->setRange(min, max);
}

void ChartWidget::setLineColor(const QColor &color)
{
    m_series->setPen(QPen(color, m_series->pen().widthF()));
}

void ChartWidget::setLineWidth(qreal width)
{
    QPen pen = m_series->pen();
    pen.setWidthF(width);
    m_series->setPen(pen);
}

void ChartWidget::updateTitle()
{
    // 可以在这里更新图表标题，当前为空
}
