#pragma once

#include <codecvt>
#include <locale>
#include <vector>
#include <string>

namespace atom {
  namespace detail {
    template <typename T, size_t N>
    struct WideImpl { typedef std::basic_string<T> type; };

    template <>
    struct WideImpl<wchar_t, 1> { typedef std::string type; };

    template <>
    struct WideImpl<wchar_t, 2> { typedef std::u16string type; };

    template <>
    struct WideImpl<wchar_t, 4> { typedef std::u32string type; };

    class Convert {
    public:
      // Unicode transformation converts (covers everything from UTF8, UTF16 and UTF32)
      inline void Impl(const std::string& from, std::u16string& to) { to = convert16.from_bytes(from); }
      inline void Impl(const std::string& from, std::u32string& to) { to = convert32.from_bytes(from); }
      inline void Impl(const std::u16string& from, std::string& to) { to = convert16.to_bytes(from); }
      inline void Impl(const std::u16string& from, std::u32string& to) { to = convert32.from_bytes(convert16.to_bytes(from)); }
      inline void Impl(const std::u32string& from, std::string& to) { to = convert32.to_bytes(from); }
      inline void Impl(const std::u32string& from, std::u16string& to) { to = convert16.from_bytes(convert32.to_bytes(from)); }

    private:
      // Private members
      std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert16;
      std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert32;
    };
  }

  /// <summary>
  /// Compare two fixed-size wide-character string, ignoring case
  /// </summary>
  int wcsncasecmp(const wchar_t* one, const wchar_t* two, size_t maxLength);

  /// <summary>
  /// Returns the length of a fixed-size wide-character string
  /// </summary>
  size_t wcsnlen(const wchar_t* string, size_t maxLength);

  /// <summary>
  /// Converts one Unicode transformation format to another 
  /// </summary>
  template <typename To, typename From>
  std::basic_string<To> ConvertUTF(const std::basic_string<From>& string) {
    typedef typename detail::WideImpl<From, sizeof(wchar_t)>::type TypeFrom;
    typedef typename detail::WideImpl<To, sizeof(wchar_t)>::type TypeTo;

    TypeTo result;
    detail::Convert().Impl(TypeFrom(string.begin(), string.end()), result);

    return std::basic_string<To>(result.begin(), result.end());
  }
}
