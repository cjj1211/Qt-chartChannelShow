#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QVector>
#include <QSet>
#include "chartWidget.h"

class ChartManager : public QObject
{
    Q_OBJECT

public:
    explicit ChartManager(QScrollArea *scrollArea, QObject *parent = nullptr);
    ~ChartManager();

    void setupChannels(int channelCount);
    void updateVisibleChannels();
    void updateChannelData(int channelId, const QVector<QPointF> &data);
    void updateAllAmplitudeDisplays();

    // 配置接口
    void setChartsPerRow(int count);
    void calculateChartSize();

    ChartWidget* getChartWidget(int channelId) const;
    int channelCount() const { return m_chartWidgets.size(); }
    QSet<int> getVisibleChannels() const { return m_visibleChannels; }

private slots:
    void onScrollBarValueChanged();

private:
    void clearCharts();
    void calculateVisibleChannels();
    void updateChartVisibility();
    void updateChartSizes();
    bool eventFilter(QObject *obj, QEvent *event) override;

    QScrollArea *m_scrollArea;
    QWidget *m_scrollContent;
    QVBoxLayout *m_mainLayout;
    QVector<ChartWidget*> m_chartWidgets;
    QSet<int> m_visibleChannels;
    int m_chartsPerRow;
    int m_chartHeight;
    int m_visibleRowCount;
};

#endif // CHARTMANAGER_H
