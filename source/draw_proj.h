#ifndef DRAW_PROJ_H
#define DRAW_PROJ_H

#define FP_SHIFT 8
#define FP_ONE (1 << FP_SHIFT)

#include "nds.h"

#include "projectile.h"

void DrawProj(Projectile proj_pool[], u16* proj_gfx)
{
    for(int i = 0; i < MAX_PROJ; i++)
    {
        Projectile& p = proj_pool[i];

        int sprite_id = i + 1;

        if(!p.active)
        {

            oamClearSprite(&oamMain, sprite_id);

        }else{

            oamSet(&oamMain, sprite_id, p.x >> FP_SHIFT, p.y >> FP_SHIFT, 2, !p.friendly, SpriteSize_8x8, SpriteColorFormat_16Color, proj_gfx, 0, false, false, false, false, false);

        }
    }
}

#endif