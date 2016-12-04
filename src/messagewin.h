#pragma once

#include <vector>
#include <string>
#include <chrono>
#include "win.h"


class MessageWin : public Win {
public:
	enum { MAX_MESSAGES = 2 };

	MessageWin() {
		resize();
	}

	void resize() override;
	void draw() override;

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
