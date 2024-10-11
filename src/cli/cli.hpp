#ifndef _CLI_CLI_HPP
#define _CLI_CLI_HPP

#include "auto_completer/auto_completer.hpp"
#include <cctype>
#include <cstdio>
#include <cstring>
#include <termios.h>

class AppCompleter {
private:
  static constexpr int32_t MAX_WORD_LEN = 1024;
  static constexpr int32_t SUGGESTION_LIMIT = 7;

public:
  void run();

private:
  void setTerminal();
  void restoreTerminal();

  void displayPossibilities();
  void processCompletion();
  void processNewText();

  char buf[MAX_WORD_LEN];
  size_t len;
  AutoCompleter completer_;
};

#endif  // _CLI_CLI_HPP
