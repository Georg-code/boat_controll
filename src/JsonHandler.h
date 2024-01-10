#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <ArduinoJson.h>  // Include the ArduinoJson library

class JsonHandler {
public:
    static bool parseJson(String jsonString, int &gps_lat, int &gps_lng, int &wind_direction_raw, int &compass_azimuth);
};

#endif // JSON_HANDLER_H
