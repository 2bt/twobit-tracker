#include "editcommand.h"
#include "patternwin.h"
#include "server.h"
#include "messagewin.h"


bool operator==(const Row& a, const Row& b) {
	return a.note == b.note && a.macros == b.macros;
}
bool operator!=(const Row& a, const Row& b) { return !(a == b); }

void EditCommand::restore_cursor(PatternWin& win) const {
	win.m_cursor_y0 = m_cursor_y0;
	win.m_cursor_x = m_cursor_x;
	if (m_type != DELETE_LINE && m_type != INSERT_LINE) {
		win.m_cursor_y1 = m_cursor_y1;
	}
	win.scroll();
}

bool paste_region(Tune& tune, int line_nr, int x, int y,
				  const std::vector<Pattern>& src, std::vector<int>* length_diffs=nullptr)
{
	if (length_diffs) length_diffs->resize(src.size());
	bool same = true;
	auto& line = tune.table[line_nr];
	for (int c = 0; c < (int) src.size(); c++) {
		auto it = tune.patterns.find(line[(x + c) % CHANNEL_COUNT]);
		if (it != tune.patterns.end()) {
			auto& pat = it->second;
			auto& buffer = src[c];

			if (length_diffs) length_diffs->at(c) = 0;
			if (pat.size() < y + buffer.size()) {
				if (length_diffs) length_diffs->at(c) = y + buffer.size() - pat.size();
				pat.resize(y + buffer.size());
				same = false;
			}
			for (int i = 0; i < (int) buffer.size(); i++) {
				if (pat[y + i] != buffer[i]) {
					pat[y + i] = buffer[i];
					same = false;
				}
			}
		}
	}
	return same;
}


bool yank_region(Tune& tune, int line_nr, int x0, int y0, int x1, int y1,
				 std::vector<Pattern>& dst, bool clear=false)
{
	int same = true;
	auto& line = tune.table[line_nr];
	dst.resize(x1 - x0);
	for (int c = x0; c < x1; c++) {
		auto& buffer = dst[c - x0];
		buffer.clear();
		auto it = tune.patterns.find(line[c]);
		if (it != tune.patterns.end()) {
			auto& pat = it->second;
			for (int i = y0; i < y1 && i < (int) pat.size(); i++) {
				buffer.push_back(pat[i]);
				if (clear) {
					if (pat[i] != Row()) {
						pat[i] = Row();
						same = false;
					}
				}
			}
		}
	}
	return same;
}


bool EditCommand::exec(PatternWin& win, Execution e) {
	if (e == ECE_DO) {
		if (m_type == RECORD_ROW) {
			server.get_nearest_row(m_cursor_y0, m_cursor_y1);
			m_cursor_x = win.m_cursor_x;
		}
		else if (m_type == YANK_REGION || m_type == TRANSPOSE_REGION) {
			m_cursor_x	= win.mark_x_begin();
			m_cursor_y0	= win.m_cursor_y0;
			m_cursor_y1	= win.mark_y_begin();
		}
		else {
			m_cursor_x	= win.m_cursor_x;
			m_cursor_y0	= win.m_cursor_y0;
			m_cursor_y1	= win.m_cursor_y1;
		}

	}


	auto& table = win.m_tune->table;
	auto& patterns = win.m_tune->patterns;
	auto& line = table[m_cursor_y0];
	auto& pat_name = line[m_cursor_x];
	auto pat = patterns.count(pat_name) ? &patterns[pat_name] : nullptr;
	auto row = (pat && m_cursor_y1 < (int) pat->size()) ? &pat->at(m_cursor_y1) : nullptr;


	switch (m_type) {
	case SET_ROW:
	case RECORD_ROW:
		if (!row) return false;
		if (e == ECE_DO) m_prev_row = *row;
		else restore_cursor(win);
		if (e != ECE_UNDO) {
			if (*row == m_row) return false;
			*row = m_row;
		}
		else *row = m_prev_row;
		return true;


	case SET_MACRO:
		if (!row) return false;
		if (e == ECE_DO) {
			m_prev_row = *row;
			m_row = *row;
			m_row.macros[m_index] = win.m_macro;
		}
		else restore_cursor(win);
		if (e != ECE_UNDO) {
			if (*row == m_row) return false;
			*row = m_row;
		}
		else *row = m_prev_row;
		return true;


	case DELETE_ROW:
		if (e == ECE_DO) {
			if (!pat || pat->size() <= 1) return false;
//			m_cursor_y1 = win.m_cursor_y1 = m_index;
			if (m_cursor_y1 >= (int) pat->size() - 1) {
				m_cursor_y1 = win.m_cursor_y1 = pat->size() - 1;
			}
			m_prev_row = pat->at(m_cursor_y1);
		}
		else restore_cursor(win);
		if (e != ECE_UNDO) {
			pat->erase(pat->begin() + m_cursor_y1);
			if (m_cursor_y1 > (int) pat->size() - 1) win.move_cursor(0, 0, -1);
		}
		else pat->insert(pat->begin() + m_cursor_y1, m_prev_row);
		return true;


	case INSERT_ROW:
		if (!pat) return false;
		if (e == ECE_DO) {
			m_cursor_y1 = win.m_cursor_y1 = m_index;
			if (m_cursor_y1 >= (int) pat->size()) {
				m_cursor_y1 = win.m_cursor_y1 = pat->size();
			}
		}
		else restore_cursor(win);
		if (e != ECE_UNDO) {
			pat->insert(pat->begin() + m_cursor_y1, Row());
		}
		else {
			pat->erase(pat->begin() + m_cursor_y1);
			if (m_cursor_y1 > (int) pat->size() - 1) win.move_cursor(0, 0, -1);
		}
		return true;



	case YANK_REGION:
		if (e == ECE_DO) {
			int xe = win.mark_x_end();
			int ye = win.mark_y_end();
			if (yank_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, xe, ye,
				win.m_pattern_buffer, m_clear)) return false;
			m_prev_region = win.m_pattern_buffer;
			yank_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, xe, ye, m_region);
		}
		else restore_cursor(win);
		if (e == ECE_REDO) paste_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, m_region);
		else if (e == ECE_UNDO) paste_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, m_prev_region);
		return true;


	case PASTE_REGION:
		if (e == ECE_DO) {
			m_region = win.m_pattern_buffer;
			int len = 0;
			for (auto& b : m_region) len = std::max(len, (int) b.size());
			yank_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, m_cursor_x + m_region.size(),
				m_cursor_y1 + len, m_prev_region);
			if (paste_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, m_region, &m_length_diffs)) {
				return false;
			}
		}
		else restore_cursor(win);
		if (e == ECE_REDO) paste_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, m_region);
		else if (e == ECE_UNDO) {
			paste_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, m_prev_region);
			// trim patterns
			for (int c = 0; c < (int) m_region.size(); c++) {
				auto it = patterns.find(line[m_cursor_x + c]);
				if (it != patterns.end()) {
					auto& pat = it->second;
					pat.resize(pat.size() - m_length_diffs[c]);
				}
			}
		}
		return true;

	case TRANSPOSE_REGION:
		if (e == ECE_DO) {
			int xe = win.mark_x_end();
			int ye = win.mark_y_end();
			yank_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, xe, ye, m_prev_region);
			for (int c = m_cursor_x; c < xe; c++) {
				auto it = patterns.find(line[c]);
				if (it != patterns.end()) {
					auto& pat = it->second;
					for (int i = m_cursor_y1; i < ye && i < (int) pat.size(); i++) {
						auto& row = pat[i];
						if (row.note <= 0) continue;
						row.note = clamp(row.note + m_index, 1, 120);
					}
				}
			}
			yank_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, xe, ye, m_region);
			if (m_region == m_prev_region) return false;
		}
		else restore_cursor(win);
		if (e == ECE_REDO) paste_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, m_region);
		else if (e == ECE_UNDO) paste_region(*win.m_tune, m_cursor_y0, m_cursor_x, m_cursor_y1, m_prev_region);
		return true;


	case DELETE_LINE:
		if (e == ECE_DO) {
			if (table.size() <= 1) return false;
			if (m_cursor_y0 >= (int) table.size() - 1) {
				m_cursor_y0 = win.m_cursor_y0 = table.size() - 1;
			}
			m_prev_line = line;
		}
		else restore_cursor(win);
		if (e != ECE_UNDO) {
			table.erase(table.begin() + m_cursor_y0);
			if (m_cursor_y0 > (int) table.size() - 1) win.move_cursor(0, -1, 0);
		}
		else table.insert(table.begin() + m_cursor_y0, m_prev_line);
		return true;


	case INSERT_LINE:
		if (e == ECE_DO) {
			m_cursor_y0 = win.m_cursor_y0 = m_index;
			if (m_cursor_y0 >= (int) table.size()) {
				m_cursor_y0 = win.m_cursor_y0 = table.size();
			}
		}
		else restore_cursor(win);
		if (e != ECE_UNDO) {
			TableLine l;
			table.insert(table.begin() + m_cursor_y0, l);
		}
		else {
			table.erase(table.begin() + m_cursor_y0);
			if (m_cursor_y0 > (int) table.size() - 1) win.move_cursor(0, -1, 0);
		}
		return true;


	default:
		msg_win.say("Invalid command");
		return false;
	}
}
