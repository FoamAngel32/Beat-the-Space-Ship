#include "MUSIC/famistudio_cc65.h"
#include "gfx/title.h"
#include "gfx/bg_star.h"
#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
#include "data.c"

#define BULLET_ON_SCREEN_SPEED 5
#define SCORE_BAR_SIZE 40
#define ENEMY_MAX 6

#define SONG_DEEP_SPACE 0
typedef struct
{
    char used;
    int x, y;
    signed char dir;
} Bullet_t;
typedef struct
{
    char used;
    int x, y;
    unsigned char self_timer;
    unsigned char type;
    signed char speed;
    signed char sub_speed;
} Enemy_t;
#pragma bss-name(push, "ZEROPAGE")
#pragma data-name(push, "ZEROPAGE")
signed char player_speed_v, player_speed_h;
signed char player_speed_sub_v, player_speed_sub_h;
signed char temp0, temp1;
unsigned char temp2, temp3;
unsigned char update_score_if;
int temp4;
char timer0, anim_timer0;
int pressed_b;
signed int last_camera_y;
signed int scroll_y;
signed int camera_y;
char state;
signed char temp_offset_x = 0, temp_offset_y = 0;
char star_state;
unsigned char wave;
unsigned char level;
unsigned char enemy_spawn_timer;
unsigned char enemy_spawn_timer_temp;
unsigned char enemy_destroyed;
signed char direction_wants[3];
unsigned char hand_flash;
unsigned char score[2][7];
unsigned char display_score[2][7];
#pragma bss-name(pop)
#pragma data-name(pop)
signed char camera_extra_moving;
unsigned char camera_extra_moving_timer;
unsigned char padHeld;
unsigned char life;
int max_player_speed;
int player_pos[2];
char star_buffer[62];
Enemy_t enemies[ENEMY_MAX];
Bullet_t bullet_0;
Bullet_t bullet_1;
signed char player_dir;
signed char player_dir_update;
signed char player_dir_target;
#define DIR_0 0
#define DIR_45 1
#define DIR_90 2
#define DIR_135 3
#define DIR_180 4
#define DIR_225 -3
#define DIR_270 -2
#define DIR_315 -1
void draw_enemy(Enemy_t *enemy);
unsigned char update_enemy(Enemy_t *enemy);
void spawn_enemy(void);
void draw_one_row_star_buffer(int);
void if_not_in_the_right_direction(void);
void cal_speed(signed char *speed, signed char *sub_speed);
void init_bullet(Bullet_t *);
void generate_bullet(Bullet_t *);
void update_bullet(Bullet_t *);
void draw_bullet(Bullet_t *);
void init_logo(void);
void update_logo(void);
void init_title(void);
void update_title(void);
void init_main_game(void);
void update_main_game(void);
void draw_star(void);
void update_score(void);
void main(void)
{
    ppu_off();
    bank_spr(1);
    set_vram_buffer();
    init_logo();
    while (1)
    {
        ppu_wait_nmi();
        padHeld = pad_poll(0);
        if (state == 0)
            update_title();
        else if (state == 1)
            update_main_game();
    }
}
void init_logo(void)
{
    state = 0xFF;
    timer0 = 0;
    ppu_on_all();
    pal_bg(logo_pal);
    ppu_wait_nmi();
    famistudio_sfx_play(2, FAMISTUDIO_SFX_CH0);
    multi_vram_buffer_horz(LOGO, 14, 0x21C8);
    pal_bright(0);
    pal_fade_to(0, 4);
    delay(255 / 2);
    pal_fade_to(4, 0);
    famistudio_music_play(SONG_DEEP_SPACE);
    ppu_wait_nmi();
    for(temp0 = 0;temp0<7;++temp0)
        score[1][temp0] = score[0][temp0] = 0;
    init_title();
}
void init_title(void)
{
    state = 0;
    last_camera_y = camera_y = scroll_y = 0;
    player_pos[0] = player_pos[1] = 0;
    wave = 0;
    level = 0;
    enemy_destroyed = 0;
    camera_extra_moving = 0;
    camera_extra_moving_timer = 0;
    player_speed_v = player_speed_h = 0;
    player_speed_sub_v = player_speed_sub_h = 0;
    life = 0;
    temp3 = 0;
    ppu_off();
    ppu_wait_nmi();
    for (temp0 = 0; temp0 < 15; ++temp0)
    {
        draw_one_row_star_buffer(NTADR_B(0, (temp0 * 2)));
        flush_vram_update2();
        if(temp0 < 7)
        {
            score[0][temp0] = 0;
            display_score[0][temp0] = 0xB0;
            display_score[1][temp0] = score[1][temp0] + 0xB0;
        }
        if(temp0 < ENEMY_MAX)
            enemies[temp0].used = 0;
    }
    vram_adr(NAMETABLE_B + 960);
    for (temp0 = 0; temp0 < 64; ++temp0)
        vram_put(0b01010101);
    ppu_wait_nmi();
    pal_bg(bg_pal);
    vram_adr(NAMETABLE_A);
    vram_unrle(title);
    multi_vram_buffer_horz(display_score[1], 7, 0x2016);
    ppu_on_all();
    ppu_wait_nmi();
    set_scroll_y(-4);
    pal_fade_to(0, 4);
    oam_clear();
    oam_set(0);
    oam_spr(248, 96, 0xFF, 3);
}
void update_title(void)
{
    xy_split(256, scroll_y);
    oam_clear();
    oam_set(0);
    oam_spr(248, 96, 0xFF, 3);
    scroll_y = add_scroll_y(scroll_y, 1);
    set_scroll_x(0);
    set_scroll_y(-4);
    ++anim_timer0;
    draw_star();
    if (scroll_y % 16 == 0)
    {
        temp0 = ((scroll_y / 8) - 2);
        if (temp0 < 30)
            draw_one_row_star_buffer(NTADR_D(0, temp0));
    }
    if (padHeld & PAD_START)
    {
        famistudio_sfx_play(0, FAMISTUDIO_SFX_CH0);
        oam_clear();
        scroll(256, scroll_y);
        pal_bright(0);
        init_main_game();
    }
}
void init_main_game(void)
{
    for (temp0 = 0; temp0 < ENEMY_MAX; ++temp0)
        enemies[temp0].used = 0;
    state = 1;
    wave = level = enemy_spawn_timer = 0;
    scroll_y = 0;
    pal_spr(spr_pal);
    pal_col(1, 0xF);
    pal_col(2, 0xF);
    pal_col(3, 0xF);
    oam_size(1);
    player_pos[1] = 0;
    player_pos[0] = (255-16-16) / 2;
    ppu_off();
    ppu_wait_nmi();
    vram_adr(NAMETABLE_B + 31);
    vram_inc(1);
    vram_fill(0xFF, 30);
    vram_inc(0);
    ppu_wait_nmi();
    ppu_on_all();
    ppu_wait_nmi();
    max_player_speed = 10;
    init_bullet(&bullet_0);
    init_bullet(&bullet_1);
    direction_wants[0] = DIR_0;
    direction_wants[1] = DIR_315;
    direction_wants[2] = DIR_45;
    set_scroll_x(256);
    set_scroll_y(0);
    pal_fade_to(0, 4);
    oam_clear();
    oam_set(0);
    oam_spr(256 - 8, 240 - SCORE_BAR_SIZE, 0xFF, 0);
}
void update_main_game(void)
{
    last_camera_y = camera_y;
    oam_clear();
    oam_set(0);
    oam_spr(256 - 8, 240 - SCORE_BAR_SIZE, 0xFF, 0);
    ++anim_timer0;
    draw_star();
    if ((anim_timer0 & 0b1) == 0)
        cal_speed(&player_speed_h, &player_speed_sub_h);
    else
        cal_speed(&player_speed_v, &player_speed_sub_v);
    if (player_speed_v > max_player_speed)
        --player_speed_sub_v;
    else if (player_speed_v < -max_player_speed)
        ++player_speed_sub_v;
    if (player_speed_h > 3)
        player_speed_sub_h -= 2;
    else if (player_speed_h < -3)
        player_speed_sub_h += 2;
    if (padHeld & PAD_A)
    {
        if (padHeld & PAD_LEFT)
        {
            if (padHeld & PAD_UP)
                player_dir_target = DIR_315;
            else if (padHeld & PAD_DOWN)
                player_dir_target = DIR_225;
            else
                player_dir_target = DIR_270;
        }
        else if (padHeld & PAD_RIGHT)
        {
            if (padHeld & PAD_UP)
                player_dir_target = DIR_45;
            else if (padHeld & PAD_DOWN)
                player_dir_target = DIR_135;
            else
                player_dir_target = DIR_90;
        }
        else
        {
            if (padHeld & PAD_UP)
            {
                player_dir_target = DIR_0;
            }
            else if (padHeld & PAD_DOWN)
            {
                player_dir_target = DIR_180;
            }
        }
        if (player_dir == DIR_45 || player_dir == DIR_90 || player_dir == DIR_135)
            player_speed_sub_h += 2;
        else if (player_dir == DIR_315 || player_dir == DIR_270 || player_dir == DIR_225)
            player_speed_sub_h -= 2;
        else
        {
            if (player_speed_h > 0)
                --player_speed_sub_h;
            else if (player_speed_h < 0)
                ++player_speed_sub_h;
        }
        if (player_dir == DIR_0 || player_dir == DIR_45 || player_dir == DIR_315)
            --player_speed_sub_v;
        else if (player_dir == DIR_180 || player_dir == DIR_135 || player_dir == DIR_225)
            ++player_speed_sub_v;
    }
    else
    {
        if (player_speed_v > 0)
            --player_speed_sub_v;
        else if (player_speed_v < 0)
            ++player_speed_sub_v;
        if (player_speed_h > 0)
            --player_speed_sub_h;
        else if (player_speed_h < 0)
            ++player_speed_sub_h;
    }
    if (padHeld & PAD_B)
    {
        if (pressed_b != 1)
        {
            pressed_b = 1;
            temp0 = 0;
            if (bullet_0.used == 0)
                generate_bullet(&bullet_0);
            else if (bullet_1.used == 0)
                generate_bullet(&bullet_1);
            else
                temp0 = 1;
            if (temp0 == 0)
                famistudio_sfx_play(0, FAMISTUDIO_SFX_CH0);
        }
    }
    else
    {
        pressed_b = 0;
    }
    ++player_dir_update;
    if ((player_dir_update & 0b111) == 0)
    {
        temp0 = player_dir_target - player_dir;
        if (temp0 > 4)
            temp0 -= 8;
        else if (temp0 < -4)
            temp0 += 8;
        if (temp0 > 0)
            ++player_dir;
        else if (temp0 < 0)
            --player_dir;
        if (player_dir > 4)
            player_dir = -3;
        else if (player_dir < -3)
            player_dir = 4;
    }
    player_pos[0] += player_speed_h;
    player_pos[1] += player_speed_v;

    if ((player_dir == direction_wants[0]) || (player_dir == direction_wants[1] || (player_dir == direction_wants[2])))
    {
        if (direction_wants[0] == DIR_0)
        {
            if (player_speed_v <= -((max_player_speed / 3) * 2))
            {
                ++camera_extra_moving_timer;
                hand_flash = 0;
                if (camera_extra_moving_timer >= 120)
                {
                    camera_extra_moving_timer = 120;
                    --camera_extra_moving;
                    if (camera_extra_moving < -80)
                    {
                        spawn_enemy();
                        camera_extra_moving = -80;
                    }
                }
            }
            else
                if_not_in_the_right_direction();
        }
        else if (direction_wants[0] == DIR_180)
        {
            if (player_speed_v >= (max_player_speed / 3) * 2)
            {
                hand_flash = 0;
                ++camera_extra_moving_timer;
                if (camera_extra_moving_timer >= 120)
                {
                    camera_extra_moving_timer = 120;
                    ++camera_extra_moving;
                    if (camera_extra_moving > 80)
                    {
                        spawn_enemy();
                        camera_extra_moving = 80;
                    }
                }
            }
            else
                if_not_in_the_right_direction();
        }
        else
            if_not_in_the_right_direction();
    }
    else
        if_not_in_the_right_direction();
    camera_y = (player_pos[1] - ((240 - 16) / 2)) + camera_extra_moving;
    if (last_camera_y < camera_y)
        scroll_y = add_scroll_y(camera_y - last_camera_y, scroll_y);
    else if (last_camera_y > camera_y)
        scroll_y = sub_scroll_y(last_camera_y - camera_y, scroll_y);
    set_scroll_x(256);
    set_scroll_y(scroll_y);
    if (player_pos[0] < 20)
        player_pos[0] = 20;
    if (player_pos[0] > 256 - 20 - 16)
        player_pos[0] = 256 - 20 - 16;
    update_bullet(&bullet_0);
    update_bullet(&bullet_1);
    if (player_speed_v != 0)
    {
        temp2 = ((scroll_y / 8) - 2);
        if ((temp2 >= 0) && (temp2 < 30))
            draw_one_row_star_buffer(NTADR_D(0, temp2));
    }
    if ((hand_flash & 0b11111) > 16)
    {
        if (direction_wants[0] == DIR_0)
            oam_meta_spr(220, 12, hand_list[0]);
        else
            oam_meta_spr(220, 12, hand_list[1]);
    }
    else
        oam_spr(0xff, 0xff, 0xFD, 0);
    if (player_speed_h == 0)
        temp0 = MAX(player_speed_v, -player_speed_v);
    else if (player_speed_v == 0)
        temp0 = MAX(player_speed_h, -player_speed_h);
    else
        temp0 = (MAX(player_speed_h, -player_speed_h) + MAX(player_speed_v, -player_speed_v));
    temp1 = max_player_speed / 4;
    if ((anim_timer0 & 0b1) == 1 && temp0 >= temp1 * 1)
    {
        if (player_dir >= 0)
            oam_meta_spr(player_pos[0], player_pos[1] - camera_y, ship_fire_small_list[player_dir]);
        else
            oam_meta_spr(player_pos[0], player_pos[1] - camera_y, ship_fire_small_list[4 - player_dir]);
    }
    else if ((anim_timer0 & 0b1) == 0 && temp0 > temp1 * 3)
    {
        if (player_dir >= 0)
            oam_meta_spr(player_pos[0], player_pos[1] - camera_y, ship_fire_big_list[player_dir]);
        else
            oam_meta_spr(player_pos[0], player_pos[1] - camera_y, ship_fire_big_list[4 - player_dir]);
    }
    temp2 = 0;
    update_score_if = 0;
    temp4 = 0;
    for (temp0 = 0; temp0 < ENEMY_MAX; ++temp0)
    {
        temp4 = update_enemy(&enemies[temp0]);
        if(temp4 == 1)break;
        if(temp4 == 0 && update_score_if == 0)update_score_if = 1;
        if (enemy_destroyed > level_enemy_needs_to_destroy[level & 0b1111][wave])
        {
            enemy_destroyed = 0;
            wave += 1;
            if (!(wave & 1))
            {
                direction_wants[0] = DIR_0;
                direction_wants[1] = DIR_315;
                direction_wants[2] = DIR_45;
            }
            else
            {
                direction_wants[0] = DIR_180;
                direction_wants[1] = DIR_135;
                direction_wants[2] = DIR_225;
            }
            if (!(level & 1))
                temp1 = level_waves[(level & 0b1111) >> 1] >> 4;
            else
                temp1 = level_waves[(level & 0b1111) >> 1] & 0b00001111;
            if (wave > temp1)
            {
                temp2 = 1;
                break;
            }
        }
        draw_enemy(&enemies[temp0]);
    }
    if(update_score_if == 1)
        update_score();
    
    draw_bullet(&bullet_0);
    draw_bullet(&bullet_1);
    xy_split(0, -4);
    if(temp4 == 1)
    {
        pal_fade_to(4, 0);
        if(life > 0)
        {
            life -= 1;
            wave = 0;
            ppu_off();
            oam_clear();
            oam_set(0);
            oam_spr(256 - 8, 240 - SCORE_BAR_SIZE, 0xFF, 0);
            vram_adr(0x21CB);
            for(temp0 = 0;temp0<10;++temp0)
                vram_put(gameover_text[temp0]+0x80);
            vram_adr(0x23DA);
            vram_put(0xAA);
            vram_put(0xAA);
            vram_put(0xAA);
            vram_put(0xAA);
            ppu_on_all();
            set_scroll_x(0);
            set_scroll_y(0);
            pal_fade_to(0, 4);
            delay(255);
            pal_fade_to(4, 0);
            set_scroll_x(256);
            set_scroll_y(scroll_y);
        }else{
            init_title();
            update_title();
        }
        pal_fade_to(0, 4);
        for (temp0 = 0; temp0 < ENEMY_MAX; ++temp0)
            enemies[temp0].used = 0;
    }
    else if (temp2)
    {
        pal_fade_to(4, 0);
        camera_y = 0;
        player_pos[1] = 0;
        wave = 0;
        level += 1;
        ppu_off();
        oam_clear();
        oam_set(0);
        oam_spr(256 - 8, 240 - SCORE_BAR_SIZE, 0xFF, 0);
        vram_adr(0x21CB);
        for(temp0 = 0;temp0<10;++temp0)
            vram_put(win_text[temp0]+0x80);
        vram_adr(0x23DA);
        vram_put(0xAA);
        vram_put(0xAA);
        vram_put(0xAA);
        vram_put(0xAA);
        ppu_on_all();
        set_scroll_x(0);
        set_scroll_y(0);
        pal_fade_to(0, 4);
        delay(255);
        pal_fade_to(4, 0);
        set_scroll_x(256);
        set_scroll_y(scroll_y);
        for (temp0 = 0; temp0 < ENEMY_MAX; ++temp0)
            enemies[temp0].used = 0;
        pal_fade_to(0, 4);
    }

    if (player_dir >= 0)
        oam_meta_spr(player_pos[0], player_pos[1] - camera_y, ship_self_list[player_dir]);
    else
        oam_meta_spr(player_pos[0], player_pos[1] - camera_y, ship_self_list[4 - player_dir]);

}
void init_bullet(Bullet_t *bullet)
{
    bullet->used = 0;
    bullet->x = bullet->y = 0;
    bullet->dir = DIR_0;
}
void generate_bullet(Bullet_t *bullet)
{
    if (bullet->used == 0)
    {
        bullet->used = 1;
        temp_offset_x = temp_offset_y = 0;
        if (player_dir == DIR_0)
        {
            temp_offset_x = 4;
            temp_offset_y = -8;
        }
        else if (player_dir == DIR_45)
            temp_offset_x = 12;
        else if (player_dir == DIR_90)
        {
            temp_offset_x = 12;
            temp_offset_y = 4;
        }
        else if (player_dir == DIR_135)
        {
            temp_offset_x = 10;
            temp_offset_y = 10;
        }
        else if (player_dir == DIR_180)
        {
            temp_offset_x = 4;
            temp_offset_y = 12;
        }
        else if (player_dir == DIR_225)
        {
            temp_offset_x = -2;
            temp_offset_y = 12;
        }
        else if (player_dir == DIR_270)
        {
            temp_offset_x = -2;
            temp_offset_y = 4;
        }
        bullet->x = player_pos[0] + temp_offset_x;
        bullet->y = player_pos[1] + temp_offset_y;
        bullet->dir = player_dir;
    }
}
void update_bullet(Bullet_t *bullet)
{
    if (bullet->used == 1)
    {
        if (bullet->dir == DIR_0)
            bullet->y -= (-player_speed_v) + BULLET_ON_SCREEN_SPEED;
        else if (bullet->dir == DIR_45)
        {
            bullet->x += BULLET_ON_SCREEN_SPEED;
            bullet->y -= (-player_speed_v) + BULLET_ON_SCREEN_SPEED;
        }
        else if (bullet->dir == DIR_90)
        {
            bullet->x += BULLET_ON_SCREEN_SPEED;
            bullet->y += player_speed_v;
        }
        else if (bullet->dir == DIR_135)
        {
            bullet->x += BULLET_ON_SCREEN_SPEED;
            bullet->y += player_speed_v + BULLET_ON_SCREEN_SPEED;
        }
        else if (bullet->dir == DIR_180)
            bullet->y += player_speed_v + BULLET_ON_SCREEN_SPEED;
        else if (bullet->dir == DIR_225)
        {
            bullet->x -= BULLET_ON_SCREEN_SPEED;
            bullet->y += player_speed_v + BULLET_ON_SCREEN_SPEED;
        }
        else if (bullet->dir == DIR_270)
        {
            bullet->x -= BULLET_ON_SCREEN_SPEED;
            bullet->y += player_speed_v;
        }
        else if (bullet->dir == DIR_315)
        {
            bullet->x -= BULLET_ON_SCREEN_SPEED;
            bullet->y -= (-player_speed_v) + BULLET_ON_SCREEN_SPEED;
        }

        if (bullet->x >= 256 - 16)
            bullet->used = 0;
        else if (bullet->x <= 0)
            bullet->used = 0;
        if (bullet->y - camera_y >= 240)
            bullet->used = 0;
        else if (bullet->y - camera_y <= 0)
            bullet->used = 0;
    }
}
void draw_bullet(Bullet_t *bullet)
{
    if (bullet->used == 1)
    {
        if ((anim_timer0 & 0b11) >= 1)
        {
            if (bullet->dir >= 0)
                oam_meta_spr(bullet->x, bullet->y - camera_y, bullet_list[bullet->dir]);
            else
                oam_meta_spr(bullet->x, bullet->y - camera_y, bullet_list[4 - bullet->dir]);
        }
        else
            oam_spr(bullet->x, bullet->y - camera_y, 0x21, 0);
    }
}
void cal_speed(signed char *speed, signed char *sub_speed)
{
    if (*sub_speed >= 10)
    {
        *sub_speed = 0;
        ++*speed;
    }
    else if (*sub_speed <= -10)
    {
        *sub_speed = 0;
        --*speed;
    }
}
void draw_star(void)
{
    if ((anim_timer0 & 0b11111) == 0)
    {
        star_state = !star_state;
        if (star_state)
        {
            pal_col(5, 0x20);
            pal_col(6, 0x0F);
        }
        else
        {
            pal_col(5, 0x30);
            pal_col(6, 0x10);
        }
    }
}
void if_not_in_the_right_direction(void)
{
    camera_extra_moving_timer = 0;
    if (camera_extra_moving > 0)
        --camera_extra_moving;
    else if (camera_extra_moving < 0)
        ++camera_extra_moving;
    ++hand_flash;
}
void generate_star(void)
{
    temp2 = rand8(); // decide how many, use 2bits for it, the rest for positon
    // 0000 0000
    // |||| ||||
    // |||| ||++- how many star
    // ||++-++--- which postion preset is used
    // |+-------- hi tile id
    // +--------- postion offset
    temp3 = rand8(); // decide squence
    // 0000 0000
    // |||| ||||
    // |||| ||++- Star 1 tile id
    // |||| ++--- Star 2 tile id
    // ||++------ Star 3 tile id
    // ++-------- Star 4 tile id
    for (temp1 = 0; temp1 < 62; ++temp1)
        star_buffer[temp1] = 0;
    for (temp1 = 0; temp1 < (temp2 & 0b11); ++temp1)
        star_buffer[(star_positons[((temp2 >> 2) & 0b1111)][temp1] + (temp2 >> 7)) % 62] = 0x97 + (((temp3 & (0b11 << (temp1 * 2))) >> (temp1 * 2)) + ((temp2 >> 6) & 0b1));
}
void draw_one_row_star_buffer(int addr)
{
    generate_star();
    multi_vram_buffer_horz(star_buffer, 31, addr);
    multi_vram_buffer_horz((star_buffer) + 31, 31, addr + 32);
}
void spawn_enemy(void)
{
    if (enemy_spawn_timer == 0)
    {
        enemy_spawn_timer = (rand8() & 0b01111111) + 100;
        enemy_spawn_timer_temp = (2 * (level >> 2));
        if(enemy_spawn_timer > enemy_spawn_timer_temp)
            enemy_spawn_timer -= enemy_spawn_timer_temp;
        for (temp1 = 0; temp1 < ENEMY_MAX; ++temp1)
        {
            if (enemies[temp1].used)
            {
                if (temp1 == (ENEMY_MAX - 1))
                    return;
            }
            else
            {
                enemies[temp1].sub_speed = enemies[temp1].speed = 0;
                enemies[temp1].used = 1;
                enemies[temp1].self_timer = 0;
                // level = wave = 0;
                if (direction_wants[0] == DIR_0)
                    enemies[temp1].type = level_enemies[level & 0b1111][wave];
                else
                    enemies[temp1].type = level_enemies[level & 0b1111][wave] | 0b10000000;
                enemies[temp1].x = (rand8() & 0b01111111) + 20;
                if (direction_wants[0] == DIR_0)
                    enemies[temp1].y = 10;
                else
                    enemies[temp1].y = 200;
                return;
            }
        }
    }
    else
        --enemy_spawn_timer;
}
unsigned char update_enemy(Enemy_t *enemy)
{
    if (enemy->used == 1)
    {
        if (enemy->type > 2)
        {
            if ((enemy->type & 0b01111111) == 3 || (enemy->type & 0b01111111) == 4)
                enemy->type = ((rand8() & 0b1) + (enemy->type - 3)) | (enemy->type & 0b10000000);
            else if ((enemy->type & 0b01111111) == 5)
            {
                if ((rand8() & 0b1) == 0)
                    enemy->type = 0 | (enemy->type & 0b10000000);
                else
                    enemy->type = 2 | (enemy->type & 0b10000000);
            }
            else if ((enemy->type & 0b01111111) == 6)
                enemy->type = (rand8() % 3) | (enemy->type & 0b10000000);
        }
        // enemy->type = 0;
        if ((enemy->type & 0b01111111) == 0 || (enemy->type & 0b01111111) == 2)
        {
            if (!(enemy->type & 0b10000000))
            {
                if (enemy->self_timer < 24)
                    ++enemy->y;
                else if (enemy->self_timer > 150)
                {
                    if((enemy->type & 0b01111111) == 2)
                    {
                        enemy->y += 3;
                        if(enemy->x < player_pos[0])
                            enemy->sub_speed += 1;
                        else if(enemy->x > player_pos[0])
                            enemy->sub_speed -= 1;
                    }else
                        enemy->y -= 2;
                }
            }
            else
            {
                if (enemy->self_timer < 24)
                    --enemy->y;
                else if (enemy->self_timer > 150)
                {
                    if((enemy->type & 0b01111111) == 2)
                    {
                        enemy->y -= 3;
                        if(enemy->x < player_pos[0])
                            enemy->sub_speed += 1;
                        else if(enemy->x > player_pos[0])
                            enemy->sub_speed -= 1;
                    }else
                        enemy->y += 2;
                }
            }
            if(enemy->y <= 8)
                enemy->used = 0;
            else if(enemy->y >= 220)
                enemy->used = 0;
            if(enemy->sub_speed > 7)
            {
                enemy->speed += 1;
                enemy->sub_speed = 0;
            }
            else if(enemy->sub_speed < -7)
            {
                enemy->speed -= 1;
                enemy->sub_speed = 0;
            }
            if(enemy->speed > 3)
                enemy->speed = 3;
            else if(enemy->speed < -3)
                enemy->speed = -3;
            enemy->x += enemy->speed;
        }
        else if ((enemy->type & 0b01111111) == 1)
        {
            if (enemy->self_timer > 100)
            {
                if (enemy->self_timer >= 254)
                    enemy->self_timer = 101;
                if (!(enemy->type & 0b10000000))
                {
                    enemy->y += 4;
                    if (enemy->y > 240)
                        enemy->used = 0;
                }
                else
                {
                    enemy->y -= 4;
                    if (enemy->y < 10)
                        enemy->used = 0;
                }
            }
            else if (enemy->self_timer < 60)
            {
                if ((enemy->self_timer & 0b1111) == 0)
                    famistudio_sfx_play(3, FAMISTUDIO_SFX_CH0);
            }
        }
        ++enemy->self_timer;

        if (bullet_0.used)
        {
            temp4 = bullet_0.y - camera_y;

            if (enemy->x + 2 < bullet_0.x + 8 &&
                enemy->x + 2 + 12 > bullet_0.x &&
                enemy->y + 2 < temp4 + 8 &&
                enemy->y + 2 + 12 > temp4)
            {
                if ((enemy->type & 0b01111111) == 1)
                {
                    if(enemy->self_timer > 100)
                    {
                        score[0][6] += 6;
                        enemy->used = 2;
                        enemy->self_timer = 0;
                        bullet_0.used = 0;
                        famistudio_sfx_play(1, FAMISTUDIO_SFX_CH0);
                        enemy_destroyed += 1;
                        return 0;
                    }
                }
                else
                {
                    if ((enemy->type & 0b01111111) == 0)
                        score[0][6] += 3;
                    else if ((enemy->type & 0b01111111) == 2)
                        score[0][5] += 1;
                    enemy->used = 2;
                    enemy->self_timer = 0;
                    bullet_0.used = 0;
                    famistudio_sfx_play(1, FAMISTUDIO_SFX_CH0);
                    enemy_destroyed += 1;
                    return 0;
                }
            }
        }
        if (bullet_1.used)
        {
            temp4 = bullet_1.y - camera_y;

            if (enemy->x + 2 < bullet_1.x + 8 &&
                enemy->x + 2 + 12 > bullet_1.x &&
                enemy->y + 2 < temp4 + 8 &&
                enemy->y + 2 + 12 > temp4)
            {
                if ((enemy->type & 0b01111111) == 1)
                {
                    if(enemy->self_timer > 100)
                    {
                        score[0][6] += 6;
                        enemy->used = 2;
                        enemy->self_timer = 0;
                        bullet_1.used = 0;
                        famistudio_sfx_play(1, FAMISTUDIO_SFX_CH0);
                        enemy_destroyed += 1;
                        return 0;
                    }
                }
                else
                {
                    if ((enemy->type & 0b01111111) == 0)
                        score[0][6] += 3;
                    else if ((enemy->type & 0b01111111) == 2)
                        score[0][5] += 1;
                    enemy->used = 2;
                    enemy->self_timer = 0;
                    bullet_1.used = 0;
                    famistudio_sfx_play(1, FAMISTUDIO_SFX_CH0);
                    enemy_destroyed += 1;
                    return 0;
                }
            }
        }
        temp3 = player_pos[1] - camera_y;
        if (enemy->x + 2 < player_pos[0] + 8 &&
            enemy->x + 2 + 12 > player_pos[0] &&
            enemy->y + 2 < temp3 + 8 &&
            enemy->y + 2 + 12 > temp3)
        {
            return 1;
        }
    }
    else if (enemy->used == 2)
    {
        if (enemy->self_timer > 20)
        {
            enemy->used = 0;
        }
        ++enemy->self_timer;
    }
    return 2;
}
void draw_enemy(Enemy_t *enemy)
{
    if (enemy->used == 1)
    {
        if ((enemy->type & 0b01111111) == 0 || ((enemy->type & 0b01111111) == 1 && (enemy->self_timer > 100)) || (enemy->type & 0b01111111) == 2)
        {
            if (enemy->type & 0b10000000)
            {
                if(enemy->self_timer & 0b1)
                {
                    if(enemy->speed < -1)
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_big_Metasprite7_data);
                    else if(enemy->speed > 1)
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_big_Metasprite1_data);
                    else
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_big_Metasprite0_data);
                }
                else
                {
                    if(enemy->speed < -1)
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_small_5_data);
                    else if(enemy->speed > 1)
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_small_1_data);
                    else
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_small_0_data);
                }
                if(enemy->speed < -1)
                    oam_meta_spr(enemy->x, enemy->y, ship_enemy0_Metasprite_DOWN_left_data);
                else if(enemy->speed > 1)
                    oam_meta_spr(enemy->x, enemy->y, ship_enemy0_Metasprite_DOWN_right_data);
                else
                    oam_meta_spr(enemy->x, enemy->y, ship_enemy0_Metasprite_DOWN_data);

            }
            else
            {
                if(enemy->self_timer & 0b1)
                {
                    if(enemy->speed < -1)
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_big_Metasprite5_data);
                    else if(enemy->speed > 1)
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_big_Metasprite3_data);
                    else
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_big_Metasprite4_data);
                }
                else
                {
                    if(enemy->speed < -1)
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_small_7_data);
                    else if(enemy->speed > 1)
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_small_3_data);
                    else
                        oam_meta_spr(enemy->x, enemy->y, ship_fire_small_4_data);
                }
                if(enemy->speed < -1)
                    oam_meta_spr(enemy->x, enemy->y, ship_enemy0_Metasprite_UP_left_data);
                else if(enemy->speed > 1)
                    oam_meta_spr(enemy->x, enemy->y, ship_enemy0_Metasprite_UP_right_data);
                else
                    oam_meta_spr(enemy->x, enemy->y, ship_enemy0_Metasprite_UP_data);

            }
        }
        if ((enemy->type & 0b01111111) == 1)
        {
            if ((enemy->self_timer < 60) && (enemy->self_timer & 1))
                oam_meta_spr(enemy->x, enemy->y, ship_enemy_warn_Metasprite);
        }
    }
    if (enemy->used == 2)
        oam_meta_spr(enemy->x, enemy->y, explosion_list[enemy->self_timer & 0b1]);
}
void update_score(void){
    temp1 = 0;
    for(temp0 = 6;temp0 >= 0;--temp0)
    {
        if(score[0][temp0] >= 10)
        {    
            if(temp0 - 1 >= 0)
            {
                score[0][temp0 - 1] += score[0][temp0] / 10;
                score[0][temp0] %= 10;
            }
        }
        display_score[0][temp0] = score[0][temp0] + 0xB0; // convert into tiles
    }
    if(temp1)
    {
        life += 10;
        for(temp0 = 0;temp0<7;++temp0)
        {
            score[0][temp0] = 0;
            display_score[0][temp0] = 0xB0; // convert into tiles
        }
    }
    multi_vram_buffer_horz(display_score[0], 7, 0x2006);
    temp3 = 0;
    for(temp0 = 0; temp0 < 7; ++temp0)  
    {
        if(score[0][temp0] == 0 && score[1][temp0] == 0)
            continue;
        if(score[0][temp0] > score[1][temp0])
            break;    
        else if(score[0][temp0] < score[1][temp0])
        {
            temp3 = 1;
            break;     
        }
    }
    if(!temp3)
    {
        for(temp0 = 0;temp0<7;++temp0)
        {
            score[1][temp0] = score[0][temp0];
            display_score[1][temp0] = score[1][temp0] + 0xB0; // convert into tiles
        }
        multi_vram_buffer_horz(display_score[1], 7, 0x2016);
    }
}
