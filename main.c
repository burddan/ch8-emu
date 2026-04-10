#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ch8.h"
#include "config.h"
#include "sdl.h"

static void usage(const char *prog) {
	fprintf(stderr,
		"usage: %s [options] <rom.ch8>\n"
		"\n"
		"options:\n"
		"  -q, --quirks <mode>   original (default) or modern\n"
		"  -c, --color  <name>   default, blue, matrix, warm\n"
		"  -h, --help\n",
		prog);
}

static void apply_quirks(Chip8State *cpu, const char *mode) {
	if (strcmp(mode, "modern") == 0) {
		cpu->quirk_shift      = true;
		cpu->quirk_load_store = true;
		cpu->quirk_jump       = false;
	}

	else if (strcmp(mode, "original") == 0) {
		cpu->quirk_shift      = false;
		cpu->quirk_load_store = false;
		cpu->quirk_jump       = false;
	}

	else {
		fprintf(stderr, "error: unknown quirks mode '%s' (use: original, modern)\n", mode);
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	Chip8State cpu;
	SdlContext sdl;

	const char        *color_name = NULL;
	const char        *quirks     = NULL;

	static struct option long_opts[] = {
		{ "quirks", required_argument, NULL, 'q' },
		{ "color",  required_argument, NULL, 'c' },
		{ "help",   no_argument,       NULL, 'h' },
		{ NULL, 0, NULL, 0 }
	};

	int opt;
	while ((opt = getopt_long(argc, argv, "q:c:h", long_opts, NULL)) != -1) {
		switch (opt) {
		case 'q': quirks     = optarg; break;
		case 'c': color_name = optarg; break;
		case 'h': usage(argv[0]); return 0;
		default:  usage(argv[0]); return 1;
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "error: no ROM file specified\n");
		usage(argv[0]);
		return 1;
	}

	const char *rom = argv[optind];

	chip8_init(&cpu);

	if (quirks)
		apply_quirks(&cpu, quirks);

	Colorscheme colors = get_colorscheme(color_name);

	if (!chip8_load_rom(&cpu, rom)) {
		fprintf(stderr, "error: failed to open ROM file '%s'\n", rom);
		return 1;
	}

	if (!sdl_init(&sdl)) {
		fprintf(stderr, "error: failed to initialize SDL\n");
		return 1;
	}

	const double ms_per_cycle = 1000.0 / CPU_CLOCK_HZ;
	const double ms_per_timer = 1000.0 / TIMER_CLOCK_HZ;
	double cycle_accumulator  = 0.0;
	double timer_accumulator  = 0.0;
	uint64_t last_time_ms     = SDL_GetTicks();

	while (sdl.running && cpu.running) {
		sdl_handle_events(&sdl, cpu.keypad);

		uint64_t current_time = SDL_GetTicks();
		double delta_ms       = (double)(current_time - last_time_ms);
		last_time_ms          = current_time;

		cycle_accumulator += delta_ms;
		timer_accumulator += delta_ms;

		int cycles_this_frame = 0;
		while (cycle_accumulator >= ms_per_cycle && cycles_this_frame < MAX_CYCLES_PER_TICK) {
			chip8_cycle(&cpu);
			cycle_accumulator -= ms_per_cycle;
			cycles_this_frame++;
		}

		while (timer_accumulator >= ms_per_timer) {
			chip8_update_timers(&cpu);
			sdl_update_audio(&sdl, cpu.sound_timer);
			timer_accumulator -= ms_per_timer;
		}

		if (cpu.needs_draw) {
			sdl_update_display(&sdl, cpu.framebuffer, colors);
			cpu.needs_draw = false;
		}

		SDL_Delay(1);
	}

	sdl_cleanup(&sdl);
	return 0;
}
