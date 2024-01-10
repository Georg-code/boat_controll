#include "JsonHandler.h"

bool JsonHandler::parseJson(String jsonString, int &gps_lat, int &gps_lng, int &wind_direction_raw, int &compass_azimuth) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error) {
        Serial.println("Error parsing JSON");
        return false;
    } else {
        gps_lat = doc["gps_lat"];
        gps_lng = doc["gps_lng"];
        wind_direction_raw = doc["wind_direction_raw"];
        compass_azimuth = doc["compass_azimuth"];
        return true;
    }
}
