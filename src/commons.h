#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>

#define CEF_STRING_TYPE_UTF16 1
#include "include/internal/cef_string.h"
#include "include/capi/cef_base_capi.h"
#include "include/capi/cef_v8_capi.h"

#define VK_OEM_PLUS     0xBB
#define VK_OEM_MINUS    0xBD
#define VK_F12          0x7B

#define CALLBACK

using str = std::string;
using ustr = std::u16string;

template <typename T>
using vec = std::vector<T>;

template <typename T>
struct CefRefCount : public T
{
    template <typename U>
    CefRefCount(const U *) noexcept : T{}, ref_(1) {
        T::base.size = sizeof(U);
        T::base.add_ref = _Base_AddRef;
        T::base.release = _Base_Release;
        T::base.has_one_ref = _Base_HasOneRef;
        T::base.has_at_least_one_ref = _Base_HasAtLeastOneRef;
        self_delete_ = [](void *self) noexcept { delete static_cast<U *>(self); };
    }

    CefRefCount(nullptr_t) noexcept : CefRefCount(static_cast<T *>(nullptr)) {}

private:
    void(*self_delete_)(void *);
    std::atomic<size_t> ref_;

    static void CALLBACK _Base_AddRef(cef_base_ref_counted_t *_) noexcept {
        ++reinterpret_cast<CefRefCount *>(_)->ref_;
    }

    static int CALLBACK _Base_Release(cef_base_ref_counted_t *_) noexcept {
        CefRefCount *self = reinterpret_cast<CefRefCount *>(_);
        if (--self->ref_ == 0) {
            self->self_delete_(_);
            return 1;
        }
        return 0;
    }

    static int CALLBACK _Base_HasOneRef(cef_base_ref_counted_t *_) noexcept {
        return reinterpret_cast<CefRefCount *>(_)->ref_ == 1;
    }

    static int CALLBACK _Base_HasAtLeastOneRef(cef_base_ref_counted_t *_) noexcept {
        return reinterpret_cast<CefRefCount *>(_)->ref_ > 0;
    }
};

struct CefStrBase : cef_string_t
{
    CefStrBase();

    bool empty() const;
    bool equal(const ustr &s, bool icase = false) const;
    bool search(const ::str &regex, bool icase = false) const;

    ustr cstr() const;
};

struct CefStr : CefStrBase
{
    CefStr();

    CefStr(const char *s, size_t l);
    CefStr(const ::str &s);
    CefStr(const ustr &s);
    ~CefStr();

    cef_string_t forward();
    static CefStrBase borrow(const cef_string_t *s);
};

struct CefScopedStr : CefStrBase
{
    CefScopedStr(cef_string_userfree_t uf);
    ~CefScopedStr();

    const cef_string_t *ptr() {
        return str_;
    }

private:
    cef_string_userfree_t str_;
};

static cef_string_t operator ""_s(const char16_t *s, size_t l)
{
    return cef_string_t { (char16 *)s, l, nullptr };
}

namespace utils
{
    str loader_dir();
    str plugins_dir();

    void shell_open(const str &url);
    void msgbox(const str &caption, const str &content);

    bool is_file(const str &path);
    bool is_dir(const str &path);

    bool read_file(const str &path, str &output);
    vec<str> read_dir(const str &path);
}