#ifndef _AUTO_COMPLETER_AUTO_COMPLETER_HPP
#define _AUTO_COMPLETER_AUTO_COMPLETER_HPP

#include <cstdint>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

class AutoCompleter {
private:
  static constexpr int64_t ROOT_STATE = 0;

  struct State {
    int64_t end_cnt = 0;
    int64_t max_freq = -1;
    int64_t max_freq_cnt = 0;
    int64_t prev = -1;
    std::map<char, int64_t> next;
  };

public:
  AutoCompleter();

  void addWord(const char *str, size_t len);
  int64_t getPossibleCnt(const char *str, size_t len);
  void printPossibilities(const char *str, size_t len);

private:
  void printPossibilitiesByState(int64_t curr, std::string &buf);

  std::vector<State> states_;
};

#endif // _AUTO_COMPLETER_AUTO_COMPLETER_HPP
