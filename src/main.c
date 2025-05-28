
// file: main.c

#include <stdbool.h>   // bool, true, false (для флагов и логических значений)
#include <stddef.h>    // size_t (для размеров массивов)
#include <stdio.h>     // printf, fprintf, stderr (вывод/ввод)
#include <stdlib.h>    // EXIT_SUCCESS, EXIT_FAILURE (коды завершения)
#include <time.h>      // nanosleep, struct timespec (задержки между кадрами)
#include <unistd.h>    // read (чтение ввода)

#include "terminal.h"  // функции работы с терминалом
#include "tiles.h"     // функции работы с тайлами

#define FPS 30
#define FRAME_TIME_MS (1000000000 / FPS)

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

  if (!tilemap_create(&map, map_cols, map_rows, map_template)) {
    fprintf(stderr, "Failed to create tilemap\n");
    return EXIT_FAILURE;
  }

  struct timespec frame_time = {
    .tv_sec = 0,
    .tv_nsec = FRAME_TIME_MS,
  };

  setup_signal_handlers();
  enable_raw_mode();
  cursor_hide();

  while (true) {

    clear_screen();
    cursor_reset();
    //tilemap_update(&map);
    tilemap_render(&map);
    flush_terminal();
    nanosleep(&frame_time, NULL);

    // Проверка на выход (ESC)
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1 && c == 27) {
      break;
    }

  }

  disable_raw_mode();
  clear_screen();
  cursor_reset();
  cursor_show();

  tilemap_free(&map);

  return EXIT_SUCCESS;
}
