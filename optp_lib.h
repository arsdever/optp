#pragma once

#ifndef BUILD_STATIC
#ifdef OPTP_LIBRARY_EXPORT
#define OPTP_LIB __declspec(dllexport)
#else
#define OPTP_LIB __declspec(dllimport)
#endif
#endif