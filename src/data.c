#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
const char logo_pal[]=
{
    0x0F, 0x00, 0x10, 0x30, 
    0x0F, 0x30, 0x30, 0x30, 
    0x0F, 0x30, 0x30, 0x30, 
    0x0F, 0x30, 0x30, 0x30, 
};
const char bg_pal[]=
{
    0x0F, 0x06, 0x15, 0x36, 
    0x0F, 0x30, 0x10, 0x0F, 
    0x0F, 0x00, 0x10, 0x30, 
    0x0F, 0x30, 0x30, 0x30, 
};
const char spr_pal[]=
{
    0x0F, 0x0F, 0x21, 0x11, 
    0x0F, 0x05, 0x16, 0x26, 
    0x0F, 0x1C, 0x39, 0x2B, 
    0x0F, 0x00, 0x30, 0x10, 
};
const char LOGO[]={
    0xA8, 0xC3, 0xA9, 0xC6, 0xCF, 0xC1, 0xCD, 0xC1, 0xCE, 0xC7, 0xC5, 0xCC, 0xB3, 0xB2
};
const char gameover_text[]="TRY AGAIN!";
const char win_text[]=" SUCCESS! ";
const unsigned char ship_self_Metasprite0_data[]={

	  8,  0,0x01,0|OAM_FLIP_H,
	  0,  0,0x01,0,
	 16,  0,0x0d,0,
	0x80

};

const unsigned char ship_self_Metasprite1_data[]={

	- 1,  2,0x07,0,
	  7,  2,0x09,0,
	 15,  2,0x0b,0,
	0x80

};

const unsigned char ship_self_Metasprite2_data[]={

	  0,  0,0x03,0,
	  8,  0,0x05,0,
	 16,  0,0x0d,0,
	0x80

};

const unsigned char ship_self_Metasprite3_data[]={

	- 1,- 1,0x07,0|OAM_FLIP_V,
	  7,- 1,0x09,0|OAM_FLIP_V,
	 15,- 1,0x0b,0|OAM_FLIP_V,
	0x80

};

const unsigned char ship_self_Metasprite4_data[]={

	  8,  0,0x01,0|OAM_FLIP_H|OAM_FLIP_V,
	  0,  0,0x01,0|OAM_FLIP_V,
	 16,  0,0x0d,0,
	0x80

};

const unsigned char ship_self_Metasprite5_data[]={

	 10,  2,0x07,0|OAM_FLIP_H,
	  2,  2,0x09,0|OAM_FLIP_H,
	- 6,  1,0x0b,0|OAM_FLIP_H,
	0x80

};

const unsigned char ship_self_Metasprite6_data[]={

	  8,  0,0x03,0|OAM_FLIP_H|OAM_FLIP_V,
	  0,  0,0x05,0|OAM_FLIP_H,
	 16,  0,0x0d,0,
	0x80

};

const unsigned char ship_self_Metasprite7_data[]={

	  9,- 1,0x07,0|OAM_FLIP_H|OAM_FLIP_V,
	  1,- 1,0x09,0|OAM_FLIP_H|OAM_FLIP_V,
	- 7,- 1,0x0b,0|OAM_FLIP_H|OAM_FLIP_V,
	0x80

};

const unsigned char* const ship_self_list[]={

	ship_self_Metasprite0_data,
	ship_self_Metasprite1_data,
	ship_self_Metasprite2_data,
	ship_self_Metasprite3_data,
	ship_self_Metasprite4_data,
	ship_self_Metasprite5_data,
	ship_self_Metasprite6_data,
	ship_self_Metasprite7_data

};

const unsigned char ship_fire_small_0_data[]={

	  0, 16,0x0f,1,
	  8, 16,0x0f,1|OAM_FLIP_H,
	128

};

const unsigned char ship_fire_small_1_data[]={

	- 3,  8,0x19,1,
	  5,  8,0x1b,1,
	128

};

const unsigned char ship_fire_small_2_data[]={

	- 8,  0,0x13,1,
	-16,  0,0x0d,1,
	128

};

const unsigned char ship_fire_small_3_data[]={

	  5,- 7,0x1b,1|OAM_FLIP_V,
	- 3,- 7,0x19,1|OAM_FLIP_V,
	128

};

const unsigned char ship_fire_small_4_data[]={

	  8,-16,0x0f,1|OAM_FLIP_H|OAM_FLIP_V,
	  0,-16,0x0f,1|OAM_FLIP_V,
	 16,-16,0x0d,1,
	128

};

const unsigned char ship_fire_small_5_data[]={

	 12,  8,0x19,1|OAM_FLIP_H,
	  4,  8,0x1b,1|OAM_FLIP_H,
	128

};

const unsigned char ship_fire_small_6_data[]={

	 16,  0,0x13,1|OAM_FLIP_H,
	 24,  0,0x0d,1,
	128

};

const unsigned char ship_fire_small_7_data[]={

	 11,- 7,0x19,1|OAM_FLIP_H|OAM_FLIP_V,
	  3,- 7,0x1b,1|OAM_FLIP_H|OAM_FLIP_V,
	128

};

const unsigned char* const ship_fire_small_list[]={

	ship_fire_small_0_data,
	ship_fire_small_1_data,
	ship_fire_small_2_data,
	ship_fire_small_3_data,
	ship_fire_small_4_data,
	ship_fire_small_5_data,
	ship_fire_small_6_data,
	ship_fire_small_7_data

};

const unsigned char ship_fire_big_Metasprite0_data[]={

	  0, 16,0x11,1,
	  8, 16,0x11,1|OAM_FLIP_H,
	0x80

};

const unsigned char ship_fire_big_Metasprite1_data[]={

	- 7,  9,0x15,1,
	  1,  9,0x17,1,
	0x80

};

const unsigned char ship_fire_big_Metasprite2_data[]={

	- 8,  0,0x1f,1,
	-16,  0,0x1d,1,
	0x80

};

const unsigned char ship_fire_big_Metasprite3_data[]={

	  1,- 9,0x17,1|OAM_FLIP_V,
	- 7,- 9,0x15,1|OAM_FLIP_V,
	0x80

};

const unsigned char ship_fire_big_Metasprite4_data[]={

	  0,-16,0x11,1|OAM_FLIP_V,
	  8,-16,0x11,1|OAM_FLIP_H|OAM_FLIP_V,
	0x80

};

const unsigned char ship_fire_big_Metasprite5_data[]={

	 14,- 9,0x15,1|OAM_FLIP_H|OAM_FLIP_V,
	  6,- 9,0x17,1|OAM_FLIP_H|OAM_FLIP_V,
	0x80

};

const unsigned char ship_fire_big_Metasprite6_data[]={

	 16,  0,0x1f,1|OAM_FLIP_H,
	 24,  0,0x1d,1|OAM_FLIP_H,
	0x80

};

const unsigned char ship_fire_big_Metasprite7_data[]={

	 17,  9,0x15,1|OAM_FLIP_H,
	  9,  9,0x17,1|OAM_FLIP_H,
	0x80

};

const unsigned char* const ship_fire_big_list[]={

	ship_fire_big_Metasprite0_data,
	ship_fire_big_Metasprite1_data,
	ship_fire_big_Metasprite2_data,
	ship_fire_big_Metasprite3_data,
	ship_fire_big_Metasprite4_data,
	ship_fire_big_Metasprite7_data,
	ship_fire_big_Metasprite6_data,
	ship_fire_big_Metasprite5_data,

};

const unsigned char bullet_Metasprite0_data[]={

	  0,  0,0x21,0,
	  0,  0,0x23,1,
	0x80

};

const unsigned char bullet_Metasprite1_data[]={

	  0,  0,0x21,0,
	- 8,- 2,0x25,1,
	  0,- 2,0x27,1,
	0x80

};

const unsigned char bullet_Metasprite2_data[]={

	  0,  0,0x21,0,
	  0,  0,0x29,1|OAM_FLIP_H,
	- 8,  0,0x2b,1|OAM_FLIP_H,
	0x80

};

const unsigned char bullet_Metasprite3_data[]={

	  0,  0,0x21,0,
	- 8,- 6,0x25,1|OAM_FLIP_V,
	  0,- 6,0x27,1|OAM_FLIP_V,
	0x80

};

const unsigned char bullet_Metasprite4_data[]={

	  0,  0,0x21,0,
	  0,- 8,0x23,1|OAM_FLIP_V,
	0x80

};

const unsigned char bullet_Metasprite5_data[]={

	  0,  0,0x21,0,
	  8,- 2,0x25,1|OAM_FLIP_H,
	  0,- 2,0x27,1|OAM_FLIP_H,
	0x80

};

const unsigned char bullet_Metasprite6_data[]={

	  0,  0,0x21,0,
	  0,  0,0x29,1,
	  8,  0,0x2b,1,
	0x80

};

const unsigned char bullet_Metasprite7_data[]={

	  0,  0,0x21,0,
	  7,- 6,0x25,1|OAM_FLIP_H|OAM_FLIP_V,
	- 1,- 6,0x27,1|OAM_FLIP_H|OAM_FLIP_V,
	0x80

};

const unsigned char* const bullet_list[]={

	bullet_Metasprite0_data,
	bullet_Metasprite1_data,
	bullet_Metasprite2_data,
	bullet_Metasprite3_data,
	bullet_Metasprite4_data,
	bullet_Metasprite5_data,
	bullet_Metasprite6_data,
	bullet_Metasprite7_data

};

const unsigned char hand_Metasprite0_data[]={

	  0,  0,0x33,1,
	  8,  0,0x35,1,
	0x80

};

const unsigned char hand_Metasprite1_data[]={

	  0,  0,0x33,1|OAM_FLIP_V,
	  8,  0,0x35,1|OAM_FLIP_V,
	0x80

};

const unsigned char* const hand_list[]={

	hand_Metasprite0_data,
	hand_Metasprite1_data

};

const unsigned char star_positons[16][4]={
	{0, 9, 18, 26},
	{6, 12, 54, 98},
	{12, 24, 58, 90},
	{39, 49, 60, 79},
	{100, 109, 118, 126},
	{106, 112, 154, 198},
	{112, 124, 158, 190},
	{139, 149, 160, 179},
	{200, 209, 218, 226},
	{206, 212, 254, 255},
	{212, 224, 258, 255},
	{220, 230, 240, 250},
	{10, 20, 230, 140},
	{70, 50, 10, 100},
	{15, 74, 144, 210},
	{139, 149, 160, 179},
};
const unsigned char ship_enemy0_Metasprite_UP_data[]={

	  8,  0,0x01,3|OAM_FLIP_H|OAM_FLIP_V,
	  0,  0,0x01,3|OAM_FLIP_V,
	0x80

};
const unsigned char ship_enemy0_Metasprite_DOWN_data[]={

	  8,  0,0x01,3|OAM_FLIP_H,
	  0,  0,0x01,3,
	0x80

};
const unsigned char ship_enemy0_Metasprite_UP_right_data[]={

	- 1,- 1,0x07,3|OAM_FLIP_V,
	  7,- 1,0x09,3|OAM_FLIP_V,
	 15,- 1,0x0b,3|OAM_FLIP_V,
	0x80
};

const unsigned char ship_enemy0_Metasprite_UP_left_data[]={

	  9,- 1,0x07,3|OAM_FLIP_H|OAM_FLIP_V,
	  1,- 1,0x09,3|OAM_FLIP_H|OAM_FLIP_V,
	- 7,- 1,0x0b,3|OAM_FLIP_H|OAM_FLIP_V,
	0x80
};

const unsigned char ship_enemy0_Metasprite_DOWN_left_data[]={

	 10,  2,0x07,3|OAM_FLIP_H,
	  2,  2,0x09,3|OAM_FLIP_H,
	- 6,  1,0x0b,3|OAM_FLIP_H,
	0x80
};

const unsigned char ship_enemy0_Metasprite_DOWN_right_data[]={

	- 1,  2,0x07,3,
	  7,  2,0x09,3,
	 15,  1,0x0b,3,
	0x80
};



const unsigned char explosion_Metasprite0_data[]={

	  0,  0,0x41,1,
	  8,  0,0x41,1|OAM_FLIP_H,
	0x80

};

const unsigned char explosion_Metasprite1_data[]={

	  0,  0,0x43,1,
	  8,  0,0x43,1|OAM_FLIP_H,
	0x80

};

const unsigned char* const explosion_list[]={

	explosion_Metasprite0_data,
	explosion_Metasprite1_data

};

const unsigned char ship_enemy_warn_Metasprite[]={

	  0,  0,0x45,1,
	  8,  0,0x45,1|OAM_FLIP_H,
	0x80
};



// 0-2: 0-2 type
// 3:   0, 1 mix
// 4:   1, 2 mix
// 5:   0, 2 mix
// 6:   0, 1, 2 mix
const unsigned char level_enemies[16][16]={
	{2, 2, 1, 3},
	{0, 0, 1, 2},
	{0, 0, 1, 2, 3},
	{4, 6, 1, 2, 3},
};
const unsigned char level_waves[]={
	0x33 //level0 and 1 is both 4 waves
};
const unsigned char level_enemy_needs_to_destroy[][16]={
	// {0, 0, 0, 0}
	{5, 10, 3, 16}
};
