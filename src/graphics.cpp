#include <SDL2/SDL_image.h>
#include "graphics.h"


bool Graphics::init() {
	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_CreateWindowAndRenderer(
			m_width * char_width,
			m_height * char_height,
			0, &m_window, &m_renderer) != 0) {
		return false;
	}
	SDL_SetWindowTitle(m_window, "twobit tracker");

	SDL_Surface* img = IMG_Load("font.png");
	m_font = SDL_CreateTextureFromSurface(m_renderer, img);
	SDL_FreeSurface(img);
	return true;
}

void Graphics::resize(int w, int h) {
	m_width = w / char_width;
	m_height = h / char_height;
	m_x_offset = w % char_width / 2;
	m_y_offset = h % char_height / 2;
}

struct Style { uint32_t fg, bg; bool bold; };

static const Style styles[] {
	{},
	{	0x999999,	0x000000,	false	},
	{	0xffffff,	0x002200,	false	},

	{	0x999999,	0x000000,	false	},
	{	0x999999,	0x002200,	false	},
	{	0x999999,	0x005500,	false	},
	{	0x999999,	0x002233,	false	},
	{	0x999999,	0xff0000,	false	},
	{	0x999999,	0x550000,	false	},

	{	0xffffff,	0x000000,	true	},
	{	0x999999,	0x000000,	false	},
	{	0xffffff,	0x002200,	true	},
	{	0x999999,	0x002200,	false	},
	{	0xffffff,	0x005500,	true	},
	{	0x999999,	0x005500,	false	},
	{	0xffffff,	0x002233,	true	},
	{	0x999999,	0x002233,	false	},
	{	0xffffff,	0xff0000,   true	},
	{	0x999999,	0xff0000,   false	},
	{	0xffffff,	0xff0000,   true	},
	{	0x999999,	0xff0000,   false	},
	{	0xffffff,	0x550000,	true	},
	{	0x999999,	0x550000,	false	},

	{	0x007700,	0x000000,	true	},


	{	0xffffff,	0x005500,	false	},

};

static Style s;

void Graphics::style(int i) {
	s = styles[i];
}

void Graphics::style(uint32_t fg, uint32_t bg, bool bold) {
	s.fg = fg;
	s.bg = bg;
	s.bold = bold;
}

void Graphics::put(char c) {


	if (s.bg != 0x000000) {
		static SDL_Rect rect = { 0, 0, char_width, char_height };
		SDL_SetTextureColorMod(m_font,
				s.bg >> 16 & 255,
				s.bg >> 8 & 255,
				s.bg & 255);
		SDL_RenderCopy(m_renderer, m_font, &rect, &m_dst);
	}

	if (c != 32) {
		m_src.x = c % 16 * char_width;
		m_src.y = (c / 16 + (s.bold ^ 1) * 8) * char_height;
		SDL_SetTextureColorMod(m_font,
				s.fg >> 16 & 255,
				s.fg >> 8 & 255,
				s.fg & 255);
		SDL_RenderCopy(m_renderer, m_font, &m_src, &m_dst);
	}
	m_dst.x += char_width;
}

