#include <stdint.h>
#include <dlfcn.h>
#include <unistd.h>

static void *find_proc(const char* func)
{
    static void *egl = nullptr;
    if (egl == nullptr) {
        egl = dlopen("old_libEGL.dylib", 0/* RTLD_LAZY */);
    }
    return dlsym(egl, func);
}

template<int L, typename T>
static T _forward(const char* funcName, T)
{
    static T proc = nullptr;
    if (proc != nullptr) return proc;
    return proc = reinterpret_cast<T>(find_proc(funcName));
}

#define FORWARD(F) _forward(#F, F)
#define FUNCTION(T, N)

#define __X_P0(_1)                                  _1 p1
#define __X_P1(_1, _2)                              _1 p1, _2 p2
#define __X_P2(_1, _2, _3)                          _1 p1, _2 p2, _3 p3
#define __X_P3(_1, _2, _3, _4)                      _1 p1, _2 p2, _3 p3, _4 p4
#define __X_P4(_1, _2, _3, _4, _5)                  _1 p1, _2 p2, _3 p3, _4 p4, _5 p5
#define __X_P5(_1, _2, _3, _4, _5, _6)              _1 p1, _2 p2, _3 p3, _4 p4, _5 p5, _6 p6
#define __X_P6(_1, _2, _3, _4, _5, _6, _7)          _1 p1, _2 p2, _3 p3, _4 p4, _5 p5, _6 p6, _7 p7
#define __X_P7(_1, _2, _3, _4, _5, _6, _7, _8)      _1 p1, _2 p2, _3 p3, _4 p4, _5 p5, _6 p6, _7 p7, _8 p8
#define __X_P8(_1, _2, _3, _4, _5, _6, _7, _8, _9)  _1 p1, _2 p2, _3 p3, _4 p4, _5 p5, _6 p6, _7 p7, _8 p8, _9 p9

#define __X_P_FC(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define __X_P_FR(args) __X_P_FC args
#define __X_P_CFA(...) __X_P_FR((__VA_ARGS__, __X_P8, __X_P7, __X_P6, __X_P5, __X_P4, __X_P3, __X_P2, __X_P1, __X_P0, , ))
#define __X_P_MC(...) __X_P_CFA(__VA_ARGS__ ())

#define __X_A0(_1)                                  p1
#define __X_A1(_1, _2)                              p1, p2
#define __X_A2(_1, _2, _3)                          p1, p2, p3
#define __X_A3(_1, _2, _3, _4)                      p1, p2, p3, p4
#define __X_A4(_1, _2, _3, _4, _5)                  p1, p2, p3, p4, p5
#define __X_A5(_1, _2, _3, _4, _5, _6)              p1, p2, p3, p4, p5, p6
#define __X_A6(_1, _2, _3, _4, _5, _6, _7)          p1, p2, p3, p4, p5, p6, p7
#define __X_A7(_1, _2, _3, _4, _5, _6, _7, _8)      p1, p2, p3, p4, p5, p6, p7, p8
#define __X_A8(_1, _2, _3, _4, _5, _6, _7, _8, _9)  p1, p2, p3, p4, p5, p6, p7, p8, p9

#define __X_A_FC(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define __X_A_FR(args) __X_A_FC args
#define __X_A_CFA(...) __X_A_FR((__VA_ARGS__, __X_A8, __X_A7, __X_A6, __X_A5, __X_A4, __X_A3, __X_A2, __X_A1, __X_A0, , ))
#define __X_A_MC(...) __X_A_CFA(__VA_ARGS__ ())

#define __EXPAND(x) x

#define PARAMS(...) __EXPAND(__X_P_MC(__VA_ARGS__)(__VA_ARGS__))
#define ARGUMENTS(...) __EXPAND(__X_A_MC(__VA_ARGS__)(__VA_ARGS__))

#define PROXY_API(FN, ...) \
    FN (PARAMS(__VA_ARGS__)) { return _forward<__LINE__>(#FN, FN)(ARGUMENTS(__VA_ARGS__)); }

/* EGL type definitions */

#define EGLAPI extern "C" __attribute__((visibility("default")))
#define EGLAPIENTRY

typedef int32_t EGLint;
typedef unsigned int EGLBoolean;
typedef unsigned int EGLenum;
typedef uint64_t EGLTime;
typedef uint64_t EGLTimeKHR;
typedef intptr_t EGLAttrib;
typedef uint64_t EGLnsecsANDROID;
typedef uint64_t EGLuint64KHR;
typedef uint64_t EGLFrameTokenANGLE;

typedef void *EGLDisplay;
typedef void *EGLConfig;
typedef void *EGLSurface;
typedef void *EGLContext;
typedef void *EGLSync;
typedef void *EGLSyncKHR;
typedef void *EGLDeviceEXT;
typedef void *EGLNativePixmapType;
typedef void *EGLImage;
typedef void *EGLImageKHR;
typedef void *EGLClientBuffer;
typedef void *EGLStreamKHR;
typedef void *EGLNativeWindowType;
typedef void *EGLDEBUGPROCKHR;
typedef void *EGLLabelKHR;
typedef void *EGLObjectKHR;
typedef void *__eglMustCastToProperFunctionPointerType;
typedef int   EGLNativeDisplayType;
typedef void *EGLNativePixmapType;
typedef void *EGLNativeWindowType;

// Reference to dylib

extern "C" EGLClientBuffer eglCreateNativeClientBufferANDROID(const EGLint *);
void *__ref = reinterpret_cast<void *>(&eglCreateNativeClientBufferANDROID);

/* EGL API functions */

EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglBindAPI, EGLenum);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglBindTexImage, EGLDisplay, EGLSurface, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglChooseConfig, EGLDisplay, const EGLint *, EGLConfig *, EGLint, EGLint *);
EGLAPI EGLint EGLAPIENTRY PROXY_API(eglClientWaitSync, EGLDisplay, EGLSync, EGLint, EGLTime);
EGLAPI EGLint EGLAPIENTRY PROXY_API(eglClientWaitSyncKHR, EGLDisplay, EGLSyncKHR, EGLint, EGLTimeKHR);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglCopyBuffers, EGLDisplay, EGLSurface, EGLNativePixmapType);
EGLAPI EGLContext EGLAPIENTRY PROXY_API(eglCreateContext, EGLDisplay, EGLConfig, EGLContext, const EGLint *);
EGLAPI EGLDeviceEXT EGLAPIENTRY PROXY_API(eglCreateDeviceANGLE, EGLint, void *, const EGLAttrib *);
EGLAPI EGLImage EGLAPIENTRY PROXY_API(eglCreateImage, EGLDisplay, EGLContext, EGLenum, EGLClientBuffer, const EGLAttrib *);
EGLAPI EGLImageKHR EGLAPIENTRY PROXY_API(eglCreateImageKHR, EGLDisplay, EGLContext, EGLenum, EGLClientBuffer, const EGLint *);
// EGLAPI EGLClientBuffer EGLAPIENTRY PROXY_API(eglCreateNativeClientBufferANDROID, const EGLint *);
EGLAPI EGLSurface EGLAPIENTRY PROXY_API(eglCreatePbufferFromClientBuffer, EGLDisplay, EGLenum, EGLClientBuffer, EGLConfig, const EGLint *);
EGLAPI EGLSurface EGLAPIENTRY PROXY_API(eglCreatePbufferSurface, EGLDisplay, EGLConfig, const EGLint *);
EGLAPI EGLSurface EGLAPIENTRY PROXY_API(eglCreatePixmapSurface, EGLDisplay, EGLConfig, EGLNativePixmapType, const EGLint *);
EGLAPI EGLSurface EGLAPIENTRY PROXY_API(eglCreatePlatformPixmapSurface, EGLDisplay, EGLConfig, void *, const EGLAttrib *);
EGLAPI EGLSurface EGLAPIENTRY PROXY_API(eglCreatePlatformPixmapSurfaceEXT, EGLDisplay, EGLConfig, void *, const EGLint *);
EGLAPI EGLSurface EGLAPIENTRY PROXY_API(eglCreatePlatformWindowSurface, EGLDisplay, EGLConfig, void *, const EGLAttrib *);
EGLAPI EGLSurface EGLAPIENTRY PROXY_API(eglCreatePlatformWindowSurfaceEXT, EGLDisplay, EGLConfig, void *, const EGLint *);
EGLAPI EGLStreamKHR EGLAPIENTRY PROXY_API(eglCreateStreamKHR, EGLDisplay, const EGLint *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglCreateStreamProducerD3DTextureANGLE, EGLDisplay, EGLStreamKHR, const EGLAttrib *);
EGLAPI EGLSync EGLAPIENTRY PROXY_API(eglCreateSync, EGLDisplay, EGLenum, const EGLAttrib *);
EGLAPI EGLSyncKHR EGLAPIENTRY PROXY_API(eglCreateSyncKHR, EGLDisplay, EGLenum, const EGLint *);
EGLAPI EGLSurface EGLAPIENTRY PROXY_API(eglCreateWindowSurface, EGLDisplay, EGLConfig, EGLNativeWindowType, const EGLint *);
EGLAPI EGLint EGLAPIENTRY PROXY_API(eglDebugMessageControlKHR, EGLDEBUGPROCKHR, const EGLAttrib *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglDestroyContext, EGLDisplay, EGLContext);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglDestroyImage, EGLDisplay, EGLImage);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglDestroyImageKHR, EGLDisplay, EGLImageKHR);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglDestroyStreamKHR, EGLDisplay, EGLStreamKHR);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglDestroySurface, EGLDisplay, EGLSurface);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglDestroySync, EGLDisplay, EGLSync);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglDestroySyncKHR, EGLDisplay, EGLSyncKHR);
EGLAPI EGLint EGLAPIENTRY PROXY_API(eglDupNativeFenceFDANDROID, EGLDisplay, EGLSyncKHR);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglGetCompositorTimingANDROID, EGLDisplay, EGLSurface, EGLint, const EGLint *, EGLnsecsANDROID *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglGetCompositorTimingSupportedANDROID, EGLDisplay, EGLSurface, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglGetConfigAttrib, EGLDisplay, EGLConfig, EGLint, EGLint *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglGetConfigs, EGLDisplay, EGLConfig *, EGLint, EGLint *);
EGLAPI EGLContext EGLAPIENTRY PROXY_API(eglGetCurrentContext, int);
EGLAPI EGLDisplay EGLAPIENTRY PROXY_API(eglGetCurrentDisplay, int);
EGLAPI EGLSurface EGLAPIENTRY PROXY_API(eglGetCurrentSurface, EGLint);
EGLAPI EGLDisplay EGLAPIENTRY PROXY_API(eglGetDisplay, EGLNativeDisplayType);
EGLAPI EGLint EGLAPIENTRY PROXY_API(eglGetError, int);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglGetFrameTimestampsANDROID, EGLDisplay, EGLSurface, EGLuint64KHR, EGLint, const EGLint *, EGLnsecsANDROID *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglGetFrameTimestampSupportedANDROID, EGLDisplay, EGLSurface, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglGetMscRateANGLE, EGLDisplay, EGLSurface, EGLint *, EGLint *);
EGLAPI EGLClientBuffer EGLAPIENTRY PROXY_API(eglGetNativeClientBufferANDROID, const struct AHardwareBuffer *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglGetNextFrameIdANDROID, EGLDisplay, EGLSurface, EGLuint64KHR *);
EGLAPI EGLDisplay EGLAPIENTRY PROXY_API(eglGetPlatformDisplay, EGLenum, void *, const EGLAttrib *);
EGLAPI EGLDisplay EGLAPIENTRY PROXY_API(eglGetPlatformDisplayEXT, EGLenum, void *, const EGLint *);
EGLAPI __eglMustCastToProperFunctionPointerType EGLAPIENTRY PROXY_API(eglGetProcAddress, const char *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglGetSyncAttrib, EGLDisplay, EGLSync, EGLint, EGLAttrib *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglGetSyncAttribKHR, EGLDisplay, EGLSyncKHR, EGLint, EGLint *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglGetSyncValuesCHROMIUM, EGLDisplay, EGLSurface, EGLuint64KHR *, EGLuint64KHR *, EGLuint64KHR *);
EGLAPI void EGLAPIENTRY PROXY_API(eglHandleGPUSwitchANGLE, EGLDisplay);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglInitialize, EGLDisplay, EGLint *, EGLint *);
EGLAPI EGLint EGLAPIENTRY PROXY_API(eglLabelObjectKHR, EGLDisplay, EGLenum, EGLObjectKHR, EGLLabelKHR);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglMakeCurrent, EGLDisplay, EGLSurface, EGLSurface, EGLContext);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglPostSubBufferNV, EGLDisplay, EGLSurface, EGLint, EGLint, EGLint, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglPresentationTimeANDROID, EGLDisplay, EGLSurface, EGLnsecsANDROID);
EGLAPI EGLint EGLAPIENTRY PROXY_API(eglProgramCacheGetAttribANGLE, EGLDisplay, EGLenum);
EGLAPI void EGLAPIENTRY PROXY_API(eglProgramCachePopulateANGLE, EGLDisplay, const void *, EGLint, const void *, EGLint);
EGLAPI void EGLAPIENTRY PROXY_API(eglProgramCacheQueryANGLE, EGLDisplay, EGLint, void *, EGLint *, void *, EGLint *);
EGLAPI EGLint EGLAPIENTRY PROXY_API(eglProgramCacheResizeANGLE, EGLDisplay, EGLint, EGLint);
EGLAPI EGLenum EGLAPIENTRY PROXY_API(eglQueryAPI, int);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglQueryContext, EGLDisplay, EGLContext, EGLint, EGLint *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglQueryDebugKHR, EGLint, EGLAttrib *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglQueryDeviceAttribEXT, EGLDeviceEXT, EGLint, EGLAttrib *);
EGLAPI const char *EGLAPIENTRY PROXY_API(eglQueryDeviceStringEXT, EGLDeviceEXT, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglQueryDisplayAttribANGLE, EGLDisplay, EGLint, EGLAttrib *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglQueryDisplayAttribEXT, EGLDisplay, EGLint, EGLAttrib *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglQueryStreamKHR, EGLDisplay, EGLStreamKHR, EGLenum, EGLint *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglQueryStreamu64KHR, EGLDisplay, EGLStreamKHR, EGLenum, EGLuint64KHR *);
EGLAPI const char *EGLAPIENTRY PROXY_API(eglQueryString, EGLDisplay, EGLint);
EGLAPI const char *EGLAPIENTRY PROXY_API(eglQueryStringiANGLE, EGLDisplay, EGLint, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglQuerySurface, EGLDisplay, EGLSurface, EGLint, EGLint *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglQuerySurfacePointerANGLE, EGLDisplay, EGLSurface, EGLint, void **);
EGLAPI void EGLAPIENTRY PROXY_API(eglReacquireHighPowerGPUANGLE, EGLDisplay, EGLContext);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglReleaseDeviceANGLE, EGLDeviceEXT);
EGLAPI void EGLAPIENTRY PROXY_API(eglReleaseHighPowerGPUANGLE, EGLDisplay, EGLContext);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglReleaseTexImage, EGLDisplay, EGLSurface, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglReleaseThread, int);
EGLAPI void EGLAPIENTRY PROXY_API(eglSetBlobCacheFuncsANDROID, EGLDisplay, void *, void *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglSignalSyncKHR, EGLDisplay, EGLSyncKHR, EGLenum);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglStreamAttribKHR, EGLDisplay, EGLStreamKHR, EGLenum, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglStreamConsumerAcquireKHR, EGLDisplay, EGLStreamKHR);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglStreamConsumerGLTextureExternalAttribsNV, EGLDisplay, EGLStreamKHR, const EGLAttrib *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglStreamConsumerGLTextureExternalKHR, EGLDisplay, EGLStreamKHR);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglStreamConsumerReleaseKHR, EGLDisplay, EGLStreamKHR);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglStreamPostD3DTextureANGLE, EGLDisplay, EGLStreamKHR, void *, const EGLAttrib *);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglSurfaceAttrib, EGLDisplay, EGLSurface, EGLint, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglSwapBuffers, EGLDisplay, EGLSurface);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglSwapBuffersWithDamageKHR, EGLDisplay, EGLSurface, const EGLint *, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglSwapBuffersWithFrameTokenANGLE, EGLDisplay, EGLSurface, EGLFrameTokenANGLE);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglSwapInterval, EGLDisplay, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglTerminate, EGLDisplay);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglWaitClient, int);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglWaitGL, int);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglWaitNative, EGLint);
EGLAPI EGLBoolean EGLAPIENTRY PROXY_API(eglWaitSync, EGLDisplay, EGLSync, EGLint);
EGLAPI EGLint EGLAPIENTRY PROXY_API(eglWaitSyncKHR, EGLDisplay, EGLSyncKHR, EGLint);