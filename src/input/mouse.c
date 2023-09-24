#include "mouse.h"

#if __linux == 1
struct Mouse context;
#endif

DLL_EXPORT void mouse_init() {
#if __linux == 1
  context.display = XOpenDisplay(0);
  context.root = DefaultRootWindow(context.display);
#endif
}

DLL_EXPORT void mouse_dispose() {
#if __linux == 1
  XCloseDisplay(context.display);
#endif
}

DLL_EXPORT void mouse_move(int x, int y) {
#if _WIN32 == 1
  double fScreenWidth = (double)GetSystemMetrics(SM_CXSCREEN) - 1;
  double fScreenHeight = (double)GetSystemMetrics(SM_CYSCREEN) - 1;
  double fx = x * (65535.0f / fScreenWidth);
  double fy = y * (65535.0f / fScreenHeight);

  INPUT input;
  // https://stackoverflow.com/questions/17315672/screen-goes-black-when-i-use-sendinput-to-send-mouse-cursor-positions
  ZeroMemory(&input, sizeof(input));
  input.type = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
  // printf("%d %d\n", fx, fy);
  input.mi.dx = fx;
  input.mi.dy = fy;
  SendInput(1, &input, sizeof(INPUT));
#elif __linux == 1
  XWarpPointer(context.display, None, context.root, 0, 0, 0, 0, x, y);
  XSync(context.display, false);
#endif
}

DLL_EXPORT void mouse_wheel(enum MouseWheel direction) { // -1: up, 1: down
#if _WIN32 == 1
  INPUT input;
  ZeroMemory(&input, sizeof(input));
  input.type = INPUT_MOUSE;
  input.mi.mouseData = direction > 0 ? WHEEL_DELTA : -1 * WHEEL_DELTA;
  input.mi.dwFlags = MOUSEEVENTF_WHEEL | MOUSEEVENTF_ABSOLUTE;
  input.mi.dx = 0;
  input.mi.dy = 0;
  SendInput(1, &input, sizeof(INPUT));
#elif __linux == 1
  int button;
  if (-1 == direction) {
    button = Button4;
  }
  if (1 == direction) {
    button = Button5;
  }
  XTestFakeButtonEvent(context.display, button, true, 0);
  XFlush(context.display);
  XTestFakeButtonEvent(context.display, button, false, 0);
  XFlush(context.display);
#endif
}

DLL_EXPORT void mouse_down(enum MouseButton button) { // 1: left(Button1), 2: middle(Button2), 3: right(Button3)
#if _WIN32 == 1
  INPUT input;
  ZeroMemory(&input, sizeof(input));
  input.type = INPUT_MOUSE;
  input.mi.dx = 0;
  input.mi.dy = 0;
  switch (button) {
    case MOUSE_LEFT: input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_ABSOLUTE; break;
    case MOUSE_MIDDLE: input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_ABSOLUTE; break;
    case MOUSE_RIGHT: input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_ABSOLUTE; break;
  }
  SendInput(1, &input, sizeof(INPUT));
#elif __linux == 1
  XTestFakeButtonEvent(context.display, button, true, 0);
  XFlush(context.display);
#endif
}

DLL_EXPORT void mouse_up(int button) {
#if _WIN32 == 1
  INPUT input;
  ZeroMemory(&input, sizeof(input));
  input.type = INPUT_MOUSE;
  input.mi.dx = 0;
  input.mi.dy = 0;
  switch (button) {
    case MOUSE_LEFT: input.mi.dwFlags = MOUSEEVENTF_LEFTUP | MOUSEEVENTF_ABSOLUTE; break;
    case MOUSE_MIDDLE: input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP | MOUSEEVENTF_ABSOLUTE; break;
    case MOUSE_RIGHT: input.mi.dwFlags = MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_ABSOLUTE; break;
  }
  SendInput(1, &input, sizeof(INPUT));
#elif __linux == 1
  XTestFakeButtonEvent(context.display, button, false, 0);
  XFlush(context.display);
#endif
}