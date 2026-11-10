#ifndef UPDATE_PROJ_H
#define UPDATE_PROJ_H

#define FP_SHIFT 8
#define FP_ONE (1 << FP_SHIFT)

#include "projectile.h"
#include "collision.h"

void UpdateProj(Projectile proj_pool[], int ship_x, int ship_y, uint8_t& ship_health)
{
    for(int i = 0; i < MAX_PROJ; i++)
    {
        Projectile& p = proj_pool[i];

        int proj_x;
        int proj_y;

        if(p.active)
        {
            proj_x = p.x >> FP_SHIFT;
            proj_y = p.y >> FP_SHIFT;

            p.x += p.vx;
            p.y += p.vy;

            if(!p.friendly)
            {
                int ship_draw_x = ship_x >> FP_SHIFT;
                int ship_draw_y = ship_y >> FP_SHIFT;

                if(CheckCollision(proj_x, proj_y, 8, 8, ship_draw_x + 4, ship_draw_y + 4, 8, 8))
                {
                    ship_health -= 1;
                    p.active = false;
                }
            }
        }
        if(p.active)
        {
            if(proj_x < -8 ||
                proj_x > 256 ||
                proj_y < -8 ||
                proj_y > 192)
            {
                p.active = false;
            }
        }
    }
}

#endif