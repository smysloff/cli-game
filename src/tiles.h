
/**
 * @file tiles.h
 * @brief Модуль для работы с тайлами и тайловыми картами.
 *
 * Предоставляет:
 * - Типы тайлов (TileType) и их символьное представление (TileSymbol).
 * - Функции для создания/управления тайлами.
 * - Функции для работы с тайловыми картами (TileMap).
 * - Рендеринг и обновление анимаций тайлов.
 */

#pragma once

#include <stddef.h>    // size_t (для cols/rows в TileMap)
#include <stdlib.h>    // free (освобождение памяти в tilemap_free)

#include "terminal.h"  // cursor_next_line

/**
 * @brief Типы тайлов.
 */
typedef enum TileType {
  EMPTY_TILE = 0,  ///< Пустой тайл (символ '?').
  GRASS_TILE = 1,  ///< Трава (символ ';').
  STONE_TILE = 2,  ///< Камень (символ 'o').
  WOOD_TILE  = 3,  ///< Дерево (символ '=').
  SAND_TILE  = 4,  ///< Песок (символ '*').
  WATER_TILE = 5,  ///< Вода (символ '~').
  TILE_TYPE_COUNT  ///< Количество типов тайлов (для валидации).
} TileType;

typedef char TileSymbol;  ///< Символьное представление тайла.

/**
 * @brief Структура тайла.
 */
typedef struct Tile {
  size_t id;               ///< Уникальный идентификатор тайла.
  TileType type;           ///< Тип тайла (см. TileType).
  bool animated;           ///< Флаг анимации тайла.
  float animation_progress; ///< Прогресс анимации (0.0–1.0).
} Tile;

/**
 * @brief Структура тайловой карты.
 */
typedef struct TileMap {
  size_t cols;  ///< Количество колонок.
  size_t rows;  ///< Количество строк.
  Tile* tiles;  ///< Массив тайлов (размер: cols × rows).
} TileMap;

/* Функции для работы с тайлами */

/**
 * @brief Возвращает символ для указанного типа тайла.
 * @param type Тип тайла.
 * @return Символ тайла или '?', если тип невалиден.
 */
TileSymbol tile_get_symbol(TileType type);

/**
 * @brief Возвращает тип тайла по его символу.
 * @param symbol Символ тайла.
 * @return Тип тайла или EMPTY_TILE, если символ не найден.
 */
TileType tile_get_type(TileSymbol symbol);

/**
 * @brief Создаёт новый тайл.
 * @param type Тип тайла.
 * @return Структура Tile с уникальным ID.
 */
Tile tile_create(TileType type);

/* Функции для работы с тайловыми картами */

/**
 * @brief Создаёт тайловую карту из шаблона символов.
 * @param map Указатель на структуру TileMap (инициализируется).
 * @param cols Количество колонок.
 * @param rows Количество строк.
 * @param map_template Массив символов (размер: cols × rows).
 * @return true при успехе, false при ошибке (неверные параметры или память).
 */
bool tilemap_create(
  TileMap* map,
  size_t cols,
  size_t rows,
  const TileSymbol* map_template
);

/**
 * @brief Освобождает память, занятую тайловой картой.
 * @param map Указатель на TileMap (поля обнуляются).
 */
void tilemap_free(TileMap *map);

/**
 * @brief Возвращает тайл по координатам.
 * @param map Указатель на TileMap.
 * @param col Номер колонки (0 ≤ col < cols).
 * @param row Номер строки (0 ≤ row < rows).
 * @return Указатель на тайл или NULL при ошибке.
 */
Tile* tilemap_get_tile(const TileMap *map, size_t col, size_t row);

/**
 * @brief Рендерит тайловую карту в терминале.
 * @param map Указатель на TileMap.
 * @details Выводит символы тайлов построчно с учётом анимаций (если есть).
 */
void tilemap_render(const TileMap* map);

/**
 * @brief Обновляет анимации тайлов на карте.
 * @param map Указатель на TileMap.
 * @details Увеличивает animation_progress для анимированных тайлов.
 */
void tilemap_update(TileMap* map);
