#ifndef ENEMY_H
#define ENEMY_H

const int MAX_ENEMIES = 8;

struct Enemy
{
    int x;
    int y;

    int vx;
    int vy;

    bool active;
    uint8_t health;

    uint8_t shoot_cooldown;
};

#endif