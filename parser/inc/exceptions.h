//
// Created by tooka on 20/10/2026.
//

#ifndef NTR_TOOLCHAIN_EXCEPTIONS_H
#define NTR_TOOLCHAIN_EXCEPTIONS_H
#include <exception>
#include <stdexcept>

#endif //NTR_TOOLCHAIN_EXCEPTIONS_H

class OutOfFile : public std::runtime_error{
public:
    explicit OutOfFile(const std::string& message)
        : std::runtime_error(message) {}
};

class IncorrectTokenType : public std::runtime_error {
public:
    explicit IncorrectTokenType(const std::string& message)
        : std::runtime_error(message) {}

};