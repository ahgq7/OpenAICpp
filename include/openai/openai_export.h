#ifndef OPENAI_EXPORT_H
#define OPENAI_EXPORT_H

#if defined(_WIN32)
#if defined(OPENAI_LIBRARY)
#define OPENAI_EXPORT __declspec(dllexport)
#else
#define OPENAI_EXPORT __declspec(dllimport)
#endif
#else
#define OPENAI_EXPORT
#endif

#endif // OPENAI_EXPORT_H
