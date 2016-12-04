#include <SDL2/SDL_image.h>
#include "display.h"


bool Display::init() {
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

void Display::resize(int w, int h) {
	m_width = w / char_width;
	m_height = h / char_height;
	m_x_offset = w % char_width / 2;
	m_y_offset = h % char_height / 2;
}


void Display::put(char c) {


	if (m_bg != 0x000000) {
		static SDL_Rect rect = { 0, 0, char_width, char_height };
		SDL_SetTextureColorMod(m_font,
				m_bg >> 16 & 255,
				m_bg >> 8 & 255,
				m_bg & 255);
		SDL_RenderCopy(m_renderer, m_font, &rect, &m_dst);
	}

	if (c != 32) {
		m_src.x = c % 16 * char_width;
		m_src.y = (c / 16 + (m_bold ^ 1) * 8) * char_height;
		SDL_SetTextureColorMod(m_font,
				m_fg >> 16 & 255,
				m_fg >> 8 & 255,
				m_fg & 255);
		SDL_RenderCopy(m_renderer, m_font, &m_src, &m_dst);
	}
	m_dst.x += char_width;
}
