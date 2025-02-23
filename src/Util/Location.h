//
// Created by georg on 20.02.2025.
//

#ifndef LOCATION_H
#define LOCATION_H



class Location {


  private:
    double longitude;
    double latitude;

    public:
      Location(double longitude, double latitude) {
          this->longitude = longitude;
          this->latitude = latitude;
      }

      double getLat() {
          return latitude;
      }

      double getLong() {
            return longitude;
      }



};



#endif //LOCATION_H
