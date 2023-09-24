#include "keyboard.h"

struct Keyboard context;

DLL_EXPORT void keyboard_init() {
#if __linux == 1
  display = XOpenDisplay(NULL);
#elif _WIN32 == 1
  context.kbl = GetKeyboardLayout(0);
  context.ip.type = INPUT_KEYBOARD;
  context.ip.ki.time = 0;
  context.ip.ki.dwExtraInfo = 0;
  context.ip.ki.wScan = 0;
#endif
}

DLL_EXPORT void keyboard_dispose() {
#if __linux == 1
  XCloseDisplay(display);
  // display = NULL;
#elif _WIN32 == 1
#endif
}

DLL_EXPORT int charToKeycode(int scancode) {
  // VkKeyScanEx(keystr.at(0), context.kbl);
#if __linux == 1
#elif _WIN32 == 1
  int virtualKeyCode = MapVirtualKey(scancode, MAPVK_VSC_TO_VK);
#endif
  return virtualKeyCode;
}

DLL_EXPORT bool keydown(int scancode) {
  int code = charToKeycode(scancode);
  if (code > 0) {
#if __linux == 1
    unsigned int keycode = XKeysymToKeycode(display, code);
    XTestFakeKeyEvent(display, keycode, True, 0);
    XFlush(display);
#elif _WIN32 == 1
    context.ip.ki.dwFlags = 0 | KEYEVENTF_EXTENDEDKEY;
    context.ip.ki.wVk = code;
    SendInput(1, &context.ip, sizeof(INPUT));
#endif
    return true;
  }
  return false;
}

DLL_EXPORT bool keyup(int scancode) {
  int code = charToKeycode(scancode);
  if (code > 0) {
#if __linux == 1
    unsigned int keycode = XKeysymToKeycode(display, code);
    XTestFakeKeyEvent(display, keycode, False, 0);
    XFlush(display);
#elif _WIN32 == 1
    context.ip.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_EXTENDEDKEY;
    context.ip.ki.wVk = code;
    SendInput(1, &context.ip, sizeof(INPUT));
#endif
    return true;
  }
  return false;
}