/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "datasource.h"
#include <QtCore/QtMath>
#include <QtCore/QRandomGenerator>

QT_CHARTS_USE_NAMESPACE

DataSource::DataSource(QObject *parent) :
    QObject(parent),
    m_index(-1)
{
    generateData(0, 0, 0);
}

void DataSource::update(QAbstractSeries *series, int seriesIndex)
{
    if (series) {
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);
        const QVector<QVector<QPointF> > &seriesData = m_data.at(seriesIndex);
        if (seriesIndex == 0)
            m_index++;
        if (m_index > seriesData.count() - 1)
            m_index = 0;

        QVector<QPointF> points = seriesData.at(m_index);
        // Use replace instead of clear + append, it's optimized for performance
        xySeries->setUseOpenGL(true);
        xySeries->replace(points);
    }
}

void DataSource::handleSceneChanged()
{
    m_dataUpdater.start();
}

void DataSource::updateAllSeries()
{
    static int frameCount = 0;
    static QString labelText = QStringLiteral("FPS: %1");

    for (int i = 0; i < m_seriesList.size(); i++)
        update(m_seriesList[i], i);

    frameCount++;
    int elapsed = m_fpsTimer.elapsed();
    if (elapsed >= 1000) {
        elapsed = m_fpsTimer.restart();
        qreal fps = qreal(0.1 * int(10000.0 * (qreal(frameCount) / qreal(elapsed))));
        m_fpsLabel->setText(labelText.arg(QString::number(fps, 'f', 1)));
        m_fpsLabel->adjustSize();
        frameCount = 0;
    }
}

void DataSource::startUpdates(const QList<QXYSeries *> &seriesList, QLabel *fpsLabel)
{
    m_seriesList = seriesList;
    m_fpsLabel = fpsLabel;

    m_dataUpdater.setInterval(0);
    m_dataUpdater.setSingleShot(true);
    QObject::connect(&m_dataUpdater, &QTimer::timeout,
                     this, &DataSource::updateAllSeries);

    m_fpsTimer.start();
    updateAllSeries();
}

void DataSource::generateData(int seriesCount, int rowCount, int colCount)
{

    m_data.clear();

        qreal xAdjustment = 20.0 / (colCount * rowCount);

        // 为每个通道生成不同频率和相位的正弦波
        for (int k(0); k < seriesCount; k++) {
            QVector<QVector<QPointF> > seriesData;

            // 每个通道有不同的频率和相位
            qreal frequency = 1.0 + (qreal(k) * 0.5);  // 频率从1.0开始递增
            qreal phase = (qreal(k) * M_PI) / 16.0;    // 相位偏移
            qreal amplitude = 1.0;                     // 振幅

            for (int i(0); i < rowCount; i++) {
                QVector<QPointF> points;
                points.reserve(colCount);
                for (int j(0); j < colCount; j++) {
                    qreal x = 0.000001 + 20.0 * (qreal(j) / qreal(colCount)) + (xAdjustment * qreal(i));

                    // 生成正弦波，添加少量随机噪声
                    qreal y = amplitude * qSin(frequency * M_PI * j / 50.0 + phase)
                             + 0.1 * QRandomGenerator::global()->generateDouble();

                    points.append(QPointF(x, y));
                }
                seriesData.append(points);
            }
            m_data.append(seriesData);
        }
}
