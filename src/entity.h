#ifndef ENTITY_H
#define ENTITY_H

class Entity {
    Dimension* dimension;
    double x, y, z;
public:
    void setDimension(Dimension* dimension);
    void setDimension(std::string dimension);

    Entity();
};

#endif
