#ifndef BEAT_H
#define BEAT_H

#include <QtGlobal>

class Beat
{
#define WHOLE_NOTE_TICKS 256.0f
public:
	Beat(int tick=0) { this->iTick=tick; }

	void setTick(int b) { this->iTick=b; }
	int tick() { return this->iTick; }

	static Beat measures(int measures, quint8 beatspermeasure, quint8 beatunit) {
		int ticks = (WHOLE_NOTE_TICKS*measures);
		float ratio = (beatspermeasure/float(beatunit));
		return Beat(roundf(ticks*ratio));
	}
	static Beat wholeNote() { return Beat(WHOLE_NOTE_TICKS); }
	static Beat halfNote() { return Beat(WHOLE_NOTE_TICKS/2); }
	static Beat quarterNote() { return Beat(WHOLE_NOTE_TICKS/4); }
	static Beat eighthNote() { return Beat(WHOLE_NOTE_TICKS/8); }
	static Beat sixteenthNote() { return Beat(WHOLE_NOTE_TICKS/16); }
	static Beat fromUnit(quint8 u) { return Beat(WHOLE_NOTE_TICKS/u); }

	static Beat fromTimelinePosition(float pos, float measurespacing, quint8 beatspermeasure, quint8 beatunit, quint8 snap) { return Beat((roundf(((pos/measurespacing)*(beatspermeasure/float(beatunit)))*snap)/snap)*WHOLE_NOTE_TICKS); }
	float toTimelinePosition(float measurespacing, quint8 beatspermeasure, quint8 beatunit) const { return (this->iTick*beatunit/WHOLE_NOTE_TICKS)*(measurespacing/beatspermeasure); }
	static Beat fromSeconds(float secs, float tempo, quint8 beatunit) { return Beat(roundf(secs*(tempo/60.0f)/beatunit*WHOLE_NOTE_TICKS)); }
	float toSeconds(float tempo, quint8 beatunit) { return (this->iTick*(60.0f/tempo)*beatunit/WHOLE_NOTE_TICKS); }
	int measureCount(quint8 beatspermeasure, quint8 beatunit) const { return ceilf((this->iTick/WHOLE_NOTE_TICKS)*(beatunit/float(beatspermeasure))); }

	Beat operator+(const Beat &b)	{ return Beat(iTick+b.iTick); }
	Beat operator-(const Beat &b)	{ return Beat(iTick-b.iTick); }
	Beat operator*(const Beat &b)	{ return Beat(iTick*b.iTick); }
	Beat operator/(const Beat &b)	{ return Beat(iTick/b.iTick); }

	Beat &operator+=(const Beat &b)	{ iTick+=b.iTick; return *this; }
	Beat &operator-=(const Beat &b)	{ iTick-=b.iTick; return *this; }
	Beat &operator*=(const Beat &b)	{ iTick*=b.iTick; return *this; }
	Beat &operator/=(const Beat &b)	{ iTick/=b.iTick; return *this; }

	bool operator==(const Beat &b) const	{ return (iTick==b.iTick); }
	bool operator!=(const Beat &b) const	{ return !(*this==b); }
	bool operator<(const Beat &b) const		{ return (iTick<b.iTick); }
	bool operator<=(const Beat &b) const	{ return (iTick<=b.iTick); }
	bool operator>(const Beat &b) const		{ return (iTick>b.iTick); }
	bool operator>=(const Beat &b) const	{ return (iTick>=b.iTick); }

	friend bool operator<=(const int &a, const Beat &b)	{ return (a<=b.iTick); }
	friend bool operator>=(const int &a, const Beat &b)	{ return (a>=b.iTick); }

private:
	int iTick;
};

#endif // BEAT_H
