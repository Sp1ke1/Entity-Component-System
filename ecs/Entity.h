
typedef unsigned long EntityIdType;

class Entity {

public:

    explicit Entity ( EntityIdType id );

    const EntityIdType GetId () const;

private:
    EntityIdType m_ID;
};

