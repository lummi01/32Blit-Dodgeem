// Dodge'em
// 2024 M. Gerloff

#include "dodgeem.hpp"
#include "assets.hpp"

using namespace blit;

Font font(font3x5);

struct GAME
{
    short state;
    short field[15][20]{
        { 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7},
        {15, 4, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 7, 8},
        {15,15, 4, 5, 5, 5, 5, 5, 0, 0, 0, 0, 6, 6, 6, 6, 6, 7, 8, 8},
        {15,15,15, 4, 5, 5, 5, 5, 0, 0, 0, 0, 6, 6, 6, 6, 7, 8, 8, 8},
        {15,15,15,15, 4, 5, 5, 5, 0, 0, 0, 0, 6, 6, 6, 7, 8, 8, 8, 8},
        {15,15,15,15,15, 4, 5, 5, 0, 0, 0, 0, 6, 6, 7, 8, 8, 8, 8, 8},
        {14, 0, 0, 0, 0, 0, 4,42,43,44,45,46,47, 7, 0, 0, 0, 0, 0, 8},
        {14, 0, 0, 0, 0, 0,14,58,59,60,61,62,63, 8, 0, 0, 0, 0, 0, 8},
        {14, 0, 0, 0, 0, 0,13,11,11,11,11,11,11,10, 0, 0, 0, 0, 0, 8},
        {14,14,14,14,14,13,12,12, 0, 0, 0, 0,11,11,10, 9, 9, 9, 9, 9},
        {14,14,14,14,13,12,12,12, 0, 0, 0, 0,11,11,11,10, 9, 9, 9, 9},
        {14,14,14,13,12,12,12,12, 0, 0, 0, 0,11,11,11,11,10, 9, 9, 9},
        {14,14,13,12,12,12,12,12, 0, 0, 0, 0,11,11,11,11,11,10, 9, 9},
        {14,13,12,12,12,12,12,12, 0, 0, 0, 0,11,11,11,11,11,11,10, 9},
        {13,12,12,12,12,12,12,12,11,11,11,11,11,11,11,11,11,11,11,10}};
    short move[15][20]{
        {1,9,9,9,9,9,9,9,3,3,3,8,9,9,9,9,9,9,9,1},
        {9,1,9,9,9,9,9,9,2,2,2,8,9,9,9,9,9,9,1,9},
        {9,9,1,9,9,9,9,9,2,2,2,8,9,9,9,9,9,1,9,9},
        {9,9,9,1,9,9,9,9,2,2,2,8,9,9,9,9,1,9,9,9},
        {9,9,9,9,1,9,9,9,2,2,2,8,9,9,9,1,9,9,9,9},
        {9,9,9,9,9,1,9,9,4,4,4,8,9,9,1,9,9,9,9,9},
        {8,8,8,8,8,8,9,9,9,9,9,9,9,9,6,5,5,5,5,7},
        {6,5,5,5,5,7,9,9,9,9,9,9,9,9,6,5,5,5,5,7},
        {6,5,5,5,5,7,9,9,9,9,9,9,9,9,8,8,8,8,8,8},
        {9,9,9,9,9,1,9,9,8,3,3,3,9,9,1,9,9,9,9,9},
        {9,9,9,9,1,9,9,9,8,2,2,2,9,9,9,1,9,9,9,9},
        {9,9,9,1,9,9,9,9,8,2,2,2,9,9,9,9,1,9,9,9},
        {9,9,1,9,9,9,9,9,8,2,2,2,9,9,9,9,9,1,9,9},
        {9,1,9,9,9,9,9,9,8,2,2,2,9,9,9,9,9,9,1,9},
        {1,9,9,9,9,9,9,9,8,4,4,4,9,9,9,9,9,9,9,1}};
    short dots;
    int score;
    short life = 3;
    short blend;
};

struct CAR
{
    short sprite;
    short step;
    short move;
    short row;
    Vec2 pos;
    Vec2 vel;
};


GAME game;
CAR p;
CAR o[2];

Timer player_timer;
Timer oppo_timer;

void new_level(bool dots);

void dots()
{
    game.dots++;
    if (game.dots == 192)
    {
        game.state = 0;
        new_level(true);
    }
}

void player_control()
{
    Vec2 vel[4]{Vec2(1,0),Vec2(0,1),Vec2(-1,0),Vec2(0,-1)};

    p.pos += vel[p.sprite];

    int x = (p.pos.x + 4) * .125f;
    int y = (p.pos.y + 4) * .125f;

    p.step++;
    if (p.step == 8)
        p.step =0;

    if (p.move > 0)
    {
        p.move--;
        p.pos += p.vel;
    }
    else
    {
        p.vel = Vec2(0,0);
        switch (game.move[y][x])
        {
            case 1:
            {
                if (p.step == 4)
                {
                    p.sprite++;
                    if (p.sprite > 3)
                        p.sprite = 0;
                }
                break;
            }
            case 2:
            {
                if (buttons & Button::DPAD_UP)
                {
                    p.vel.y = -1;
                    p.move = 8;
                }
                else if (buttons & Button::DPAD_DOWN)
                {
                    p.vel.y = 1;
                    p.move = 8;
                }
                break;
            }
            case 3:
            {
                if (buttons & Button::DPAD_DOWN)
                {
                    p.vel.y = 1;
                    p.move = 8;
                }
                break;
            }
            case 4:
            {
                if (buttons & Button::DPAD_UP)
                {
                    p.vel.y = -1;
                    p.move = 8;
                }
                break;
            }
            case 5:
            {
                if (buttons & Button::DPAD_LEFT)
                {
                    p.vel.x = -1;
                    p.move = 8;
                }
                else if (buttons & Button::DPAD_RIGHT)
                {
                    p.vel.x = 1;
                    p.move = 8;
                }
                break;
            }
            case 6:
            {
                if (buttons & Button::DPAD_RIGHT)
                {
                    p.vel.x = 1;
                    p.move = 8;
                }
                break;
            }
            case 7:
            {
                if (buttons & Button::DPAD_LEFT)
                {
                    p.vel.x = -1;
                    p.move = 8;
                }
                break;
            }
        }

        if (game.field[y][x] > 19)
        {
            game.field[y][x] -= 16;
            game.score++;
            dots();
        }
    }

    for (int i=0; i<2; i++)
    {
        if (p.pos.x+2 < o[i].pos.x+6 && p.pos.x+6 > o[i].pos.x+2 && p.pos.y+2 < o[i].pos.y+6 && p.pos.y+6 > o[i].pos.y+2)
        {
            game.life--;
            game.state = 0;
            new_level(false);
            break;
        }
    }
}

void player_update(Timer &t)
{
    player_control();
}

void oppo_update(Timer &t)
{
    Vec2 vel[4]{Vec2(1,0),Vec2(0,1),Vec2(-1,0),Vec2(0,-1)};
    for (int i=0; i<2; i++)
    {
        o[i].pos += vel[o[i].sprite];

        int x = (3 + o[i].pos.x) * .125f;
        int y = (3 + o[i].pos.y) * .125f;

        o[i].step++;
        if (o[i].step == 8)
            o[i].step = 0;
        
        if (game.move[y][x] == 1 && o[i].step == 4)
        {
            o[i].sprite--;
            if (o[i].sprite == 2 || (o[i].sprite == 0))
                o[i].vel = -o[i].vel;
            else if (o[i].sprite < 0)
                o[i].sprite = 3;
        }
        else if (game.move[y][x] == 8)
        {
            if (o[i].sprite == 0 || o[i].sprite == 2)             // Horizontal
                o[i].pos = o[i].pos + (Vec2(0,1) * o[i].vel);
            else                                            // Senkrecht
                o[i].pos = o[i].pos + (Vec2(1,0) * o[i].vel);

            o[i].move++;
            if (o[i].move == 8)
            {
                o[i].move = 0;
                if (o[i].sprite == 1 || o[i].sprite == 2)
                    o[i].row -= o[i].vel.x;
                else
                    o[i].row += o[i].vel.x;

                if (o[i].row == 0 || o[i].row == 5)
                    o[i].vel = -o[i].vel;
            }     
        }

        if (game.field[y][x] > 19)
        {
            game.field[y][x] -= 16;
            dots();
        }
    }
}

void new_level(bool dots)
{
    player_timer.init(player_update, 15, -1);
    player_timer.stop();

    oppo_timer.init(oppo_update, 20, -1);
    oppo_timer.stop();

    if (dots)
    {
        for (int y=0; y<6; y++)
        {
            for (int x=0; x<8; x++)
            {
                game.field[y][x] += 16;
                game.field[y][x + 12] += 16;
                game.field[y + 9][x] += 16;
                game.field[y + 9][x + 12] += 16;
            }
        }
        game.dots = 0;
    }

    p.sprite = 3;
    p.step = 0;
    p.move = 0;
    p.pos = Vec2(0,60);
    p.vel = Vec2(0,-1);

    for (int i=0; i<2; i++)
    {
        o[i].sprite = 3;
        o[i].step = 0;
        o[i].move = 0;
        o[i].row = 1 + (i);
        o[i].pos = Vec2(120 + (i * 8),61);
        o[i].vel = Vec2(1 - (i * 2),1 - (i * 2));
    }
}

// init
void init() 
{
    set_screen_mode(ScreenMode::lores);
    screen.sprites = Surface::load(asset_sprites);

    game.life--;
    new_level(true);
}

// render
void render(uint32_t time) 
{
    if (game.state < 2)
    {
        screen.alpha = 255;
        screen.mask = nullptr;

        for (int x=0;x<20;x++)
            for (int y=0;y<15;y++)
                screen.sprite(game.field[y][x], Point(x * 8, y * 8));

        for (int i=0; i<2; i++)
             screen.sprite(48 + o[i].sprite + (16 * i), Point(o[i].pos.x, o[i].pos.y)); // Opponent

        screen.sprite(32 + p.sprite, Point(p.pos.x, p.pos.y)); // Player

        for (int i=0; i<game.life; i++)
            screen.sprite(35, Point(51 + (i*6), 64));
            
        screen.pen = Pen(255, 255, 255);
        screen.text(std::to_string(game.score), font, Point(108, 68), true, TextAlign::center_right);
    }
    else
    {
        for (int i=0; i<48; i++)
        {}
    }
}

// update
void update(uint32_t time) 
{
    if (game.state == 1)
    {
        if (buttons & Button::A)
        {
            player_control();
        }
    }
    else if (game.state == 0)
    {
        if (buttons.released & Button::A)
        {
            player_timer.start();
            oppo_timer.start();
            game.state = 1;
        }
    }
    else if (game.state == 2)
    {
        
    }
}
