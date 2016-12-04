#include "messagewin.h"
#include "display.h"

void MessageWin::resize() {
	m_width = display.get_width() - m_left;
	m_top = display.get_height() - MAX_MESSAGES;
}

void MessageWin::draw() {
	for (int y = 0; y < m_height; y++) {
		display.move(m_left, m_top + y);
		if (y < (int) m_messages.size()) {
			display.printf("%-*s", m_width, m_messages[y].text.c_str());
		}
		else {
			display.printf("%-*s", m_width, "");
		}
	}
	auto now = std::chrono::system_clock::now();
	while (!m_messages.empty() && m_messages.front().time + std::chrono::seconds(3) < now) {
		m_messages.erase(m_messages.begin());
	}
}

void MessageWin::append(const char* format, ...) {
	char line[256];
	va_list a;
	va_start(a, format);
	vsnprintf(line, 256, format, a);
	va_end(a);
	m_messages.back().text += line;
}

void MessageWin::say(const char* format, ...) {
	char line[256];
	va_list a;
	va_start(a, format);
	vsnprintf(line, 256, format, a);
	va_end(a);
	m_messages.push_back({ line, std::chrono::system_clock::now() });
	while (m_messages.size() > MAX_MESSAGES) m_messages.erase(m_messages.begin());
}
