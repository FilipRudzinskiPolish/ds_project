#ifndef UPDATE_ENEMY_H
#define UPDATE_ENEMY_H

#define FP_SHIFT 8
#define FP_ONE (1 << FP_SHIFT)

#include "enemy.h"
#include "projectile.h"
#include "spawn_proj.h"
#include "collision.h"

void UpdateEnemy(Enemy enemy_pool[], Projectile proj_pool[], int ship_x, int ship_y)
{
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        Enemy& e = enemy_pool[i];
        
        if(e.active)
        {
            for(int i2 = 0; i2 < MAX_PROJ; i2++)
            {
                Projectile& p = proj_pool[i2];

                if(p.active and p.friendly)
                {
                    if(CheckCollision(e.x, e.y, 16 * FP_ONE, 16 * FP_ONE, p.x, p.y, 8 * FP_ONE, 8 * FP_ONE))
                    {
                        p.active = false;
                        
                        if(e.health == 1)
                        {
                            e.active = false;
                            break;
                        }else
                        {
                            e.health -= 1;
                        }
                    }
                }
            }
        }
        
        if(e.active)
        {
            e.x += e.vx;
            e.y += e.vy;

            if((e.x >> FP_SHIFT) < 0 ||
                (e.x >> FP_SHIFT) > 256 - 16)
            {
                e.vx *= -1;
            }

            e.shoot_cooldown -= 1;

            if(e.shoot_cooldown == 0)
            {
                e.shoot_cooldown = 120;

                Projectile* p = SpawnProj(proj_pool);

                if(p)
                {
                    p->x = e.x + (4 * FP_ONE);
                    p->y = e.y;

                    p->vx = (ship_x - e.x) / 70;
                    p->vy = (ship_y - e.y) / 70;
                    p->friendly = false;
                }
            }
        }
    }
}

#endif