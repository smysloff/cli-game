
// file: termios.c

#define FPS 60
#define FRAME_TIME_MS (1'000'000'000 / FPS)

static struct termios original_termios;

// Включение raw-режима
void enable_raw_mode() {
  struct termios raw;

  // Получаем текущие настройки терминала
  tcgetattr(STDIN_FILENO, &original_termios);

  // Копируем настройки для модификации
  raw = original_termios;

  // Отключаем обработку сигналов, эхо, канонический режим
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  // Отключаем обработку специальных символов
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  // Отключаем постобработку вывода
  raw.c_oflag &= ~(OPOST);

  // Устанавливаем размер символа 8 бит
  raw.c_cflag |= (CS8);

  // Минимальное количество символов для read()
  raw.c_cc[VMIN] = 0;
  // Максимальное время ожидания для read() (1/10 секунды)
  raw.c_cc[VTIME] = 1;

  // Применяем новые настройки
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Выключение raw-режима (восстановление оригинальных настроек)
void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

void clear_screen() {
  printf("\033[2J"); // clear screen
  printf("\033[H");  // move cursor to begining
}

// Функция для скрытия курсора
void hide_cursor() {
  printf("\033[?25l");
  fflush(stdout);
}

// Функция для показа курсора
void show_cursor() {
  printf("\033[?25h");
  fflush(stdout);
}

// Перемещаем курсор в начальную позицию (0,0)
void refresh_cursor() {
  printf("\033[H");
  fflush(stdout);
}

// Сброс цвета
void refresh_colors() {
  printf("\033[0m");
}

// Переход в начало следующей строки
void new_line() {
  printf("\033[E");
}

void signal_handler(int sig) {
  disable_raw_mode();
  _exit(1);
}
