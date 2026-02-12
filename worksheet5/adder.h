#pragma once

// 1) Static build: no dll import/export decorations
#ifdef maths_STATIC
  #define MATHSLIB_API
#else
  // 2) Shared build (DLL)
  #if defined(_WIN32) || defined(__CYGWIN__)
    #ifdef maths_EXPORTS
      // building the DLL
      #define MATHSLIB_API __declspec(dllexport)
    #else
      // using the DLL
      #define MATHSLIB_API __declspec(dllimport)
    #endif
  #else
    #define MATHSLIB_API
  #endif
#endif

// Function declaration
extern "C" MATHSLIB_API int add(int a, int b);
