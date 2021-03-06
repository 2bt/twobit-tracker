#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <uv.h>
#include <set>

#include "server.h"
#include "display.h"
#include "tune.h"
#include "messagewin.h"
#include "patternwin.h"
#include "keyboard.h"


static Tune tune = { {}, {}, {}, 800, 8, };

Display		display;
PatternWin	pat_win;
MessageWin	msg_win;
Keyboard	keyboard;
Server		server;



const char* watch;
void reload(uv_fs_event_t* handle, const char* name, int events, int status) {
	if (events & UV_CHANGE) {
		uv_fs_event_stop(handle);
		msg_win.say("Reloading tune watch file... ");
		if (!load_tune(tune, watch)) msg_win.append("error.");
		else msg_win.append("done.");
		uv_fs_event_start(handle, &reload, watch, 0);
	}
}


void usage(const char* prg) {
	printf("Usage: %s [options] tune.x [tune.y]\n", prg);
	printf("  -w         Write entire tune to log.wav and quit.\n");
	printf("  -r reps    Repeate tune. Implies -w.\n");
	printf("  -s nr      Choose subtune. Implies -w.\n");
}


int main(int argc, char** argv) {
	bool write_tune = false;
	int write_reps = 1;
	int write_subtune = 0;

	int opt;
	while ((opt = getopt(argc, argv, "wr:s:")) != -1) {
		switch (opt) {
		case 'w':
			write_tune = true;
			break;
		case 'r':
			write_tune = true;
			write_reps = atoi(optarg);
			break;
		case 's':
			write_tune = true;
			write_subtune = atoi(optarg);
			break;
		default:
			usage(argv[0]);
			return 1;
		}
	}
	int opts_left = argc - optind;
	if (opts_left < 1 || opts_left > 2) {
		usage(argv[0]);
		return opts_left != 0;
	}

	const char* filename = argv[optind];
	if (opts_left == 2) watch = argv[optind + 1];


	if (!load_tune(tune, filename)) {
		if (write_tune) {
			printf("Error loading tune file\n");
			return 1;
		}
		msg_win.say("Error loading tune file");
	}

	uv_loop_t* loop = uv_default_loop();
	uv_fs_event_t handle;
	if (watch) {
		if (!load_tune(tune, watch)) {
			if (write_tune) {
				printf("Error loading tune watch file\n");
				return 1;
			}
			msg_win.say("Error loading tune watch file");
		}
		uv_fs_event_init(loop, &handle);
		uv_fs_event_start(&handle, &reload, watch, 0);
	}

	if (tune.table.empty()) tune.table.resize(1);

	server.init(&tune);
	if (write_tune) {
		printf("Writing tune...\n");
		server.generate_full_log(write_subtune, write_reps);
		return 0;
	}


	// become interactive

	display.init();
	pat_win.init(&tune, filename);
	msg_win.resize();
	keyboard.init();
	server.start();


	std::set<int> keys = {
		SDLK_UP,
		SDLK_DOWN,
		SDLK_LEFT,
		SDLK_RIGHT,
		SDLK_RETURN,
		SDLK_PAGEUP,
		SDLK_PAGEDOWN,
		SDLK_BACKSPACE,
		SDLK_ESCAPE,
	};
	const Uint8* key_state = SDL_GetKeyboardState(NULL);


	bool running = true;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {

			case SDL_QUIT:
				running = false;
				break;

			case SDL_WINDOWEVENT:
				switch (e.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					display.resize(e.window.data1, e.window.data2);
					pat_win.resize();
					msg_win.resize();
					break;
				default: break;
				}
				break;

			case SDL_TEXTINPUT:
				if (strlen(e.text.text) == 1) {
					pat_win.key(
						e.text.text[0],
						key_state[SDL_SCANCODE_LCTRL] | key_state[SDL_SCANCODE_RCTRL] ? KMOD_CTRL : 0);
				}
				break;

			case SDL_KEYDOWN:
				if (!e.key.keysym.mod && !e.key.repeat && pat_win.keyboard_enabled()) {
					if (keyboard.event(e.key.keysym.scancode, true)) break;
				}
				if (keys.count(e.key.keysym.sym)) {
					pat_win.key(e.key.keysym.sym, e.key.keysym.mod);
				}
				break;


			case SDL_KEYUP:
				if (e.key.keysym.mod == 0 && pat_win.keyboard_enabled()) {
					keyboard.event(e.key.keysym.scancode, false);
				}
				break;

			default: break;
			}
		}

		uv_run(loop, UV_RUN_NOWAIT);

		display.clear();

		msg_win.draw();
		pat_win.draw();

		display.present();
		SDL_Delay(5);
	}

	uv_loop_close(loop);
}


