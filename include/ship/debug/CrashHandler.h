#pragma once

#include <stddef.h>

#if (__linux__)
#include <csignal>
#include <cstdio>
#include <cxxabi.h> // for __cxa_demangle
#include <dlfcn.h>  // for dladdr
#include <execinfo.h>
#include <unistd.h>
#include <SDL.h>
#endif

#if _WIN32
#include <windows.h>
#endif

namespace Ship {
typedef void (*CrashHandlerCallback)(char*, size_t*);

class CrashHandler {
  public:
    CrashHandler();
    ~CrashHandler();
    CrashHandler(CrashHandlerCallback callback);

    void RegisterCallback(CrashHandlerCallback callback);
    void AppendLine(const char* str);
    void AppendStr(const char* str);
    void PrintCommon();

    /**
     * @brief Whether a SIGINT, SIGTERM or SIGQUIT has asked the process to shut down.
     *
     * The signal handler only records the request, since exit() is not async-signal-safe; the window
     * backend's event loop polls this and closes the window. Always false where no handler is installed.
     */
    static bool IsShutdownRequested();

#ifdef __linux__
    void PrintRegisters(ucontext_t* ctx);
#elif _WIN32
    void PrintRegisters(CONTEXT* ctx);
    void PrintStack(CONTEXT* ctx);
#endif

  private:
    CrashHandlerCallback mCallback = nullptr;
    char* mOutBuffer = nullptr;
    static constexpr size_t gMaxBufferSize = 32768;
    size_t mOutBuffersize = 0;

    void AppendStrTrunc(const char* str);

    bool CheckStrLen(const char* str);
};
} // namespace Ship
