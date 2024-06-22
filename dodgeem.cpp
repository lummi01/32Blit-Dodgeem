// Dodge'em
// 2024 M. Gerloff


#include <list>
#include "dodgeem.hpp"
#include "assets.hpp"

using namespace blit;

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
    int score[2];
    short life = 3;
    short speed = 30;
    short car = 1;
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

struct CONFETTI
{
    Vec2 pos;
    Vec2 vel;
    float rot;
    float drot;
    Pen c;
};

struct PARTICLE
{
	Vec2 pos;
	int alpha;
    float radius;
};




GAME game;
CAR p;
CAR o[2];
CONFETTI confetti[25];

Timer player_timer;
Timer oppo_timer;
Tween crash_tween;
Tween blend_tween;
Tween sound_tween;
Tween score_tween;

static std::list<PARTICLE> particles;

void new_level();

void NewParticle(float x, float y)
{
    PARTICLE p;
    p.pos = Vec2(x,y) + Vec2(rand() %7, rand() %7);
    p.alpha = 96 + rand() %160;
    p.radius = 0;
    particles.push_back(p);
}

void UpdateParticle()
{
    for(auto p = particles.begin(); p != particles.end();) 
	{
        if(p->alpha < 4) 
		{
            p = particles.erase(p);
            continue;
        }
        p->alpha-=4;
        p->radius+=.2f;
        ++p;
    }
}

void NewConfetti(short i)
{
    Pen color[7]{
        Pen(255,255,255),
        Pen(255,0,0),
        Pen(0,255,0),
        Pen(0,0,255),
        Pen(255,255,0),
        Pen(255,0,255),
        Pen(0,255,255)};
    
    confetti[i].pos = Vec2(rand() %156, 0);
    confetti[i].vel = Vec2((rand() %20 * .01f) -.1f, (rand() %10 *.01f) + .2f);
    confetti[i].rot = rand() %3;
    confetti[i].drot = (rand() %5 *.01f) + .05f;
    confetti[i].c = color[rand() %7];
}

void UpdateConfetti()
{
    for(int i = 0; i<25; i++) 
	{
        if(confetti[i].pos.x < -2 || confetti[i].pos.y > 159 || confetti[i].pos.y > 120) 
		{
            NewConfetti(i);
            continue;
        }
        confetti[i].pos += confetti[i].vel;
        if (confetti[i].rot < 0 || confetti[i].rot > 2)
            confetti[i].drot = -confetti[i].drot;
        confetti[i].rot += confetti[i].drot;
    }
}



void dots()
{
    game.dots--;
    if (game.dots == 0)
    {
        player_timer.stop();
        oppo_timer.stop();
        blend_tween.init(tween_linear,0 , 7, 400, 1);
        blend_tween.start();
        game.car++;
        if (game.car > 2)
        {
            game.car = 1;
            game.speed -= 5;
            if (game.speed < 15)
            {
                game.speed = 15;
                game.car = 2;
            }
        }
        game.state = 3;
    }
}

void player_control()
{
    channels[0].trigger_attack();

    Vec2 vel[4]{Vec2(1,0),Vec2(0,1),Vec2(-1,0),Vec2(0,-1)};

    p.pos += vel[p.sprite];

    int x = (p.pos.x + 4) * .125f;
    int y = (p.pos.y + 4) * .125f;

    p.step++;
    if (p.step == 8)
    {
        p.step =0;
    }
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
            channels[1].trigger_attack();
            game.score[0]++;
            dots();
        }
    }

    for (int i=0; i<game.car; i++)
    {
        if (p.pos.x+2 < o[i].pos.x+6 && p.pos.x+6 > o[i].pos.x+2 && p.pos.y+2 < o[i].pos.y+6 && p.pos.y+6 > o[i].pos.y+2)
        {
            player_timer.stop();
            oppo_timer.stop();
            crash_tween.start();
            sound_tween.start();
            game.state = 2;
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
    for (int i=0; i<game.car; i++)
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

void new_level()
{
    player_timer.init(player_update, 15, -1);
    player_timer.stop();

    oppo_timer.init(oppo_update, game.speed, -1);
    oppo_timer.stop();

    if (game.dots == 0)
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
        game.dots = 192;
    }

    p.sprite = 3;
    p.step = 0;
    p.move = 0;
    p.pos = Vec2(0,60);
    p.vel = Vec2(0,-1);

    short row = rand() %5;
    for (int i=0; i<game.car; i++)
    {
        o[i].sprite = 3;
        o[i].step = 0;
        o[i].move = 0;
        o[i].row = row + (i);
        o[i].pos = Vec2(112 + ((row + i) * 8),61);
        o[i].vel = Vec2(1 - (i * 2),1 - (i * 2));
    }
}

// init
void init() 
{
    set_screen_mode(ScreenMode::lores);
    screen.sprites = Surface::load(asset_sprites);

    channels[0].waveforms = Waveform::SQUARE;
    channels[0].volume = 4000;
    channels[0].frequency = 100;
    channels[0].attack_ms = 5;
    channels[0].decay_ms = 10;
    channels[0].sustain = 10;
    channels[0].release_ms = 5;

    channels[1].waveforms = Waveform::SQUARE;
	channels[1].volume = 8000;
    channels[1].frequency = 2500;
    channels[1].attack_ms = 5;
    channels[1].decay_ms = 10;
    channels[1].sustain = 5;
    channels[1].release_ms = 5;

    channels[2].waveforms = Waveform::NOISE;
	channels[2].volume = 12000;
    channels[2].frequency = 500;
    channels[2].attack_ms = 5;
    channels[2].decay_ms = 10;
    channels[2].sustain = 50;
    channels[2].release_ms = 200;

    blend_tween.init(tween_linear,7 , 0, 400, 1);
    blend_tween.start();
    crash_tween.init(tween_linear,0 , 1, 2500, 1);
    sound_tween.init(tween_linear,1000 , 100, 500, 1);
    score_tween.init(tween_sine, 0.0f, 255.0f, 3500, -1);

    if (read_save(game.score[1]) == false)
        game.score[1] = 0;

    for (int i=0; i<25; i++)
    {
        NewConfetti(i);
        confetti[i].pos.y = rand() %120;
    }

    game.life--;
    game.speed = 30;
    game.car = 1;
    new_level();

    game.state = 4;
}

// render
void render(uint32_t time) 
{
    screen.alpha = 255;
    screen.mask = nullptr;

    if (game.state > 2)
    {
        screen.pen = Pen(0, 0, 0);
        screen.clear();
    }
    for (int x=blend_tween.value;x<(20 - blend_tween.value);x++)
        for (int y=blend_tween.value;y<(15 - blend_tween.value);y++)
            screen.sprite(game.field[y][x], Point(x * 8, y * 8));

    if (game.state < 3)
    {
        for (int i=0; i<game.car; i++)
            screen.sprite(48 + o[i].sprite + (16 * i), Point(o[i].pos.x, o[i].pos.y)); // Opponent

        screen.sprite(32 + p.sprite, Point(p.pos.x, p.pos.y)); // Player
    }

    screen.pen = Pen(255, 255, 255);
    screen.text(std::to_string(game.score[0]), minimal_font, Point(109, 67), true, TextAlign::center_right);

    for (int i=0; i<game.life; i++)
        screen.sprite(35, Point(51 + (i*6), 63));

    if (game.state == 5)
    {
        screen.pen = Pen(128, 128, 128);
        if (game.score[0] == game.score[1])
        {
            screen.sprite(Rect(0, 5, 2, 2), Point(72, 14));
            screen.text("-- New Top Score --", minimal_font, Point(80, 38), true, TextAlign::center_center);
            for(int i=0; i<25; i++)
      	    {
      	    	screen.pen = confetti[i].c;
                screen.rectangle(Rect(confetti[i].pos.x, confetti[i].pos.y - confetti[i].rot, 3, 2 * confetti[i].rot));
       	    }
        }
        else
            screen.text("Top Score: " + std::to_string(game.score[1]), minimal_font, Point(80, 32), true, TextAlign::center_center);
        screen.pen = Pen(255, 0, 0);
        screen.text("Press X", minimal_font, Point(80, 92), true, TextAlign::center_center);
    }
    else if (game.state == 2)
    {
        for(auto &p : particles)
  	    {
  	    	screen.alpha = p.alpha;
            screen.circle(Point(p.pos.x, p.pos.y),p.radius);
   	    }
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
        if (crash_tween.is_finished())
        {
            if (particles.size() == 0)
            {
                game.life--;
                if (game.life < 0)
                {
                    if (game.score[0] > game.score[1])
                    {
                        game.score[1] = game.score[0];
                        write_save(game.score[1]);
                    }                
                    game.dots = 0;
                }
                blend_tween.init(tween_linear,0 , 6, 400, 1);
                blend_tween.start();
                game.state = 3;
            }
        }
        else if (rand() %4 == 0)
        {
            NewParticle(p.pos.x, p.pos.y);
        }
        if (sound_tween.is_running())
        {    
            channels[2].frequency = sound_tween.value;
            channels[2].trigger_attack();
        }
        UpdateParticle();
    }
    else if (game.state == 5)
    {
        if (buttons.released & Button::A)
        {
            for (int y=0; y<15; y++)
                for (int x=0; x<20; x++)
                    if (game.field[y][x] > 19 && game.field[y][x] < 32)
                        game.field[y][x] -= 16;
            game.dots = 0;
            new_level();
            game.score[0] = 0;
            game.life = 2;
            game.speed = 30;
            game.car = 1;
            blend_tween.init(tween_linear,7 , 0, 400, 1);
            blend_tween.start();
            game.state = 4;
        }
        if (game.score[0] == game.score[1])
            UpdateConfetti();
    }
    else if (blend_tween.is_finished())
    {
        if (game.state == 3)
        {
            if (game.life < 0)
                game.state = 5;
            else
            {
                new_level();
                blend_tween.init(tween_linear,7 , 0, 400, 1);
                blend_tween.start();
                game.state = 4;
            }
        }
        else if (game.state == 4)
        {
            game.state = 0;
        }
    }
}
