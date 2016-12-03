#pragma once

#include <SDL2/SDL.h>



enum EStyle {
	S_DEFAULT,
	S_NORMAL,
	S_HL_NORMAL,

	S_PATTERN,
	S_HL_PATTERN,
	S_CS_PATTERN,
	S_PL_PATTERN,
	S_ET_PATTERN,
	S_RC_PATTERN,

	S_NOTE,
	S_MACRO,
	S_HL_NOTE,
	S_HL_MACRO,
	S_CS_NOTE,
	S_CS_MACRO,
	S_PL_NOTE,
	S_PL_MACRO,
	S_ET_NOTE,
	S_ET_MACRO,
	S_MK_NOTE,
	S_MK_MACRO,
	S_RC_NOTE,
	S_RC_MACRO,

	S_FRAME,

	S_LEVEL,
};


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



class Graphics {
public:

	bool init();

	~Graphics() {
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

	void style(int i);
	void style(uint32_t fg, uint32_t bg, bool bold=true);


	void move(int x, int y) {
		m_dst.x = x * char_width + m_x_offset;
		m_dst.y = y * char_height + m_y_offset;
	}

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

	int get_width() const { return m_width; }
	int get_height() const { return m_height; }


protected:
//	void fill_rect(int x, int y, int w, int h) const {
//		SDL_Rect rect = { x, y, w, h };
//		SDL_RenderFillRect(m_renderer, &rect);
//	}

	void set_text_color(uint8_t r, uint8_t g, uint8_t b) const {
		SDL_SetTextureColorMod(m_font, r, g, b);
	}



private:

	const int char_width	= 8;
	const int char_height	= 12;

	int m_width		= 100;
	int m_height	= 50;

	int m_x_offset = 0;
	int m_y_offset = 0;

	SDL_Rect m_dst = { 0, 0, char_width, char_height };
	SDL_Rect m_src = { 0, 0, char_width, char_height };

	SDL_Window*		m_window	= nullptr;
	SDL_Renderer*	m_renderer	= nullptr;
	SDL_Texture*	m_font		= nullptr;
};


extern Graphics graphics;
