#include "chartManager.h"
#include <QScrollBar>
#include <QDebug>
#include <QResizeEvent>
#include <QApplication>
#include <QDesktopWidget>

ChartManager::ChartManager(QScrollArea *scrollArea, QObject *parent)
    : QObject(parent)
    , m_scrollArea(scrollArea)
    , m_chartsPerRow(1)
    , m_chartHeight(200)
    , m_visibleRowCount(0)
{
    m_scrollContent = new QWidget();
    m_mainLayout = new QVBoxLayout(m_scrollContent);
    m_mainLayout->setSpacing(2); // 减少间距，让更多空间用于图表
    m_mainLayout->setContentsMargins(2, 2, 2, 2);
    m_scrollArea->setWidget(m_scrollContent);
    m_scrollArea->setWidgetResizable(true);

    connect(m_scrollArea->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &ChartManager::onScrollBarValueChanged);

    m_scrollArea->viewport()->installEventFilter(this);
}

ChartManager::~ChartManager()
{
    clearCharts();
}

bool ChartManager::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_scrollArea->viewport() && event->type() == QEvent::Resize) {
        calculateChartSize();
        calculateVisibleChannels();
        updateChartVisibility();
    }
    return QObject::eventFilter(obj, event);
}

void ChartManager::setupChannels(int channelCount)
{
    clearCharts();

    for (int i = 0; i < channelCount; i++) {
        ChartWidget *chartWidget = new ChartWidget(i);
        chartWidget->setVisible(false);
        m_chartWidgets.append(chartWidget);
        m_mainLayout->addWidget(chartWidget);
    }

    // 计算图表大小
    calculateChartSize();

    calculateVisibleChannels();
    updateChartVisibility();
}

void ChartManager::calculateChartSize()
{
    if (m_chartWidgets.isEmpty()) {
        return;
    }

    // 获取滚动区域的可用高度（减去控制面板高度和边距）
    int availableHeight = m_scrollArea->viewport()->height();

    if (availableHeight <= 0) {
        // 如果还没有获取到高度，使用屏幕高度的80%作为估计
        QRect screenRect = QApplication::desktop()->screenGeometry();
        availableHeight = screenRect.height() * 0.8;
    }

    // 计算每个图表的高度：总高度 / 通道数量 - 间距
    int totalSpacing = (m_chartWidgets.size() - 1) * m_mainLayout->spacing();
    int totalMargins = m_mainLayout->contentsMargins().top() + m_mainLayout->contentsMargins().bottom();

    m_chartHeight = (availableHeight - totalSpacing - totalMargins) / m_chartWidgets.size();

    // 设置最小和最大高度限制
    m_chartHeight = qMax(80, m_chartHeight);  // 最小高度80px
    m_chartHeight = qMin(400, m_chartHeight); // 最大高度400px

    // 更新图表大小
    updateChartSizes();

    qDebug() << "Chart height calculation:";
    qDebug() << "  Available height:" << availableHeight;
    qDebug() << "  Channel count:" << m_chartWidgets.size();
    qDebug() << "  Total spacing:" << totalSpacing;
    qDebug() << "  Total margins:" << totalMargins;
    qDebug() << "  Calculated height:" << m_chartHeight;
}

void ChartManager::updateChartSizes()
{
    // 计算图表宽度（滚动区域宽度减去滚动条和边距）
    int chartWidth = m_scrollArea->viewport()->width() - 20;
    chartWidth = qMax(400, chartWidth); // 最小宽度400px

    QSize chartSize(chartWidth, m_chartHeight);

    for (ChartWidget *chart : m_chartWidgets) {
        chart->setMinimumSize(chartSize);
        chart->setMaximumSize(chartSize);
    }

    qDebug() << "Updated chart sizes:" << chartSize;
}

void ChartManager::updateVisibleChannels()
{
    calculateVisibleChannels();
    updateChartVisibility();
}

void ChartManager::updateChannelData(int channelId, const QVector<QPointF> &data)
{
    if (channelId >= 0 && channelId < m_chartWidgets.size()) {
        if (m_visibleChannels.contains(channelId)) {
            m_chartWidgets[channelId]->updateData(data);
        }
    }
}

void ChartManager::updateAllAmplitudeDisplays()
{
    for (ChartWidget *chart : m_chartWidgets) {
        if (chart->isVisible()) {
            chart->updateAmplitudeDisplay();
        }
    }
}

void ChartManager::setChartsPerRow(int count)
{
    m_chartsPerRow = qMax(1, count);
    calculateChartSize();
    calculateVisibleChannels();
    updateChartVisibility();
}

ChartWidget* ChartManager::getChartWidget(int channelId) const
{
    if (channelId >= 0 && channelId < m_chartWidgets.size()) {
        return m_chartWidgets[channelId];
    }
    return nullptr;
}

void ChartManager::clearCharts()
{
    for (ChartWidget *chart : m_chartWidgets) {
        delete chart;
    }
    m_chartWidgets.clear();
    m_visibleChannels.clear();
}

void ChartManager::onScrollBarValueChanged()
{
    calculateVisibleChannels();
    updateChartVisibility();
}

void ChartManager::calculateVisibleChannels()
{
    if (m_chartWidgets.isEmpty()) return;

    m_visibleChannels.clear();

    int scrollAreaHeight = m_scrollArea->viewport()->height();
    int scrollPosition = m_scrollArea->verticalScrollBar()->value();

    if (scrollAreaHeight <= 0) return;

    int chartTotalHeight = m_chartHeight + m_mainLayout->spacing();

    if (chartTotalHeight <= 0) return;

    // 计算可见的行范围
    int startRow = scrollPosition / chartTotalHeight;
    int endRow = (scrollPosition + scrollAreaHeight) / chartTotalHeight;

    // 限制范围
    startRow = qMax(0, startRow);
    endRow = qMin(m_chartWidgets.size() - 1, endRow);

    // 计算可见的通道
    for (int row = startRow; row <= endRow; row++) {
        if (row >= 0 && row < m_chartWidgets.size()) {
            m_visibleChannels.insert(row);
        }
    }

    m_visibleRowCount = m_visibleChannels.size();

    qDebug() << "Visible channels:" << m_visibleChannels.size()
             << "out of" << m_chartWidgets.size()
             << "rows:" << startRow << "to" << endRow;
}

void ChartManager::updateChartVisibility()
{
    for (int i = 0; i < m_chartWidgets.size(); i++) {
        bool shouldBeVisible = m_visibleChannels.contains(i);
        if (m_chartWidgets[i]->isVisible() != shouldBeVisible) {
            m_chartWidgets[i]->setVisible(shouldBeVisible);
        }
    }
}
