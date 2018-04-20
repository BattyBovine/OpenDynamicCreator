#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#define TW_BEAT_MARKER_LENGTH	3.0f
#define TW_MEASURE_MARKER_MULT	2.0f

class TimelineWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TimelineWidget(QWidget *parent = 0);

protected:
	void paintEvent(QPaintEvent*);

private:
	void drawMeasures(QPainter&, int);
};

#endif // TIMELINEWIDGET_H
