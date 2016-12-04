#include "display.h"
#include "font.h"
#include <vector>


bool Display::init() {
	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_CreateWindowAndRenderer(
			m_width * CHAR_WIDTH,
			m_height * CHAR_HEIGHT,
			0, &m_window, &m_renderer) != 0) {
		return false;
	}
	SDL_SetWindowTitle(m_window, "twobit tracker");

	// create font texture
	m_font = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB4444, SDL_TEXTUREACCESS_STATIC,
			CHAR_WIDTH * 16, CHAR_HEIGHT * 16);
	std::vector< uint16_t > data(CHAR_WIDTH * CHAR_HEIGHT * 256);
	for (int i = 0; i < (int) data.size(); ++i) {
		data[i] = (font[i / 8] & (1 << i % 8)) ? 0xffff : 0;
	}
	SDL_UpdateTexture(m_font, nullptr, data.data(), CHAR_WIDTH * 32);
	SDL_SetTextureBlendMode(m_font, SDL_BLENDMODE_BLEND);

	m_dst.w = m_src.w = CHAR_WIDTH;
	m_dst.h = m_src.h = CHAR_HEIGHT;

	return true;
}


void Display::resize(int w, int h) {
	m_width = w / CHAR_WIDTH;
	m_height = h / CHAR_HEIGHT;
	m_x_offset = w % CHAR_WIDTH / 2;
	m_y_offset = h % CHAR_HEIGHT / 2;
}


void Display::move(int x, int y) {
	m_dst.x = x * CHAR_WIDTH + m_x_offset;
	m_dst.y = y * CHAR_HEIGHT + m_y_offset;
}


void Display::put(char c) {

	if (m_bg != 0x000000) {
		static SDL_Rect rect = { 0, 0, CHAR_WIDTH, CHAR_HEIGHT };
		SDL_SetTextureColorMod(m_font,
				m_bg >> 16 & 255,
				m_bg >> 8 & 255,
				m_bg & 255);
		SDL_RenderCopy(m_renderer, m_font, &rect, &m_dst);
	}

	if (c != 32) {
		m_src.x = c % 16 * CHAR_WIDTH;
		m_src.y = (c / 16 + (m_bold ^ 1) * 8) * CHAR_HEIGHT;
		SDL_SetTextureColorMod(m_font,
				m_fg >> 16 & 255,
				m_fg >> 8 & 255,
				m_fg & 255);
		SDL_RenderCopy(m_renderer, m_font, &m_src, &m_dst);
	}
	m_dst.x += CHAR_WIDTH;
}
