#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <ArduinoJson.h> // Include the ArduinoJson library

class JsonHandler
{
public:
    static bool parseJson(String jsonString, float &gps_lat, float &gps_lng, int &wind_direction_raw);
};

#endif // JSON_HANDLER_H
