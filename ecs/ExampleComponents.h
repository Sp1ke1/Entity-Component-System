
struct Vector
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

class LocationComponent
{
    Vector Location;
};

class MovementComponent
{
    float Speed = 0.f;
    Vector Direction;
};

class HPComponent
{
    int MaxHP = 100;
    int CurrentHP = 100;
};