#pragma once
#include <stdexcept>
#include <string>

class InvalidRegexException : public std::runtime_error {
public:
    explicit InvalidRegexException(const std::string& message)
        : std::runtime_error(message) {}
};
