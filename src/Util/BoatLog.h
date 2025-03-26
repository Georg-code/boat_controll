//
// Created by jorni on 24.02.2025.
//

#ifndef BOATLOG_H
#define BOATLOG_H

#include <Arduino.h>
#include <string>
#include <sstream>

class BoatLog {
public:
    static bool EnableInfo;
    static bool EnableError;
    static bool EnableDebug;
    static bool EnableNavigation;

    template<typename... Args>
    static void info(Args&&... args) {
        if (!EnableInfo) return;
        log("[INFO] ", std::forward<Args>(args)...);

    }

    template<typename... Args>
    static void error(Args&&... args) {
        if (!EnableError) return;
        log("[ERROR] ", std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void debug(Args&&... args) {
        if (!EnableDebug) return;
        log("[DEBUG] ", std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void navigation(Args&&... args) {
        if (!EnableNavigation) return;
        log("[NAVIGATION] ", std::forward<Args>(args)...);
    }

private:
    template<typename... Args>
    static void log(const std::string& prefix, Args&&... args) {
        std::ostringstream oss;
        (oss << ... << std::forward<Args>(args)); // Fold expression to concatenate arguments
        Serial.print(prefix.c_str());
        Serial.println(oss.str().c_str());
    }
};

#endif // BOATLOG_H
