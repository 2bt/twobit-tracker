#pragma once

#include "win.h"
#include "tune.h"
#include "editcommand.h"


class PatternWin : public Win {
	friend class EditCommand;
public:
	enum {
		PATTERN_CHAR_WIDTH = 9,
		MACRO_CHAR_WIDTH = 5,
		CHAN_CHAR_WIDTH = 3 + MACROS_PER_ROW * (MACRO_CHAR_WIDTH + 1),

		POLYPHONY = 5
	};


	void init(Tune* tune, const char* tunefile) {
		for (auto& c : m_note_to_chan) c = -1;
		for (auto& n : m_chan_to_note) n = -1;
		m_tune = tune;
		m_tunefile = tunefile;
		resize();
	}

	void resize() override;
	void draw() override;
	void key(const SDL_Keysym & ks) override;

	void midi(int type, int value);

private:

	int	m_top = 0;
	int	m_left = 0;
	int	m_width;
	int	m_height;

	int	m_cursor_x = 0;
	int	m_cursor_y0 = 0;
	int	m_cursor_y1 = 0;
	int	m_mark_x;
	int	m_mark_y;

	int	m_scroll_x = 0;
	int	m_scroll_y0 = 0;
	int	m_scroll_y1 = 0;
	int	m_scroll_x_view;
	int	m_scroll_y0_view;
	int	m_scroll_y1_view;

	int	mark_y_begin() const { return std::min(m_cursor_y1, m_mark_y); }
	int	mark_y_end() const { return std::max(m_cursor_y1, m_mark_y) + 1; }
	int	mark_x_begin() const { return std::min(m_cursor_x, m_mark_x); }
	int	mark_x_end() const { return std::max(m_cursor_x, m_mark_x) + 1; }

	void scroll();
	void move_cursor(int dx, int dy0, int dy1);

	void key_pattern_name(const SDL_Keysym & ks);
	void key_macro_name(const SDL_Keysym & ks);
	void key_mark_pattern(const SDL_Keysym & ks);
	void key_record(const SDL_Keysym & ks);
	void key_normal(const SDL_Keysym & ks);
	void key_rec_norm_common(const SDL_Keysym & ks);

	enum EditMode { EM_NORMAL, EM_RECORD, EM_PATTERN_NAME, EM_MACRO_NAME, EM_MARK_PATTERN };
	EditMode				m_edit_mode = EM_NORMAL;
	bool					m_rename_pattern;
	std::string				m_old_name;
	std::vector<Pattern>	m_pattern_buffer;
	std::string				m_macro;
	int						m_octave = 3;
	Tune*					m_tune;
	const char*				m_tunefile;

	// midi keyboard state
	int				m_note_to_chan[128];
	int				m_chan_to_note[CHANNEL_COUNT];


	std::array<EditCommand,1024>	m_cmds;
	int m_cmd_head = 0;
	int m_cmd_tail = 0;
	int m_cmd_index = 0;

	template <EditCommand::Type type, typename... Args>
	void edit(Args&&... args) {
		m_cmds[m_cmd_index].init<type>(std::forward<Args>(args)...);

		if (!m_cmds[m_cmd_index].exec(*this)) return;
		m_cmd_index = (m_cmd_index + 1) % m_cmds.size();

		if (m_cmd_head == m_cmd_index) {
			m_cmd_head = (m_cmd_head + 1) % m_cmds.size();
		}
		m_cmd_tail = m_cmd_index;
	}
	void undo() {
		if (m_cmd_index == m_cmd_head) return;
		m_cmd_index = (m_cmd_index + m_cmds.size() - 1) % m_cmds.size();
		m_cmds[m_cmd_index].exec(*this, EditCommand::ECE_UNDO);
	}
	void redo() {
		if (m_cmd_index == m_cmd_tail) return;
		m_cmds[m_cmd_index].exec(*this, EditCommand::ECE_REDO);
		m_cmd_index = (m_cmd_index + 1) % m_cmds.size();
	}

};
