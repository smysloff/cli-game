
// file: tiles.c

#include "tiles.h"

static size_t tile_id_counter = 0;

static const TileSymbol TILE_SYMBOLS[TILE_TYPE_COUNT] = {
  [EMPTY_TILE] = '?',
  [GRASS_TILE] = ';',
  [STONE_TILE] = 'o',
  [WOOD_TILE]  = '=',
  [SAND_TILE]  = '*',
  [WATER_TILE] = '~',
};

TileSymbol tile_get_symbol(TileType type) {
  if (type >= EMPTY_TILE && type < TILE_TYPE_COUNT) {
    return TILE_SYMBOLS[type];
  }
  return TILE_SYMBOLS[EMPTY_TILE];
}

TileType tile_get_type(TileSymbol symbol) {
  for (size_t i = 0; i < TILE_TYPE_COUNT; ++i) {
    if (TILE_SYMBOLS[i] == symbol) {
      return (TileType)i;
    }
  }
  return EMPTY_TILE;
}

Tile tile_create(TileType type) {
  return (Tile) {
    .id = ++tile_id_counter,
    .type = type,
  };
}

bool tilemap_create(
  TileMap* map,
  size_t cols,
  size_t rows,
  const TileSymbol* map_template
) {
  if (!map || cols == 0 || rows == 0 || !map_template) {
    return false;
  }

  const size_t size = cols * rows;

  Tile* tiles = malloc(sizeof(Tile) * size);
  if (!tiles) {
    return false;
  }

  for (size_t i = 0; i < size; ++i) {
    const TileSymbol symbol = map_template[i];
    const TileType type = tile_get_type(symbol);
    tiles[i] = tile_create(type);
  }

  map->cols = cols;
  map->rows = rows;
  map->tiles = tiles;

  return true;
}

void tilemap_free(TileMap *map) {
  if (map) {
    map->cols = 0;
    map->rows = 0;
    free(map->tiles);
    map->tiles = NULL;
  }
}

Tile* tilemap_get_tile(const TileMap *map, size_t col, size_t row) {
  if (!map || !map->tiles || col >= map->cols || row >= map->rows) {
    return NULL;
  }
  return &map->tiles[row * map->cols + col];
}

void tilemap_render(const TileMap* map) {

  if (!map || !map->tiles) {
    return;
  }

  for (size_t row = 0; row < map->rows; ++row) {
    for (size_t col = 0; col < map->cols; ++col) {
      const Tile* tile = tilemap_get_tile(map, col, row);
      if (tile) {
        // для анимированных тайлов можно добавить эффекты
        //if (tile->animated) {
        //  printf("\033[38;5;%dm",
        //    16 + (int)(tile->animation_progress * 5) % 216);
        //}
        putchar(tile_get_symbol(tile->type));
        printf("\033[0m"); // сброс цвета
      }
    }

    if (row < map->rows - 1) {
      cursor_next_line(1); // @todo ?
    }
  }

}

// Функция обновления анимаций
void tilemap_update(TileMap* map) {
  if (!map || !map->tiles) return;

  const size_t size = map->cols * map->rows;
  for (size_t i = 0; i < size; ++i) {
    if (map->tiles[i].animated) {
      map->tiles[i].animation_progress += 0.1f;
      if (map->tiles[i].animation_progress > 1.0f) {
        map->tiles[i].animation_progress = 0.0f;
      }
    }
  }
}
