//
// Created by jorni on 24.02.2025.
//

#ifndef BOATLOG_H
#define BOATLOG_H

#include <Arduino.h>
#include <string>

class BoatLog {


  public:
    static bool EnableInfo;
    static bool EnableError;
    static bool EnableDebug;
    static bool EnableNavigation;

    static void info(const std::string& str)
    {
        if (!EnableInfo) return;
        Serial.print("[INFO] ");
        Serial.println(str.c_str());
    }

    static void error(const std::string& str)
    {
        if (!EnableError) return;
        Serial.print("[ERROR] ");
        Serial.println(str.c_str());
    }

    static void debug(const std::string& str)
    {
        if (!EnableDebug) return;
        Serial.print("[DEBUG] ");
        Serial.println(str.c_str());
    }

    static void navigation(const std::string& str)
    {
        if (!EnableNavigation) return;
        Serial.print("[NAVIGATION] ");
        Serial.println(str.c_str());
    }
};



#endif //BOATLOG_H
