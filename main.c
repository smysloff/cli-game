
// file: main.c

#include <signal.h>
#include <stdbool.h> // true, false, bool
#include <stddef.h>  // size_t
#include <stdio.h>   // putchar, printf, fprintf
#include <stdlib.h>  // malloc, free
#include <termios.h>
#include <time.h>    // nanosleep, struct timespec
#include <unistd.h>  // usleep

#include "./termios.c"
#include "./tiles.c"

static size_t map_cols = 8;
static size_t map_rows = 8;

static TileSymbol map_template[] =
  "oooooooo"
  "o;o;;;;o"
  "o;;;;o;o"
  "o;;;;o;o"
  "o;;;;;;o"
  "o;;;oooo"
  "oo;;;;;o"
  "oooooooo";

int main(void)
{
  TileMap map;

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  if (!tilemap_create(&map, map_cols, map_rows, map_template)) {
    fprintf(stderr, "Failed to create tilemap\n");
    return EXIT_FAILURE;
  }

  struct timespec frame_time = {
    .tv_sec = 0,
    .tv_nsec = FRAME_TIME_MS,
  };

  hide_cursor();
  enable_raw_mode();

  while (true) {

    clear_screen();
    //tilemap_update(&map);
    tilemap_render(&map);
    nanosleep(&frame_time, NULL);

    // Проверка на выход (ESC)
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1 && c == 27) {
      break;
    }

  }

  clear_screen();
  tilemap_free(&map);
  disable_raw_mode();
  refresh_cursor();
  show_cursor();

  return EXIT_SUCCESS;
}
