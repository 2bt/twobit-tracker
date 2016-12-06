#include "keyboard.h"
#include "patternwin.h"


void Keyboard::init() {
	Pm_Initialize();
	int dev_count = Pm_CountDevices();
	int i;
	for (i = 0; i < dev_count; i++) {
		auto info = Pm_GetDeviceInfo(i);
		if (info->output &&
			std::string(info->name).find("MIDI") != std::string::npos) break;
	}
	if (i < dev_count) Pm_OpenInput(&m_midi, 3, nullptr, 0, nullptr, nullptr);

	for (auto& c : m_note_to_chan) c = -1;
	for (auto& n : m_chan_to_note) n = -1;
}


Keyboard::~Keyboard() {
	if (m_midi) Pm_Close(m_midi);
}


void Keyboard::tick() {
	// rough and ready midi support
	if (!m_midi) return;
	struct { unsigned char type, val, x, y; } event;
	for (;;) {
		if (!Pm_Read(m_midi, (PmEvent*) &event, 1)) break;
		midi(event.type, event.val);
	}
}


void Keyboard::midi(int type, int value) {

	Row row;
	int chan;

	if (type == 128) { // note off event
		if (m_note_to_chan[value] == -1) return;
		chan = m_note_to_chan[value];
		m_chan_to_note[chan] = -1;
		m_note_to_chan[value] = -1;
		row.note = -1;
	}
	else if (type == 144) {

		chan = pat_win.get_active_channel();
		for (int i = 1; i < POLYPHONY; i++) {
			if (m_chan_to_note[chan] == -1) break;
			chan = (chan + 1) % CHANNEL_COUNT;
		}
		int old_note = m_chan_to_note[chan];
		if (old_note != -1) m_note_to_chan[old_note] = -1;
		m_chan_to_note[chan] = value;
		m_note_to_chan[value] = chan;

		row.note = value + 1;
		row.macros[0] = pat_win.get_macro();
	}
	else return;


	server.play_row(chan, row);


	// record note off event only if no other voice active
	if (row.note == -1) {
		for (int n : m_chan_to_note) if (n != -1) return;
	}

	pat_win.jam(row);
}
