#include <ctype.h>
#include <string.h>

#include "patternwin.h"
#include "messagewin.h"
#include "display.h"
#include "server.h"




void PatternWin::resize() {
	m_width = display.width() - m_left;
	m_height = display.height() - m_top - 2;
	m_scroll_x_view = (m_width - 4) / (CHAN_CHAR_WIDTH + 1);

	m_scroll_y0_view = 8;
	m_scroll_y1_view = m_height - m_scroll_y0_view - 5;

	scroll();
}

void PatternWin::scroll() {
	if (m_scroll_x > m_cursor_x) m_scroll_x = m_cursor_x;
	if (m_scroll_x < m_cursor_x - m_scroll_x_view + 1) {
		m_scroll_x = m_cursor_x - m_scroll_x_view + 1;
	}
	if (m_scroll_y0 > m_cursor_y0) m_scroll_y0 = m_cursor_y0;
	if (m_scroll_y0 < m_cursor_y0 - m_scroll_y0_view + 1) {
		m_scroll_y0 = m_cursor_y0 - m_scroll_y0_view + 1;
	}

	// scroll with scrolloff
	int scrolloff = std::min(m_scroll_y1_view / 2, 8);
	int y = std::max(0, m_cursor_y1 - scrolloff);
	m_scroll_y1 = std::min(m_scroll_y1, y);
	int max_rows = std::max(1, get_max_rows(*m_tune, m_cursor_y0) - m_scroll_y1_view);
	y = std::min(max_rows, m_cursor_y1 - m_scroll_y1_view + 1 + scrolloff);
	m_scroll_y1 = std::max(m_scroll_y1, y);
}

void PatternWin::move_cursor(int dx, int dy0, int dy1) {
	if (dx) m_cursor_x = (m_cursor_x + dx + CHANNEL_COUNT) % CHANNEL_COUNT;
	if (dy0) m_cursor_y0 = (m_cursor_y0 + dy0 + m_tune->table.size()) % m_tune->table.size();
	if (dy1) {
		int max_rows = std::max(1, get_max_rows(*m_tune, m_cursor_y0));
		m_cursor_y1 = (m_cursor_y1 + dy1 + max_rows) % max_rows;
	}
	scroll();
}


// colors
enum {
	FG_FRAME		= 0x002222,
	FG_NOTE			= 0xffffff,
	FG_MACRO		= 0xbbbbbb,
	FG_PATTERN		= 0xbbbbbb,
	FG_NUMBER		= 0xbbbbbb,
	FG_LEVEL		= 0x00ff00,

	BG_BLANK		= 0x000000,
	BG_MAKRED		= 0x336600,
	BG_PLAYING		= 0x002222,
	BG_BAR			= 0x0a0a0a,

	BG_CURSOR		= 0x443300,
	BG_CURSOR_EDIT	= 0x550000,
	BG_CURSOR_REC	= 0x550000,
};


void PatternWin::draw() {


	int server_row = server.get_row();
	int server_line = server.get_line();

	// follow
	if (m_edit_mode == EM_RECORD) {
		m_cursor_y0 = server_line;
		m_cursor_y1 = server_row;
		scroll();
	}


	// line numbers
	display.style(FG_FRAME, BG_BLANK);
	display.mvprint(m_left, m_top, "   ");
	display.put(ULCORNER);

	display.mvprint(m_left, m_top + 1, "   ");
	display.put(VLINE);
	display.move(m_left, m_top + 2);
	display.put(ULCORNER);
	display.put(HLINE, 2);
	display.put(PLUS);
	for (int r = 0; r < m_scroll_y0_view; r++) {
		display.move(m_left, m_top + r + 3);
		display.put(VLINE);
		display.style(FG_NUMBER, BG_BLANK, r + m_scroll_y0 == m_cursor_y0);
		if (r + m_scroll_y0 >= (int) m_tune->table.size()) display.print("  ");
		else display.printf("%02X", r + m_scroll_y0);
		display.style(FG_FRAME, BG_BLANK);
		display.put(VLINE);
	}

	auto& line = m_tune->table[m_cursor_y0];
	int max_rows = get_max_rows(*m_tune, m_cursor_y0);
	int y1 = m_top + m_scroll_y0_view + 3;

	display.move(m_left, y1);
	display.put(LTEE);
	display.put(HLINE, 2);
	display.put(PLUS);
	for (int r = 0; r < m_scroll_y1_view; r++) {
		int i = r + m_scroll_y1;
		display.move(m_left, y1 + r + 1);
		display.put(VLINE);
		display.style(FG_NUMBER, BG_BLANK, r + m_scroll_y1 == m_cursor_y1);
		if (i >= max_rows) display.print("  ");
		else display.printf("%02X", i);
		display.style(FG_FRAME, BG_BLANK);
		display.put(VLINE);
	}

	display.move(m_left, m_top + m_height - 1);
	display.put(' ');
	display.put(HLINE, 2);
	display.put(BTEE);


	int x = m_left + 4;
	int chan_limit = std::min(m_scroll_x + m_scroll_x_view, (int) CHANNEL_COUNT);
	for (int chan_nr = m_scroll_x; chan_nr < chan_limit; chan_nr++, x += CHAN_CHAR_WIDTH + 1) {

		auto& pat_name = line[chan_nr];
		auto pat = m_tune->patterns.count(pat_name) ? &m_tune->patterns[pat_name] : nullptr;


		// table head
		display.move(x, m_top);
		display.put(HLINE, CHAN_CHAR_WIDTH);
		display.put(chan_nr < chan_limit - 1 ? TTEE : URCORNER);


		// channel level
		display.style(FG_LEVEL, BG_BLANK);
		display.move(x, m_top + 1);
		float level = clamp(server.get_chan_level(chan_nr)) * CHAN_CHAR_WIDTH;
		for (int i = 0; i < CHAN_CHAR_WIDTH; i++) {
			display.put( level > i + 0.25 ? level > i + 0.75 ? LEVELHEIGH : LEVELLOW : ' ');
		}

		display.bold(false);
		display.fg(FG_NUMBER);
		if (server.get_muted(chan_nr)) {
			display.move(x, m_top + 1);
			display.printf("MUTED");
		}
		display.move(x + CHAN_CHAR_WIDTH - 1, m_top + 1);
		display.printf("%X", chan_nr);


		display.style(FG_FRAME, BG_BLANK);
		display.put(VLINE);
		display.move(x, m_top + 2);
		display.put(HLINE, CHAN_CHAR_WIDTH);
		display.put(chan_nr < chan_limit - 1 ? PLUS : RTEE);

		display.move(x, y1);
		display.put(HLINE, CHAN_CHAR_WIDTH);
		display.put(chan_nr < chan_limit - 1 ? PLUS : RTEE);


		// upper table
		for (int r = 0; r < m_scroll_y0_view; r++) {
			int i = r + m_scroll_y0;
			display.move(x, m_top + r + 3);
			if (i < (int) m_tune->table.size()) {
				auto pn = m_tune->table[i][chan_nr];

				display.bold(false);
				display.fg(FG_PATTERN);
				uint32_t bg = BG_BLANK;
				if (i == m_cursor_y0 && m_cursor_x == chan_nr) bg = BG_CURSOR;
				else if	(i == server_line && !pn.empty()) bg = BG_PLAYING;
				display.bg(bg);
				display.printf("%s", pn.c_str());
				display.put(pn == "" ? ' ' : '.', CHAN_CHAR_WIDTH - pn.size());
			}
			else {
				display.bg(BG_BLANK);
				display.put(' ', CHAN_CHAR_WIDTH);
			}
			display.style(FG_FRAME, BG_BLANK);
			display.put(VLINE);
		}

		// bottom table
		for (int r = 0; r < m_scroll_y1_view; r++) {
			int i = r + m_scroll_y1;
			bool on_pat = (pat && i < (int) pat->size());

			uint32_t bg = (on_pat && i % 16 == 0) ? BG_BAR : BG_BLANK;
			if (on_pat && i == server_row
			&& m_tune->table[server_line][chan_nr] == pat_name) bg = BG_PLAYING;
			if (i == m_cursor_y1 && m_cursor_x == chan_nr) {
				bg = BG_CURSOR;
//				bg = (m_edit_mode == EM_MACRO_NAME) ? S_ET_NOTE :
//						(m_edit_mode == EM_RECORD) ? S_RC_NOTE :
//						S_CS_NOTE;
			}
			if (m_edit_mode == EM_MARK_PATTERN
			&& mark_x_begin() <= chan_nr && chan_nr < mark_x_end()
			&& mark_y_begin() <= i && i < mark_y_end()) bg = BG_MAKRED;
			display.bg(bg);

			display.move(x, y1 + r + 1);
			if (on_pat) {
				auto& row = pat->at(i);

				display.fg(FG_NOTE);
				if (row.note > 0) {
					display.printf("%c%c%X",
						"CCDDEFFGGAAB"[(row.note - 1) % 12],
						"-#-#--#-#-#-"[(row.note - 1) % 12],
						(row.note - 1) / 12);
				}
				else if (row.note == -1) display.print("===");
				else display.print("...");

				display.bold(false);
				display.fg(FG_MACRO);
				for (int m = 0; m < MACROS_PER_ROW; m++) {
					std::string m_macro = row.macros[m];
					display.printf(" %s", m_macro.c_str());
					display.put('.', MACRO_CHAR_WIDTH - m_macro.size());
				}
			}
			else {
				display.put(' ', CHAN_CHAR_WIDTH);
			}
			display.style(FG_FRAME, BG_BLANK);
			display.put(VLINE);
		}

		display.style(FG_FRAME, BG_BLANK);
		display.move(x, m_top + m_height - 1);
		display.put(HLINE, CHAN_CHAR_WIDTH);
		display.put(chan_nr < chan_limit - 1 ? BTEE : LRCORNER);
	}


	// extra editing info
	display.style(FG_FRAME, BG_BLANK);
	display.move(m_left, m_top + m_height - 1);
	display.put(LTEE);
	display.move(m_left + 3 + MACRO_CHAR_WIDTH, m_top + m_height - 1);
	display.put(TTEE);


	display.move(m_left, m_top + m_height);
	display.put(VLINE);
	display.style(FG_NOTE, BG_BLANK);
	display.put('0' + m_octave);
	display.put(' ');
	display.style(FG_MACRO, BG_BLANK, false);
	display.printf("%s", m_macro.c_str());
	display.put('.', MACRO_CHAR_WIDTH - m_macro.size());
	display.style(FG_FRAME, BG_BLANK);
	display.put(VLINE);


	display.move(m_left, m_top + m_height + 1);
	display.put(LLCORNER);
	display.put(HLINE, 2 + MACRO_CHAR_WIDTH);
	display.put(LRCORNER);


	// TODO:
//	// set cursor position
//	if (m_edit_mode == EM_PATTERN_NAME) {
//		curs_set(1);
//		move(m_top + 3 + m_cursor_y0 - m_scroll_y0,
//			m_left + (m_cursor_x - m_scroll_x) * (CHAN_CHAR_WIDTH + 1) + 4 + line[m_cursor_x].size());
//	}
//	else if (m_edit_mode == EM_MACRO_NAME) {
//		curs_set(1);
//		auto& pat_name = line[m_cursor_x];
//		auto& pat = m_tune->patterns[pat_name];
//		auto& row = pat[m_cursor_y1];
//		move(m_top + 3 + m_scroll_y0_view + m_cursor_y1 - m_scroll_y1 + 1,
//			m_left + (m_cursor_x - m_scroll_x) * (CHAN_CHAR_WIDTH + 1) + 4 + row.macros[0].size() + 4);
//	}
//	else curs_set(0);
}



// input handling

enum {
	KEY_CTRL_DOWN = 525,
	KEY_CTRL_UP = 566,
	KEY_CTRL_RIGHT = 560,
	KEY_CTRL_LEFT = 545,
	KEY_CTRL_A = 1,
	KEY_CTRL_O = 15,
	KEY_CTRL_R = 18,
	KEY_CTRL_X = 24,
	KEY_CTRL_N = 14,
	KEY_CTRL_D = 4,
	KEY_ESCAPE = 27,
	KEY_TAB = 9,
};


void PatternWin::key(int key, int mod) {
	switch (m_edit_mode) {
	case EM_PATTERN_NAME:	key_pattern_name(key, mod); break;
	case EM_MACRO_NAME:		key_macro_name(key, mod); break;
	case EM_MARK_PATTERN:	key_mark_pattern(key, mod); break;
	case EM_RECORD:
	case EM_NORMAL:			key_normal(key, mod); break;
	}
}

typedef EditCommand EC;

void PatternWin::key_pattern_name(int key, int mod) {
	auto& line = m_tune->table[m_cursor_y0];
	auto& pat_name = line[m_cursor_x];
	if ((isalnum(key) || strchr("_-+", key)) && pat_name.size() < PATTERN_CHAR_WIDTH) {
		pat_name += key;
	}
	else if (key == SDLK_BACKSPACE && pat_name.size() > 0) pat_name.pop_back();
	else if (key == SDLK_ESCAPE) {
		m_edit_mode = EM_NORMAL;
		pat_name.assign(m_old_name);
	}
	else if (key == SDLK_RETURN) {
		m_edit_mode = EM_NORMAL;

		if (pat_name != "" && m_tune->patterns.count(pat_name) == 0) {
			if (m_rename_pattern && m_old_name != "") { // rename pattern
				m_tune->patterns[pat_name] = m_tune->patterns[m_old_name];
			}
			else {
				// new pattern
				int len = std::max<int>(1, get_max_rows(*m_tune, m_cursor_y0));
				m_tune->patterns[pat_name].resize(len);
			}
		}
	}
}

void PatternWin::key_macro_name(int key, int mod) {
	auto& line = m_tune->table[m_cursor_y0];
	auto& pat_name = line[m_cursor_x];
	auto& pat = m_tune->patterns[pat_name];
	auto& row = pat[m_cursor_y1];
	auto& macro_name = row.macros[0];

	if ((isalnum(key) || strchr("_-+", key)) && macro_name.size() < MACRO_CHAR_WIDTH) {
		macro_name += key;
	}
	else if (key == SDLK_BACKSPACE && macro_name.size() > 0) macro_name.pop_back();
	else if (key == SDLK_ESCAPE) {
		m_edit_mode = EM_NORMAL;
		macro_name.assign(m_old_name);
	}
	else if (key == SDLK_RETURN) {
		m_edit_mode = EM_NORMAL;
		m_macro = macro_name;
		macro_name.assign(m_old_name);
		edit(EC::SET_MACRO, 0);
	}
}

void PatternWin::key_mark_pattern(int key, int mod) {
	switch (key) {
	case SDLK_UP:		move_cursor( 0, 0, -1); return;
	case SDLK_DOWN:		move_cursor( 0, 0,  1); return;
	case SDLK_PAGEUP:	move_cursor( 0, 0, -4); return;
	case SDLK_PAGEDOWN:	move_cursor( 0, 0,  4); return;
	case SDLK_RIGHT:	move_cursor( 1, 0,  0); return;
	case SDLK_LEFT:		move_cursor(-1, 0,  0); return;
	case SDLK_HOME:
		m_cursor_y1 = 0;
		scroll();
		return;
	case SDLK_END:
		m_cursor_y1 = std::max(0, get_max_rows(*m_tune, m_cursor_y0) - 1);
		scroll();
		return;

	// mark whole pattern
	case 'V':
		m_mark_y = 0;
		m_cursor_y1 = std::max<int>(0, get_max_rows(*m_tune, m_cursor_y0) - 1);
		scroll();
		return;

	case SDLK_ESCAPE:
		m_edit_mode = EM_NORMAL;
		return;

	// copy pattern to buffer
	case 'y':
	case 'd':
	case SDLK_BACKSPACE:
		m_edit_mode = EM_NORMAL;
		edit(EC::YANK_REGION, key != 'y');
		return;

	// transpose
	case '>':
	case '<':
		edit(EC::TRANSPOSE_REGION, key == '>' ? 1 : -1);
		return;

	default: break;
	}
}


void PatternWin::key_normal(int key, int mod) {

	switch (key) {
	case SDLK_RIGHT:	move_cursor(1, 0, 0); return;
	case SDLK_LEFT:		move_cursor(-1, 0, 0); return;

	case '<':
		if(--m_octave < 0) m_octave = 0;
		return;
	case '>':
		if (++m_octave > 8) m_octave = 8;
		return;

	case '+': {
			auto it = m_tune->macros.find(m_macro);
			it++;
			if (m_macro == "") it = m_tune->macros.begin();
			if (it != m_tune->macros.end()) m_macro = it->first;
			else m_macro = "";
		}
		return;
	case '-': {
			MacroMap::reverse_iterator it(m_tune->macros.find(m_macro));
			if (m_macro == "") it = m_tune->macros.rbegin();
			if (it != m_tune->macros.rend()) m_macro = it->first;
			else m_macro = "";
		}
		return;

	case '\0':	// continue playing
		if (server.is_playing()) {
			m_edit_mode = EM_NORMAL;
			server.pause();
		}
		else server.play(server.get_line(), server.get_row());
		return;

	case SDLK_SPACE:	// play from the begining current line
		if (server.is_playing()) {
			m_edit_mode = EM_NORMAL;
			server.pause();
		}
		else server.play(m_cursor_y0);
		return;

	case SDLK_RETURN:	// loop current line
		if (server.is_playing()) {
			m_edit_mode = EM_NORMAL;
			server.pause();
		}
		else server.play(m_cursor_y0, 0, true);
		return;

	case SDLK_TAB:
		if (m_edit_mode == EM_RECORD) {
			m_edit_mode = EM_NORMAL;
		}
		else {
			m_edit_mode = EM_RECORD;
			if (!server.is_playing()) server.play(m_cursor_y0, m_cursor_y1);
		}
		return;


	case 'm':	// mute
		if (mod & KMOD_SHIFT) {
			server.set_muted(m_cursor_x, !server.get_muted(m_cursor_x));
			if (server.get_muted(m_cursor_x)) server.play_row(m_cursor_x, { -1 });
		}
		return;
	case 'l':	// solo
		if (mod & KMOD_SHIFT) {
			int s = 0;
			for (int i = 0; i < CHANNEL_COUNT; i++) s += server.get_muted(i);
			if (!server.get_muted(m_cursor_x) && s == CHANNEL_COUNT - 1) {
				for (int i = 0; i < CHANNEL_COUNT; i++) server.set_muted(i, false);
			}
			else {
				for (int i = 0; i < CHANNEL_COUNT; i++) {
					server.set_muted(i, i != m_cursor_x);
					if (i != m_cursor_x) server.play_row(i, { -1 });
				}
			}
		}
		return;

	default: break;
	}


	if (m_edit_mode != EM_RECORD) {

		auto& line = m_tune->table[m_cursor_y0];
		auto& pat_name = line[m_cursor_x];
		auto pat = m_tune->patterns.count(pat_name) ? &m_tune->patterns[pat_name] : nullptr;
		auto row = (pat && m_cursor_y1 < (int) pat->size()) ? &pat->at(m_cursor_y1) : nullptr;

		switch (key) {
		case SDLK_UP:
			if (mod & KMOD_SHIFT) move_cursor(0, -1,  0);
			else move_cursor(0,  0, -1);
			return;
		case SDLK_DOWN:
			if (mod & KMOD_SHIFT) move_cursor(0, 1,  0);
			else move_cursor(0,  0, 1);
			return;
		case SDLK_PAGEUP:		move_cursor(0,  0, -4); return;
		case SDLK_PAGEDOWN:		move_cursor(0,  0,  4); return;
		case SDLK_HOME:
			m_cursor_y1 = 0;
			scroll();
			return;
		case SDLK_END:
			m_cursor_y1 = std::max(0, get_max_rows(*m_tune, m_cursor_y0) - 1);
			scroll();
			return;

		case SDLK_BACKSPACE:
			edit(EC::SET_ROW, Row());
			return;

		case '1':			// set 1st macro
		case '2':			// set 2st macro
			if (mod & KMOD_SHIFT) {
				edit(EC::SET_MACRO, key - '1');
				return;
			}
			break;
		case 'I':			// edit m_macro name
			if (row) {
				m_edit_mode = EM_MACRO_NAME;
				m_old_name = row->macros[0];
			}
			return;

		case 'V':			// mark pattern
			m_edit_mode = EM_MARK_PATTERN;
			m_mark_x = m_cursor_x;
			m_mark_y = m_cursor_y1;
			return;

		case 'P':			// paste pattern
			edit(EC::PASTE_REGION);
			return;

		case 'X':			// delete row
			edit(EC::DELETE_ROW);
			return;

		case 'O':			// insert new row
			edit(EC::INSERT_ROW, m_cursor_y1);
			return;

		case 'A':			// append new row
			edit(EC::INSERT_ROW, m_cursor_y1 + 1);
			scroll();
			return;

		case KEY_CTRL_X:	// delete line
			edit(EC::DELETE_LINE);
			return;

		case KEY_CTRL_O:	// insert new line
			edit(EC::INSERT_LINE, m_cursor_y0);
			return;

		case KEY_CTRL_A:	// append new line
			edit(EC::INSERT_LINE, m_cursor_y0 + 1);
			scroll();
			return;

		case KEY_CTRL_R:	// rename pattern
			m_edit_mode = EM_PATTERN_NAME;
			m_rename_pattern = true;
			m_old_name = pat_name;
			return;

		case KEY_CTRL_N:	// new pattern
			m_edit_mode = EM_PATTERN_NAME;
			m_rename_pattern = false;
			m_old_name = pat_name;
			return;

		case KEY_CTRL_D: 	// duplicate above pattern with auto naming
			if (pat) return;
			for (int i = m_cursor_y0; i >= 0; i--) {
				auto pn = m_tune->table[i][m_cursor_x];
				if (pn != "") {
					auto& p = m_tune->patterns[pn];
					auto pos = pn.find_last_not_of("0123456789");
					auto suffix = pn.substr(pos + 1);
					if (suffix == "") suffix = "0";
					else pn.erase(pos + 1);
					while (m_tune->patterns.find(pn + suffix) != m_tune->patterns.end()) {
						auto n = std::to_string(std::stoi(suffix) + 1);
						suffix = std::string(std::max<int>(0, suffix.size() - n.size()), '0') + n;
					}
					pn += suffix;
					m_tune->table[m_cursor_y0][m_cursor_x] = pn;
					m_tune->patterns[pn] = p;
					return;
				}
			}
			return;

		case 'S':
			strip_tune(*m_tune);
			msg_win.say("Saving tune file... ");
			if (!save_tune(*m_tune, m_tunefile)) msg_win.append("error.");
			else msg_win.append("done.");
			return;

		case 'U':
			undo();
			return;
		case 'R':
			redo();
			return;


		default: break;
		}
	}

	if (mod) return;

	if (key == '#') {
		Row row { -1 };
		edit(EC::SET_ROW, row);
		server.play_row(m_cursor_x, row);
	}
}


void PatternWin::note_input(const Row& row) {
	if (row.note > 0 && m_edit_mode == EM_NORMAL) {
		edit(EC::SET_ROW, row);
	}
	else if (m_edit_mode == EM_RECORD) {
		edit(EC::RECORD_ROW, row);
	}
}
