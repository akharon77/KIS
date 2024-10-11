#include "auto_completer/auto_completer.hpp"

AutoCompleter::AutoCompleter() { states_.emplace_back(); }

void AutoCompleter::addWord(const char *str, size_t len) {
  int64_t curr = ROOT_STATE;
  for (size_t i = 0; i < len; ++i) {
    char sym = str[i];
    if (states_[curr].next.contains(sym)) {
      curr = states_[curr].next[sym];
    } else {
      size_t last = states_.size();
      states_[curr].next[sym] = last;
      states_.emplace_back();
      states_.back().prev = curr;
      curr = last;
    }
  }

  states_[curr].end_cnt += 1;

  while (curr != -1) {
    State &curr_state = states_[curr];
    curr_state.max_freq = curr_state.end_cnt;
    curr_state.max_freq_cnt = 1;

    for (const auto &[sym, next] : states_[curr].next) {
      State &next_state = states_[next];
      if (next_state.max_freq == curr_state.max_freq) {
        curr_state.max_freq_cnt += next_state.max_freq_cnt;
      } else if (next_state.max_freq > curr_state.max_freq) {
        curr_state.max_freq = next_state.max_freq;
        curr_state.max_freq_cnt = next_state.max_freq_cnt;
      }
    }

    curr = states_[curr].prev;
  }
}

int64_t AutoCompleter::getPossibleCnt(const char *str, size_t len) {
  int64_t curr = ROOT_STATE;
  for (size_t i = 0; i < len; ++i) {
    char sym = str[i];
    if (states_[curr].next.contains(sym)) {
      curr = states_[curr].next[sym];
    } else {
      return 0;
    }
  }

  return states_[curr].max_freq_cnt;
}

void AutoCompleter::printPossibilities(const char *str, size_t len) {
  std::string buf;
  int64_t curr = ROOT_STATE;

  for (size_t i = 0; i < len; ++i) {
    char sym = str[i];
    if (states_[curr].next.contains(sym)) {
      curr = states_[curr].next[sym];
      buf.push_back(sym);
    } else {
      printf("There is no possibilities!\n");
      return;
    }
  }

  printPossibilitiesByState(curr, buf);
}

void AutoCompleter::printPossibilitiesByState(int64_t curr, std::string &buf) {
  State &curr_state = states_[curr];

  if (curr_state.end_cnt == curr_state.max_freq) {
    printf("%s\n", buf.c_str());
  }

  for (const auto &[sym, next] : curr_state.next) {
    State &next_state = states_[next];
    if (next_state.max_freq == curr_state.max_freq) {
      buf.push_back(sym);
      printPossibilitiesByState(next, buf);
      buf.pop_back();
    }
  }
}
