#include "clipboard.h"

struct Clipboard context;

DLL_EXPORT void clipboard_init() {
  context.content = (char*)malloc(sizeof(char) * 1024 * 1024);
  context.notify = false;
}

DLL_EXPORT void clipboard_dispose() {
  free(context.content);
}

#if _WIN32 == 1
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_CLIPBOARDUPDATE:
    // handle change 
    context.fn();
		break;
	// case WM_PAINT:
	// {
	// 	PAINTSTRUCT ps;
	// 	HDC hdc = BeginPaint(hWnd, &ps);
	// 	EndPaint(hWnd, &ps);
	// }
	// break;
	case WM_DESTROY:
		PostQuitMessage(0);
		RemoveClipboardFormatListener(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
#endif

DLL_EXPORT bool write_text(char* text) {
  bool res = false;
#if _WIN32 == 1
  OpenClipboard(0);
  HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(text) + 1);
  if (clipbuffer != NULL) {
    char* buffer = (char*)GlobalLock(clipbuffer);
    if (buffer != NULL) {
      memcpy(buffer, text, strlen(text));
      EmptyClipboard();
      SetClipboardData(CF_TEXT, clipbuffer);
      GlobalUnlock(clipbuffer);
      res = true;
    }
  }
  CloseClipboard();
#endif
  return res;
}

DLL_EXPORT char* read_text() {
  strcpy(context.content, "");
#if _WIN32 == 1
  OpenClipboard(0);

  if (!IsClipboardFormatAvailable(CF_UNICODETEXT) && !IsClipboardFormatAvailable(CF_TEXT)) {
    return context.content;
  }

  HANDLE handle = (HANDLE)GetClipboardData(CF_TEXT);
  if (handle != NULL) {
    if (GlobalLock(handle) != NULL) {
      char* buffer = (char*)handle;
      int len = strlen(buffer);
      strncpy(context.content, buffer, len);
      context.content[len] = '\0';
      GlobalUnlock(handle);
    }
  }
  CloseClipboard();
#endif
  return context.content;
}

// bool write_files(std::vector<std::u16string> files) {
// #if _WIN32 == 1
//   OpenClipboard(0);

//   std::u16string sFiles; // 对文件名以双字节存放
//   for (std::u16string file : files) {
//     sFiles += file;
//     sFiles.push_back(0); // 追加'\0'
//   }

//   DROPFILES dobj = { 20, { 0, 0 }, 0, 1 };
//   int nLen = (sFiles.size() + 1) * sizeof(char16_t);
//   int nGblLen = sizeof(dobj) + nLen;
  
//   HGLOBAL hGbl = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, nGblLen);
//   if (hGbl != NULL) {
//     char* buffer = (char*)GlobalLock(hGbl);
//     if (buffer != NULL) {
//       memcpy(buffer, &dobj, sizeof(dobj));
//       memcpy(buffer + sizeof(dobj), sFiles.c_str(), nLen);
//       EmptyClipboard();
//       SetClipboardData(CF_HDROP, hGbl);
//       GlobalUnlock(hGbl);

//       CloseClipboard();
//       return true;
//     }
//   }
//   CloseClipboard();
// #endif
//   return false;
// }

// std::vector<std::u16string> read_files() {
//   std::vector<std::basic_string<char16_t>> filenames;
// #if _WIN32 == 1
//   OpenClipboard(0);

//   if (!IsClipboardFormatAvailable(CF_HDROP)) return filenames;

//   HGLOBAL hClipboardText = (HGLOBAL)GetClipboardData(CF_HDROP);
//   if (hClipboardText != NULL) {
//     HDROP  h = (HDROP)GlobalLock(hClipboardText);
//     if (h != NULL) {
//       int fileCount = DragQueryFile(h, 0xFFFFFFFF, nullptr, 0);
//       for (int i = 0; i < fileCount; i++) {
//         std::basic_string<char16_t> str;
//         filenames.push_back(str);
//         int filenameLength = DragQueryFile(h, i, nullptr, 0);
//         filenames[i].reserve(filenameLength);
//         DragQueryFile(h, i, (LPWSTR)&(filenames[i][0]), filenameLength + 1);
//       }
//       GlobalUnlock(hClipboardText);

//       CloseClipboard();
//       return filenames;
//     }
//   }
//   CloseClipboard();
// #endif
//   return filenames;
// }

DLL_EXPORT void capture(void (*fn)(void)) {
  context.fn = fn;

#if _WIN32 == 1
  const wchar_t CLASS_NAME[] = L"Clipboard Window Class";

  HINSTANCE hInstance = GetModuleHandle(0);
  WNDCLASS wc = { 0 };
  wc.lpfnWndProc = WndProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;

  RegisterClass(&wc);
  HWND hwnd = CreateWindowEx(
    0,                              // Optional window styles.
    CLASS_NAME,                     // Window class
    L"Clipboard Listener Windows",    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style
    // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    NULL,       // Parent window    
    NULL,       // Menu
    hInstance,  // Instance handle
    NULL        // Additional application data
  );
  if (hwnd != NULL) {
    AddClipboardFormatListener(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
#elif __linux == 1
  notify = true;
  Display *display = XOpenDisplay(NULL);
  unsigned long color = BlackPixel(display, DefaultScreen(display));
  Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0,0, 1,1, 0, color, color);
  
  const char* bufname_clipboard = "CLIPBOARD";
  const char* bufname_primary = "PRIMARY";
  int event_base, error_base;
  XEvent event;
  Atom bufid_clip = XInternAtom(display, bufname_clipboard, False);
  Atom bufid_pri = XInternAtom(display, bufname_primary, False);

  XFixesQueryExtension(display, &event_base, &error_base);
  XFixesSelectSelectionInput(display, DefaultRootWindow(display), bufid_clip, XFixesSetSelectionOwnerNotifyMask);
  XFixesSelectSelectionInput(display, DefaultRootWindow(display), bufid_pri, XFixesSetSelectionOwnerNotifyMask);

  while (notify) {
    if (XPending(display)) {
      XNextEvent(display, &event);
      if (
        event.type == event_base + XFixesSelectionNotify && 
        (((XFixesSelectionNotifyEvent*)&event)->selection == bufid_clip || ((XFixesSelectionNotifyEvent*)&event)->selection == bufid_pri)
      ) {
        // printf("update\n");
        lambda_update_handler();
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // prevent high cpu usage
  }

  XDestroyWindow(display, window);
  XCloseDisplay(display);
#endif
}

DLL_EXPORT void release() {
#if __linux == 1
  context.notify = false;
#endif
}