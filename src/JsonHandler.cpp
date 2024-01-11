#include "JsonHandler.h"

bool JsonHandler::parseJson(String jsonString, float &gps_lat, float &gps_lng, int wind_direction_raw)
{
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error)
    {
        Serial.println("Error parsing JSON");
        return false;
    }
    else
    {
        gps_lat = doc["gps_lat"];
        gps_lng = doc["gps_lng"];
        wind_direction_raw = doc["wind_rotation_raw"];
        return true;
    }
}
