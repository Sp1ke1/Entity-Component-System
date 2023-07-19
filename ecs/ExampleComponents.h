
struct Vector
{
    Vector ( float x, float y, float z )
    : X ( x ), Y ( x ), Z ( z )
    {};
    float X = 0.f;
    float Y = 0.f;
    float Z = 0.f;
};

struct LocationComponent
{
    LocationComponent ( Vector location )
    : Location ( location )
    {};
    Vector Location;
};

struct MovementComponent
{
    MovementComponent ( float speed, Vector direction )
    : Speed ( speed ), Direction( direction )
    {};
    float Speed = 0.f;
    Vector Direction;
};

struct HPComponent
{
    HPComponent ( int maxHp, int currentHp )
    : MaxHP ( maxHp ), CurrentHP ( currentHp )
    {};
    int MaxHP = 100;
    int CurrentHP = 100;
};