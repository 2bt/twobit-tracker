#pragma once

#include "tune.h"

class PatternWin;

class EditCommand {
public:
	enum Execution { ECE_DO, ECE_UNDO, ECE_REDO };
	enum Type {
		SET_NOTE,
		SET_MACRO,
		SET_ROW,
		RECORD_ROW,
		YANK_REGION,
		PASTE_REGION,
		TRANSPOSE_REGION,
		DELETE_ROW,
		INSERT_ROW,
		DELETE_LINE,
		INSERT_LINE,
		// TODO:
		// pattern renaming
	};


	template <Type type>
	void init() {
		m_type = type;
	}

	template <Type type>
	void init(const Row& row) {
		m_type = type;
		m_row = row;
	}

	template <Type type>
	void init(bool clear) {
		m_type = type;
		m_clear = clear;
	}

	template <Type type>
	void init(int index) {
		m_type = type;
		m_index = index;
	}

	bool exec(PatternWin& win, Execution e=ECE_DO);

private:
	void restore_cursor(PatternWin& win) const;

	Type	m_type;
	int		m_cursor_x;
	int		m_cursor_y0;
	int		m_cursor_y1;
	Row		m_row;
	Row		m_prev_row;
	int		m_index;
	int		m_clear;
	std::vector<Pattern>	m_region;
	std::vector<Pattern>	m_prev_region;
	std::vector<int>		m_length_diffs;
	TableLine				m_prev_line;
};


