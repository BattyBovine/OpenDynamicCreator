#ifndef MUSICEVENT_H
#define MUSICEVENT_H

class Beat
{
#define BEAT_TICK_UNIT 256.0f
public:
	Beat(int beat=0) { this->iBeat=beat; }

	void setBeat(int b) { this->iBeat=b; }
	int beat() { return this->iBeat; }

	static Beat wholeNote() { return Beat(BEAT_TICK_UNIT); }
	static Beat halfNote() { return Beat(BEAT_TICK_UNIT/2); }
	static Beat quarterNote() { return Beat(BEAT_TICK_UNIT/4); }
	static Beat eighthNote() { return Beat(BEAT_TICK_UNIT/8); }
	static Beat sixteenthNote() { return Beat(BEAT_TICK_UNIT/16); }

	static Beat fromTimelinePosition(float pos, float measurespacing, int beatspermeasure, quint8 beatunit, quint8 snap) { return Beat((roundf(((pos/measurespacing)*(beatspermeasure/float(beatunit)))*snap)/snap)*BEAT_TICK_UNIT); }
	float toTimelinePosition(float measurespacing, int beatspermeasure, quint8 beatunit) const { return (this->iBeat/BEAT_TICK_UNIT)*measurespacing*(beatunit/float(beatspermeasure)); }
	static Beat fromSeconds(float secs, float tempo, int beatspermeasure) { return Beat(roundf(secs*(tempo/60.0f)/beatspermeasure*BEAT_TICK_UNIT)); }
//	float toSeconds() const { return this->iBeat*(60.0f/this->fTempo); }
	int measureCount() const { return ceilf(this->iBeat/BEAT_TICK_UNIT); }

	Beat operator+(const Beat &b)	{ return Beat(iBeat+b.iBeat); }
	Beat operator-(const Beat &b)	{ return Beat(iBeat-b.iBeat); }
	Beat operator*(const Beat &b)	{ return Beat(iBeat*b.iBeat); }

	bool operator==(const Beat &b) const	{ return (iBeat==b.iBeat); }
	bool operator!=(const Beat &b) const	{ return !(*this==b); }
	bool operator<(const Beat &b) const		{ return (iBeat<b.iBeat); }
	bool operator<=(const Beat &b) const	{ return (iBeat<=b.iBeat); }
	bool operator>(const Beat &b) const		{ return (iBeat>b.iBeat); }
	bool operator>=(const Beat &b) const	{ return (iBeat>=b.iBeat); }

	friend bool operator<=(const int &a, const Beat &b)	{ return (a<=b.iBeat); }
	friend bool operator>=(const int &a, const Beat &b)	{ return (a>=b.iBeat); }

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
