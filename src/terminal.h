
/**
 * @file terminal.h
 * @brief Библиотека для управления терминалом в raw-режиме с поддержкой ANSI-последовательностей.
 *
 * Предоставляет функции для:
 * - Переключения между raw- и обычным режимами терминала.
 * - Управления курсором через ANSI-коды.
 * - Очистки экрана/строк.
 * - Обработки сигналов (SIGINT, SIGTERM, SIGSEGV).
 */

#pragma once


#include <signal.h>   // signal, SIGINT, SIGTERM, SIGSEGV (обработка сигналов)
#include <stdio.h>    // printf, fprintf (вывод)
#include <stdlib.h>   // exit (аварийное завершение)
#include <termios.h>  // termios, tcgetattr, tcsetattr (raw-режим)
#include <unistd.h>   // STDIN_FILENO (дескриптор ввода)


#ifdef _WIN32
#define CSI "\x1b["  ///< ANSI Control Sequence Introducer для Windows.
#else
#define CSI "\033["  ///< ANSI Control Sequence Introducer для Unix-систем.
#endif


/**
 * @brief Режимы очистки экрана/строки.
 */
typedef enum EraseMode {
  ERASE_TO_END       = 0,  ///< Очистить от курсора до конца экрана/строки.
  ERASE_TO_BEGINNING = 1,  ///< Очистить от начала до курсора.
  ERASE_ALL          = 2,  ///< Очистить весь экран/строку.
  ERASE_SAVED_LINES  = 3,  ///< Очистить сохранённые строки (экран).
} EraseMode;


/* Режим терминала */
/**
 * @brief Активирует raw-режим терминала.
 * @details Отключает эхо, канонический режим и обработку сигналов.
 */
void enable_raw_mode(void);

/**
 * @brief Восстанавливает стандартный режим терминала.
 */
void disable_raw_mode(void);

/* ANSI-последовательности (управление курсором) */
/**
 * @brief Перемещает курсор вверх на `n` строк (CUU).
 * @param n Количество строк (≥1).
 */
void cursor_up(int n);

/**
 * @brief Перемещает курсор вниз на `n` строк (CUD).
 * @param n Количество строк (≥1).
 */
void cursor_down(int n);

/**
 * @brief Перемещает курсор вправо на `n` позиций (CUF).
 * @param n Количество позиций (≥1).
 */
void cursor_forward(int n);

/**
 * @brief Перемещает курсор влево на `n` позиций (CUB).
 * @param n Количество позиций (≥1).
 */
void cursor_back(int n);

/**
 * @brief Перемещает курсор на начало `n`-й строки ниже текущей (CNL).
 * @param n Количество строк (≥1).
 */
void cursor_next_line(int n);

/**
 * @brief Перемещает курсор на начало `n`-й строки выше текущей (CPL).
 * @param n Количество строк (≥1).
 */
void cursor_previous_line(int n);

/**
 * @brief Устанавливает курсор в колонку `n` текущей строки (CHA).
 * @param n Номер колонки (≥1).
 */
void cursor_horizontal_absolute(int n);

/**
 * @brief Устанавливает курсор на позицию (n, m) (CUP).
 * @param n Номер строки (≥1).
 * @param m Номер колонки (≥1).
 */
void cursor_position(int n, int m);

/**
 * @brief Очищает экран в указанном режиме (ED).
 * @param mode Режим очистки (см. EraseMode).
 */
void erase_in_display(EraseMode mode);

/**
 * @brief Очищает текущую строку в указанном режиме (EL).
 * @param mode Режим очистки (ERASE_TO_END, ERASE_TO_BEGINNING, ERASE_ALL).
 */
void erase_in_line(EraseMode mode);

/**
 * @brief Прокручивает экран вверх на `n` строк (SU).
 * @param n Количество строк (≥1).
 */
void scroll_up(int n);

/**
 * @brief Прокручивает экран вниз на `n` строк (SD).
 * @param n Количество строк (≥1).
 */
void scroll_down(int n);

/* Приватные ANSI-последовательности */
/**
 * @brief Сохраняет текущую позицию курсора (SCP).
 */
void cursor_save(void);

/**
 * @brief Восстанавливает сохранённую позицию курсора (RCP).
 */
void cursor_restore(void);

/**
 * @brief Скрывает курсор (DECTCEM).
 */
void cursor_hide(void);

/**
 * @brief Показывает курсор (DECTCEM).
 */
void cursor_show(void);

/* Высокоуровневые функции */
/**
 * @brief Перемещает курсор в начало экрана (1, 1).
 */
void cursor_home(void);

/**
 * @brief Сбрасывает курсор в начало экрана и делает его видимым.
 */
void cursor_reset(void);

/**
 * @brief Очищает текущую строку полностью.
 */
void clear_line(void);

/**
 * @brief Очищает весь экран.
 */
void clear_screen(void);

/**
 * @brief Сбрасывает буфер вывода терминала.
 */
void flush_terminal(void);

/* Цвета */
/**
 * @brief Сбрасывает все установленные цвета (ANSI SGR 0).
 */
void clear_colors(void);

/* Сигналы */
/**
 * @brief Устанавливает обработчики для SIGINT, SIGTERM и SIGSEGV.
 * @details При получении сигнала восстанавливает режим терминала и завершает программу.
 */
void setup_signal_handlers(void);
