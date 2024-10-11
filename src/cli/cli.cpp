#include "cli/cli.hpp"

void AppCompleter::run() {
  setTerminal();

  while (true) {
    len = 0;
    printf("> ");

    char c = getchar();
    switch (c) {
    case '\e':
      restoreTerminal();
      return;
    case '~':
      processNewText();
      break;
    default:
      ungetc(c, stdin);
      processCompletion();
    }
  }
}

void AppCompleter::setTerminal() {
  struct termios info;
  tcgetattr(0, &info);

  info.c_lflag &= ~ICANON;
  info.c_cc[VMIN] = 1;
  info.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &info);
}

void AppCompleter::restoreTerminal() {
  struct termios info;
  tcgetattr(0, &info);
  info.c_lflag |= ICANON;
  tcsetattr(0, TCSANOW, &info);
}

void AppCompleter::displayPossibilities() {
  int64_t cnt = completer_.getPossibleCnt(buf, len);
  if (cnt > SUGGESTION_LIMIT) {
    printf("Display all %ld possibilities? (y or n)\n", cnt);

    char c = getchar();
    puts("");
    switch (c) {
    case 'y':
    case 'Y':
      completer_.printPossibilities(buf, len);
      break;
    default:
      break;
    }
  } else {
    completer_.printPossibilities(buf, len);
  }
}

void AppCompleter::processCompletion() {
  while (true) {
    char c = getchar();

    if (c == '\t') {
      puts("");
      displayPossibilities();
      buf[len] = '\0';
      printf("> %s", buf);
      continue;
    } else if (c == '\n') {
      break;
    }

    buf[len] = c;
    ++len;
  }
}

void AppCompleter::processNewText() {
  restoreTerminal();
  while (true) {
    scanf("%s", buf);
    len = strlen(buf);
    if (len == 1 && buf[0] == '~') {
      break;
    }
    completer_.addWord(buf, len);
  }
  fflush(stdin);
  getchar();
  setTerminal();
}
