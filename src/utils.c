#include "utils.h"

DLL_EXPORT void utils_open(wchar_t *path) {
  setlocale(LC_ALL, "");
  SHELLEXECUTEINFO info = { 0 };
  info.cbSize = sizeof(info);
  info.nShow = SW_SHOWDEFAULT;
  info.lpFile = L"explorer.exe";
  info.lpParameters = path;
  ShellExecuteEx(&info);
}