#pragma once
#include "../export.h"

#include <stdbool.h>

#if _WIN32 == 1

#define UNICODE

#include <shlobj.h>
#include <windows.h>

#elif __linux == 1

#include <X11/extensions/Xfixes.h>

#endif
struct Clipboard {
  bool notify;
  char* content;
  void (*fn)(void);
};
DLL_EXPORT void clipboard_init();
DLL_EXPORT void clipboard_dispose();
DLL_EXPORT bool write_text(char* text);
DLL_EXPORT char* read_text();
// bool write_files(std::vector<std::u16string> files);
// std::vector<std::u16string> read_files();
DLL_EXPORT void capture(void (*fn)(void));
DLL_EXPORT void release();