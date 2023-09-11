#include "commons.h"
#include <regex>

// cef string base

CefStrBase::CefStrBase() : cef_string_t{ (char16 *)u"", 0, nullptr }
{
}

bool CefStrBase::empty() const
{
    return length == 0;
}

bool CefStrBase::equal(const ustr &s, bool icase) const
{
    if (empty() && s.empty())
        return true;

    if (icase) {
        ustr s1((const char16_t *)str, length);
        ustr s2 = s;

        for (char16_t c : s1)
            s1.push_back(std::towlower(c));

        for (char16_t c : s2)
            s1.push_back(std::towlower(c));

        return s1 == s2;
    }

    return s.compare((const char16_t *)str) == 0;
}

bool CefStrBase::search(const ::str &regex, bool icase) const
{
    if (empty()) return false;

    auto flags = icase ? std::regex::icase : std::regex::flag_type(0);
    std::regex pattern(regex, flags);

    cef_string_utf8_t u8str{};
    cef_string_to_utf8(str, length, &u8str);

    ::str input(u8str.str, u8str.length);
    return std::regex_search(input, pattern);
}

// cef string helper

// static void __dtor(char16 *chars) {
//     if (chars != nullptr) {
//         free(chars);
//     }
// }

CefStr::CefStr() : CefStrBase()
{
}

CefStr::CefStr(const char *s, size_t l) : CefStrBase()
{
    cef_string_from_utf8(s, l, this);
}

CefStr::CefStr(const ::str &s) : CefStr(s.c_str(), s.length())
{
}

CefStr::CefStr(const ustr &s) : CefStrBase()
{
    cef_string_from_utf16((char16 *)s.c_str(), s.length(), this);
}

CefStr::~CefStr()
{
    if (dtor != nullptr)
    {
        dtor(str);
    }
}

cef_string_t CefStr::forward()
{
    auto dtor_ = this->dtor;
    this->dtor = nullptr;

    return cef_string_t{ str, length, dtor_ };
}

CefStrBase CefStr::borrow(const cef_string_t *s)
{
    CefStrBase base{};

    if (s != nullptr)
    {
        base.str = s->str;
        base.length = s->length;
    }
    else
    {
        base.str = (char16 *)u"";
        base.length = 0;
    }

    return base;
}

// userfree scoped string

CefScopedStr::CefScopedStr(cef_string_userfree_t uf) : CefStrBase(), str_(uf)
{
    if (uf != nullptr)
    {
        cef_string_t::str = uf->str;
        cef_string_t::length = uf->length;
    }
    else
    {
        cef_string_t::str = (char16 *)u"";
        cef_string_t::length = 0;
    }
}

CefScopedStr::~CefScopedStr()
{
    if (str_ != nullptr)
    {
        cef_string_userfree_free(str_);
    }
}