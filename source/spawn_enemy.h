#ifndef SPAWN_ENEMY_H
#define SPAWN_ENEMY_H

#include "enemy.h"

Enemy* SpawnEnemy(Enemy enemy_proj[])
{
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        if(!enemy_proj[i].active)
        {
            enemy_proj[i].active = true;
            return &enemy_proj[i];
        }
    }

    return nullptr;
}

#endif