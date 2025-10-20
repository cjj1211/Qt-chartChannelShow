#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QLabel>
#include <QVector>
#include <QPointF>

QT_CHARTS_USE_NAMESPACE

class ChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWidget(int channelId, QWidget *parent = nullptr);
    ~ChartWidget();

    void updateData(const QVector<QPointF> &data);
    void setChannelId(int id) { m_channelId = id; updateTitle(); }
    int channelId() const { return m_channelId; }

    // 幅值统计
    double maxAmplitude() const { return m_maxAmplitude; }
    double minAmplitude() const { return m_minAmplitude; }
    double rmsAmplitude() const { return m_rmsAmplitude; }
    void updateAmplitudeDisplay();

    // 配置接口
    void setPointCount(int count);
    void setYRange(double min, double max);
    void setLineColor(const QColor &color);
    void setLineWidth(qreal width);
    // 在ChartWidget类中添加
    public:
        bool isDataUpdated() const { return m_dataUpdated; }
        void setDataUpdated(bool updated) { m_dataUpdated = updated; }


private:
    void setupUI();
    void updateTitle();
    void calculateAmplitudeStats(const QVector<QPointF> &data);

    int m_channelId;
    int m_pointCount;

    // UI组件
    QChart *m_chart;
    QChartView *m_chartView;
    QLineSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QLabel *m_amplitudeLabel;

    // 统计信息
    double m_maxAmplitude;
    double m_minAmplitude;
    double m_rmsAmplitude;
    bool m_dataUpdated;
};

#endif // CHARTWIDGET_H
