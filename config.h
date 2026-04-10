#ifndef CHIP8_CONFIG_H
#define CHIP8_CONFIG_H

#include <stdint.h>
#include <string.h>

#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

#define AUDIO_SAMPLE_RATE 44100
#define AUDIO_BEEP_HZ     440
#define AUDIO_VOLUME      3000

#define CPU_CLOCK_HZ        500
#define TIMER_CLOCK_HZ      60
#define MAX_CYCLES_PER_TICK 10

#define MEMORY_SIZE 4096

/* 
 * The SDL pixel format used is SDL_PIXELFORMAT_XRGB8888, each color is represented as a 32-bit value.
 * The first byte is ignored, and it only exists for alignment padding.
 * All other 3 bytes are for the 3 main color channels(red, green and blue).
 * For example, 0x00FF0000 would be red, 0x0000FF00 green, 0x00FFFFFF white, and so on...
 */
#define COLOR_FG_DEFAULT 0x00FFFFFF
#define COLOR_BG_DEFAULT 0x00000000

#define COLOR_FG_BLUE    0x00FFFFFF
#define COLOR_BG_BLUE    0x000000AA

#define COLOR_FG_MATRIX  0x0000FF00
#define COLOR_BG_MATRIX  0x00000000

#define COLOR_FG_WARM    0x00FF8000
#define COLOR_BG_WARM    0x00402000

typedef struct{
	uint32_t fg;
	uint32_t bg;
} Colorscheme;

static inline Colorscheme get_colorscheme(const char *name) {
	if (name != NULL) {
		if (strcmp(name, "blue")   == 0) return (Colorscheme) { COLOR_FG_BLUE,   COLOR_BG_BLUE   };
		if (strcmp(name, "matrix") == 0) return (Colorscheme) { COLOR_FG_MATRIX, COLOR_BG_MATRIX };
		if (strcmp(name, "warm")   == 0) return (Colorscheme) { COLOR_FG_WARM,   COLOR_BG_WARM   };
	}

	return (Colorscheme){ COLOR_FG_DEFAULT, COLOR_BG_DEFAULT };
}

#endif
