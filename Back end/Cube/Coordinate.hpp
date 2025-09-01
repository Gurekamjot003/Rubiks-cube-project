#pragma once

class Coordinate{
    int x, y;
public:
    Coordinate(int x = 0, int y = 0): x(x), y(y){}

    const int get_x() &{
        return x;
    }

    const int get_y() &{
        return y;
    }
};