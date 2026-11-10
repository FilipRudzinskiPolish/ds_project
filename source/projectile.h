#ifndef PROJECTILE_H
#define PROJECTILE_H

const int MAX_PROJ = 64;

struct Projectile
{
    int x;
    int y;

    int vx;
    int vy;

    bool friendly;
    bool active;

    void Move()
    {
        x += vx;
        y += vy;
    }
};

#endif