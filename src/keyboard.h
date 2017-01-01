#pragma once

#include "server.h"


typedef void PortMidiStream;

class Keyboard {
public:

	void init();

	~Keyboard();

	enum { POLYPHONY = 5 };

	void process_midi_events();
	bool event(int scancode, bool state);

private:
	void midi_event(int type, int value);

	PortMidiStream* m_midi = nullptr;
	int	m_note_to_chan[128];
	int	m_chan_to_note[CHANNEL_COUNT];

	int	m_key_to_chan[512]; // SDL_NUM_SCANCODES
	int	m_chan_to_key[CHANNEL_COUNT];
};


extern Keyboard keyboard;
