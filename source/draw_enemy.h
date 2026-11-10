#ifndef DRAW_ENEMY_H
#define DRAW_ENEMY_H

#define FP_SHIFT 8
#define FP_ONE (1 << FP_SHIFT)

#include "nds.h"

#include "enemy.h"

void DrawEnemy(Enemy enemy_pool[], u16* sprite_gfx)
{
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        Enemy& e = enemy_pool[i];

        int sprite_id = i + 64;

        if(!e.active)
        {

            oamClearSprite(&oamMain, sprite_id);

        }else{

            oamSet(&oamMain, sprite_id, e.x >> FP_SHIFT, e.y >> FP_SHIFT, 1, 0, SpriteSize_16x16, SpriteColorFormat_16Color, sprite_gfx + 64, 0, false, false, false, false, false);

        }
    }
}

#endif