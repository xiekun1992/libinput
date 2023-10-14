#pragma once
#include "../export.h"

#include <stdio.h>
#include <stdbool.h>
#if __linux == 1

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#elif _WIN32 == 1

// #define WINVER 0x0500 // SendInput needs this
#include <windows.h>

#endif

struct Keyboard {
#if __linux == 1
  Display* display;
#elif _WIN32 == 1
  INPUT ip;
  HKL kbl;
#endif
};

DLL_EXPORT void keyboard_init();
DLL_EXPORT void keyboard_dispose();
DLL_EXPORT bool keydown(int scancode);
DLL_EXPORT bool keyup(int scancode);
DLL_EXPORT int scancode_to_keycode(int scancode);