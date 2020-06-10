#pragma once

#ifndef BUILD_STATIC
#ifdef _WIN32
#ifdef OPTP_LIBRARY_EXPORT
#define OPTP_LIB __declspec(dllexport)
#else
#define OPTP_LIB __declspec(dllimport)
#endif
#else
#define OPTP_LIB
#endif
#endif
