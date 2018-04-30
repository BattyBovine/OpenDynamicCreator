#ifndef MUSICEVENT_H
#define MUSICEVENT_H

class Beat
{
#define BEAT_TICK_UNIT 256.0f
public:
	Beat(int beat=0) { this->iBeat=beat; }

	void setBeat(int b) { this->iBeat=b; }
	int beat() { return this->iBeat; }

	static Beat fromTimelinePosition(float pos, float measurespacing, int beatspermeasure, quint8 beatunit, quint8 snap) { return Beat((roundf(((pos/measurespacing)*(beatspermeasure/float(beatunit)))*snap)/snap)*BEAT_TICK_UNIT); }
	float toTimelinePosition(float measurespacing, int beatspermeasure, quint8 beatunit) const { return (this->iBeat/BEAT_TICK_UNIT)*measurespacing*(beatunit/float(beatspermeasure)); }
	static Beat fromSeconds(float secs, float tempo, int beatspermeasure, int beatunit) {
//		return Beat(secs*(60.0f/tempo)*BASE_BEAT_UNIT);
		return Beat((roundf(secs*(60.0f/tempo)*(beatspermeasure/float(beatunit))*BEAT_TICK_UNIT)));
	}
//	float toSeconds() const { return this->iBeat*(60.0f/this->fTempo); }
	int measureCount() const { return ceilf(this->iBeat/BEAT_TICK_UNIT); }

private:
	int iBeat;
};

class MusicEvent
{
public:
	MusicEvent(Beat &b=Beat()) { this->setBeat(b); }

	void setBeat(Beat &b) { this->oBeat=b; }
	Beat beat() const { return this->oBeat; }

private:
	Beat oBeat;
};
typedef QVector<MusicEvent> MusicEventList;

#endif // MUSICEVENT_H
