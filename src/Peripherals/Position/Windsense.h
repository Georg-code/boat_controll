#ifndef WINDSENSE_H
#define WINDSENSE_H

class Windsense {
public:
    static Windsense* getInstance();  // Singleton access method

    void setup();
    int getAngle();
    int getAvgAngle();

private:
    Windsense() = default;  // Private constructor
    static Windsense* instance;
};

#endif  // WINDSENSE_H
