#pragma once

#include <vector>
#include <string>
#include <chrono>
#include "graphics.h"
#include "win.h"


class MessageWin : public Win {
public:
	enum { MAX_MESSAGES = 2 };

	MessageWin() {
		resize();
	}

	virtual void resize() {
		m_width = graphics.get_width() - m_left;
		m_top = graphics.get_height() - MAX_MESSAGES;
	}

	virtual void draw();
	void append(const char* format, ...);
	void say(const char* format, ...);

private:

	int m_top;
	int m_left = 9;
	int m_width;
	int m_height = MAX_MESSAGES;

	struct Msg {
		std::string text;
		std::chrono::time_point<std::chrono::system_clock> time;
	};
	std::vector<Msg> m_messages;
};

extern MessageWin msg_win;
