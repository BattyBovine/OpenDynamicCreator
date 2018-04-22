#ifndef MUSICPROPERTIESWIDGET_H
#define MUSICPROPERTIESWIDGET_H

#include <QScrollArea>

#include "src/qt/MusicTreeView.h"

namespace Ui {
class MusicPropertiesWidget;
}

class MusicPropertiesWidget : public QScrollArea
{
	Q_OBJECT

public:
	explicit MusicPropertiesWidget(TrackItem *track, QWidget *parent = 0);
	~MusicPropertiesWidget();

	void attachTrackItem(TrackItem*);

	int tempo();
	int beatsPerMeasure();
	int beatUnit();
	double playbackSpeed();

public slots:
	void tempoChanged(int t);
	void setTempo(int t);
	void beatsPerMeasureChanged(int b);
	void setBeatsPerMeasure(int b);
	void beatUnitChanged(int b);
	void setBeatUnit(int b);
	void playbackSpeedChanged(double s);
	void setPlaybackSpeed(double s);

private:
	Ui::MusicPropertiesWidget *ui;

	TrackItem *tiTrack = NULL;
};

#endif // MUSICPROPERTIESWIDGET_H
