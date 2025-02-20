//
// Created by georg on 13.02.2025.
//

#ifndef ACTUATOR_H
#define ACTUATOR_H



class Actuator {
    int current_position;
    int max_position;
    int min_position;

    public:
    Actuator(int max_position, int min_position) {
        this->max_position = max_position;
        this->min_position = min_position;
        this->current_position = 0;
    }

    void set_position(int position) {
        if (position > max_position) {
            position = max_position;
        }
        if (position < min_position) {
            position = min_position;
        }
        this->current_position = position;
    }

    int get_position() {
        return this->current_position;
    }
};



#endif //ACTUATOR_H
