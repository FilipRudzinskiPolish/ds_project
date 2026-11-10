#include <nds.h>
#include <stdio.h>

#include "sprite.h"
#include "proj.h"

#include "collision.h"
#include "projectile.h"
#include "spawn_proj.h"
#include "update_proj.h"
#include "draw_proj.h"
#include "enemy.h"
#include "spawn_enemy.h"
#include "update_enemy.h"
#include "draw_enemy.h"

#define FP_SHIFT 8
#define FP_ONE (1 << FP_SHIFT)

const int speed = 2 * FP_ONE;
const int diagonal_speed = (speed * 181) >> FP_SHIFT;
int ship_x = 100 * FP_ONE;
int ship_y = 100 * FP_ONE;
uint8_t ship_health = 3;

int shoot_cooldown = 0;
int enemy_spawn_cooldown = 120;

/*
static void Vblank() {
	if(shoot_cooldown > 0)
	{
		shoot_cooldown -= 1;
	}
}
*/

const int SCREEN_LEFT = 0 << FP_SHIFT;
const int SCREEN_RIGHT = (256 - 16) << FP_SHIFT;
const int SCREEN_TOP = 0 << FP_SHIFT;
const int SCREEN_BOTTOM = (192 - 16) << FP_SHIFT;

int main(void) {
	/*
	irqSet(IRQ_VBLANK, Vblank);
	*/

	srand(REG_VCOUNT);

	consoleDemoInit();

	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_32, false);

	u16* sprite_gfx = oamAllocateGfx(
		&oamMain,
		SpriteSize_32x16,
		SpriteColorFormat_16Color
	);
	
	u16* proj_gfx = oamAllocateGfx(
		&oamMain,
		SpriteSize_8x8,
		SpriteColorFormat_16Color
	);
	
	dmaCopy(spriteTiles, sprite_gfx, spriteTilesLen);
	dmaCopy(spritePal, SPRITE_PALETTE, 32);
	
	dmaCopy(projTiles, proj_gfx, projTilesLen);
	dmaCopy(spritePal, &SPRITE_PALETTE[16], 32);
	SPRITE_PALETTE[16 + 1] = RGB15(31, 0, 0);

	Projectile proj_pool[MAX_PROJ];

	Enemy enemy_pool[MAX_ENEMIES];

	while(pmMainLoop()) {

		if(enemy_spawn_cooldown > 0)
		{
			enemy_spawn_cooldown -= 1;
		}else
		{
			Enemy* e = SpawnEnemy(enemy_pool);

			if(e)
			{
				enemy_spawn_cooldown = 120;

				e->x = ship_x;
				e->y = ship_y - (64 * FP_ONE);

				e->vx = 1 * FP_ONE;
				e->vy = 0;

				e->health = 5;
				e->shoot_cooldown = (rand() % 91) + 30;
			}
		}

		int dx = 0;
		int dy = 0;

		scanKeys();
		int keys_held = keysHeld();

		if(keys_held & KEY_START) break;
		if(keys_held & KEY_LEFT) dx--;
		if(keys_held & KEY_RIGHT) dx++;
		if(keys_held & KEY_UP) dy--;
		if(keys_held & KEY_DOWN) dy++;

		if(shoot_cooldown > 0)
		{
			shoot_cooldown -= 1;
		}

		if(keys_held & KEY_A && shoot_cooldown == 0)
		{
			shoot_cooldown = 10;

			Projectile* p = SpawnProj(proj_pool);

			if(p)
			{
				p->x = ship_x + (4 * FP_ONE);
				p->y = ship_y;

				p->vx = 0;
				p->vy = -3 * FP_ONE;
				p->friendly = true;
			}
		}

		if(dx != 0 && dy != 0){
			ship_x += dx * diagonal_speed;
			ship_y += dy * diagonal_speed;
		}else{
			ship_x += dx * speed;
			ship_y += dy * speed;
		}
		
		if(ship_x < SCREEN_LEFT) ship_x = SCREEN_LEFT;
		if(ship_x > SCREEN_RIGHT) ship_x = SCREEN_RIGHT;
		if(ship_y < SCREEN_TOP) ship_y = SCREEN_TOP;
		if(ship_y > SCREEN_BOTTOM) ship_y = SCREEN_BOTTOM;

		UpdateProj(proj_pool, ship_x, ship_y, ship_health);

		UpdateEnemy(enemy_pool, proj_pool, ship_x, ship_y);

		DrawProj(proj_pool, proj_gfx);

		DrawEnemy(enemy_pool, sprite_gfx);

		int draw_x = ship_x >> FP_SHIFT;
		int draw_y = ship_y >> FP_SHIFT;

		/*
		int active_proj = 0;
		for(int i = 0; i < MAX_PROJ; i++)
		{
			if(proj_pool[i].active)
			{
				active_proj += 1;
			}
		}

		int active_enemy = 0;
		for(int i = 0; i < MAX_ENEMIES; i++)
		{
			if(enemy_pool[i].active)
			{
				active_enemy += 1;
			}
		}
		*/

		//iprintf("\x1b[0;0HX: %d Y: %d proj amount: %d enemy amount: %d   ", draw_x, draw_y, active_proj, active_enemy);

		iprintf("\x1b[0;0HHealth: %d   ", ship_health);

		oamSet(&oamMain, 0, draw_x, draw_y, 0, 0, SpriteSize_16x16, SpriteColorFormat_16Color, sprite_gfx, 0, false, false, false, false, false);

		oamUpdate(&oamMain);

		swiWaitForVBlank();
	}

	return 0;
}
