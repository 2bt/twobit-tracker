#pragma once

#include <portmidi.h>

#include "server.h"


class Keyboard {
public:

	void init();

	void tick();

	~Keyboard();

	enum {
		POLYPHONY = 5
	};

private:
	void midi(int type, int value);

	PortMidiStream* m_midi = nullptr;
	int	m_note_to_chan[128];
	int	m_chan_to_note[CHANNEL_COUNT];

};


extern Keyboard keyboard;
