// include/openai/error.h
#ifndef ERROR_H
#define ERROR_H

#pragma once
#include "openai_export.h"

#include <string>

namespace openai {

class OPENAI_EXPORT Error {
public:
    Error() = default;
    explicit Error(const std::string& message) : message(message) {}
    virtual ~Error() = default;
    std::string message;
};

class OPENAI_EXPORT APIKeyError : public Error {
public:
    APIKeyError() : Error("API key is missing or invalid.") {}
};

class OPENAI_EXPORT JSONParseError : public Error {
public:
    explicit JSONParseError(const std::string& msg) : Error("JSON parse error: " + msg) {}
};

class OPENAI_EXPORT NetworkError : public Error {
public:
    explicit NetworkError(const std::string& msg) : Error("Network error: " + msg) {}
};

} // namespace openai

#endif // ERROR_H
