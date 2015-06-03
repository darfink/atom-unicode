#include <atom-unicode/Unicode.hpp>
#include <cwchar>

namespace atom {
  int wcsncasecmp(const wchar_t* one, const wchar_t* two, size_t maxLength) {
#ifdef _WIN32
    return _wcsnicmp(one, two, maxLength);
#else
    return ::wcsncasecmp(one, two, maxLength);
#endif
  }

  size_t wcsnlen(const wchar_t* string, size_t maxLength) {
#ifdef _WIN32
    return wcsnlen_s(string, maxLength);
#else
    return ::wcsnlen(string, maxLength);
#endif
  }
}
