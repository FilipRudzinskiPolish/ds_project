#ifndef SPAWN_PROJ_H
#define SPAWN_PROJ_H

#include "projectile.h"

Projectile* SpawnProj(Projectile proj_pool[])
{
    for(int i = 0; i < MAX_PROJ; i++)
    {
        if(!proj_pool[i].active)
        {
            proj_pool[i].active = true;
            return &proj_pool[i];
        }
    }

    return nullptr;
}

#endif