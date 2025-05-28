
// file: terminal.c

#include "terminal.h"

static struct termios original_termios;


// Изменение режима работы терминала

void enable_raw_mode(void) {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &original_termios);               // получаем текущие настройки терминала
  raw = original_termios;                                   // копируем настройки для модификации
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);          // отключаем обработку сигналов, эхо, канонический режим
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // отключаем обработку специальных символов
  raw.c_oflag &= ~(OPOST);                                  // отключаем постобработку вывода
  raw.c_cflag |= (CS8);                                     // устанавливаем размер символа 8 бит
  raw.c_cc[VMIN] = 0;                                       // минимальное количество символов для read()
  raw.c_cc[VTIME] = 1;                                      // максимальное время ожидания для read() (1/10 секунды)
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);                 // применяем новые настройки
}

void disable_raw_mode(void) {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}


// ANSI control sequences

void cursor_up(int n) {
  if (n < 1) n = 1;
  printf(CSI "%dA", n);
}

void cursor_down(int n) {
  if (n < 1) n = 1;
  printf(CSI "%dB", n);
}

void cursor_forward(int n) {
  if (n < 1) n = 1;
  printf(CSI "%dC", n);
}

void cursor_back(int n) {
  if (n < 1) n = 1;
  printf(CSI "%dD", n);
}

void cursor_next_line(int n) {
  if (n < 1) n = 1;
  printf(CSI "%dE", n);
}

void cursor_previous_line(int n) {
  if (n < 1) n = 1;
  printf(CSI "%dF", n);
}

void cursor_horizontal_absolute(int n) {
  if (n < 1) n = 1;
  printf(CSI "%dG", n);
}

void cursor_position(int n, int m) {
  if (n < 1) n = 1;
  if (m < 1) m = 1;
  printf(CSI "%d;%dH", n, m);
}

void erase_in_display(EraseMode mode) {
  if (mode < 0 || mode > ERASE_SAVED_LINES) {
    fprintf(stderr, "Invalid display erase mode: %d. Using default.\n", mode);
    mode = ERASE_TO_END;
  }
  printf(CSI "%uJ", mode);
}

void erase_in_line(EraseMode mode) {
  if (mode < 0 || mode > ERASE_ALL) {
    fprintf(stderr, "Invalid line erase mode: %d. Using default.\n", mode);
    mode = ERASE_TO_END;
  }
  printf(CSI "%uK", mode);
}

void scroll_up(int n) {
  if (n < 1) n = 1;
  printf(CSI "%dS", n);
}

void scroll_down(int n) {
  if (n < 1) n = 1;
  printf(CSI "%dT", n);
}


// ANSI private sequences

void cursor_save(void) {
  printf(CSI "s");
}

void cursor_restore(void) {
  printf(CSI "u");
}

void cursor_hide(void) {
  printf(CSI "?25l");
}

void cursor_show(void) {
  printf(CSI "?25h");
}


// More high-level abstractions

void cursor_home(void) {
  cursor_position(1, 1);
}

void cursor_reset(void) {
  cursor_home();
  cursor_show();
}

void clear_line(void) {
  erase_in_line(ERASE_ALL);
}

void clear_screen(void) {
  erase_in_display(ERASE_ALL);
}

void flush_terminal(void) {
  fflush(stdout);
}

// Colors

void clear_colors(void) {
  printf("\033[0m");
}


// Signals

static void handle_signal(int sig) {
  disable_raw_mode();
  exit(1);
}

void setup_signal_handlers(void) {
  signal(SIGINT,  handle_signal);
  signal(SIGTERM, handle_signal);
  signal(SIGSEGV, handle_signal);
}
