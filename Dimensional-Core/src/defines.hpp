#ifndef DM_DEFINES
#define DM_DEFINES

// Core Defines

//----TYPES----

typedef unsigned char u8;

typedef unsigned short u16;

typedef unsigned int u32;

typedef unsigned long long u64;

// Signed int types.

typedef signed char i8;

typedef signed short i16;

typedef signed int i32;

typedef signed long long i64;

// Floating point types

typedef float f32;

typedef double f64;

//----HELPFUL----

// Thanks TheCherno!
#define BIT(x) (1 << x)

#define DM_EXPAND_MACRO(x) x
#define DM_STRINGIFY_MACRO(x) #x

//----SYSTEM INFO----
#if defined(_WIN32) || defined(_WIN64)
#define DM_WINDOWS
#define DM_PLATFORM "WINDOWS"

#elif defined(__linux__)
#define DM_LINUX
#define DM_PLATFORM "LINUX"

#elif defined(__APPLE__) && defined(__MACH__)
#define DM_MACOS
#define DM_PLATFORM "MACOS"

#else
#error UNKNOWN_PLATFORM
#endif
#endif

//----CORE-APP ARCH HELP----
#if defined(DM_WINDOWS)
#ifdef MYLIB_BUILD
#define DMCORE_API __declspec(dllexport)
#else
#define DMCORE_API __declspec(dllimport)
#endif
#elif defined(DM_LINUX)
#ifdef MYLIB_BUILD
#define DMCORE_API __attribute__((visibility("default")))
#else
#define DMCORE_API
#endif
#else
#define DMCORE_API
#endif
