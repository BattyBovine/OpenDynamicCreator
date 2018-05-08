#ifndef MUSICEVENT_H
#define MUSICEVENT_H

class Beat
{
#define WHOLE_NOTE_TICKS 256.0f
public:
	Beat(int beat=0) { this->iBeat=beat; }

	void setBeat(int b) { this->iBeat=b; }
	int beat() { return this->iBeat; }

	static Beat wholeNote() { return Beat(WHOLE_NOTE_TICKS); }
	static Beat halfNote() { return Beat(WHOLE_NOTE_TICKS/2); }
	static Beat quarterNote() { return Beat(WHOLE_NOTE_TICKS/4); }
	static Beat eighthNote() { return Beat(WHOLE_NOTE_TICKS/8); }
	static Beat sixteenthNote() { return Beat(WHOLE_NOTE_TICKS/16); }

	static Beat fromTimelinePosition(float pos, float measurespacing, quint8 beatspermeasure, quint8 beatunit, quint8 snap) { return Beat((roundf(((pos/measurespacing)*(beatspermeasure/float(beatunit)))*snap)/snap)*WHOLE_NOTE_TICKS); }
	float toTimelinePosition(float measurespacing, quint8 beatspermeasure, quint8 beatunit) const { return (this->iBeat*beatunit/WHOLE_NOTE_TICKS)*(measurespacing/beatspermeasure); }
	static Beat fromSeconds(float secs, float tempo, quint8 beatunit) { return Beat(roundf(secs*(tempo/60.0f)/beatunit*WHOLE_NOTE_TICKS)); }
	int measureCount(quint8 beatspermeasure, quint8 beatunit) const { return ceilf((this->iBeat/WHOLE_NOTE_TICKS)*(beatunit/float(beatspermeasure))); }

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
