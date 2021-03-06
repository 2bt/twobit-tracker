#pragma once

#include <SDL2/SDL.h>



// special characters
enum {
	CURSOR,
	ULCORNER,
	URCORNER,
	LLCORNER,
	LRCORNER,
	HLINE,
	VLINE,
	PLUS,
	TTEE,
	LTEE,
	BTEE,
	RTEE,
	LEVELLOW,
	LEVELHEIGH,
};



class Display {
public:

	bool init();

	~Display() {
		SDL_DestroyTexture(m_font);
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	void resize(int w, int h);

	void  present() const {
		SDL_RenderPresent(m_renderer);
	}

	void clear() const {
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_renderer);
	}

	void style(uint32_t fg, uint32_t bg, bool bold=true) {
		m_fg = fg;
		m_bg = bg;
		m_bold = bold;
	}
	void fg(uint32_t fg) { m_fg = fg; }
	void bg(uint32_t bg) { m_bg = bg; }
	void bold(bool bold) { m_bold = bold; }

	void move(int x, int y);

	void put(char c);

	void put(char c, int n) {
		while (n--) put(c);
	}

	void print(const char* s) {
		while (*s) put(*s++);
	}


	void vprintf(const char* format, va_list args) {
		char line[256];
		vsnprintf(line, 256, format, args);
		print(line);
	}

	void printf(const char* format, ...) {
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}

	void mvprint(int x, int y, const char* s) {
		move(x, y);
		print(s);
	}

	void mvprintf(int x, int y, const char* format, ...) {
		move(x, y);
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}

	int width() const { return m_width; }
	int height() const { return m_height; }


private:

	int		m_width		= 100;
	int		m_height	= 50;
	int		m_x_offset	= 0;
	int		m_y_offset	= 0;

	int		m_fg;
	int		m_bg;
	bool	m_bold;

	SDL_Rect m_dst;
	SDL_Rect m_src;

	SDL_Window*		m_window	= nullptr;
	SDL_Renderer*	m_renderer	= nullptr;
	SDL_Texture*	m_font		= nullptr;
};


extern Display display;
