#include "main.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int TS;

typedef struct{
  float x, y, d, s, a, r, ra;
  int t;
}asteroid;

int circleCollision(int x1, int y1, int r1, int x2, int y2, int r2)
{
  int x = abs(x1-x2);
  int y = abs(y1-y2);
  if(sqrt(x*x + y*y)<=r1+r2){
    return 1;
  }
  return 0;
}

int main(void)
{
  SCREEN_WIDTH = 1200;
  SCREEN_HEIGHT = 1000;
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroid Aeronaut");
  int display = GetCurrentMonitor();
  SCREEN_WIDTH = GetMonitorWidth(display);
  SCREEN_HEIGHT = GetMonitorHeight(display);
  SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  ToggleFullscreen();
  
  SetTargetFPS(60);
  float dt = 1;

  TS = SCREEN_HEIGHT/20;
  Texture2D tileset = LoadTexture("resources/tileset.png");

  float a = 0;
  float pa = 0;
  Rectangle p = {(float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2, TS, TS*2};
  Rectangle d = {(float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2, TS, TS};
  float defDist = TS*2.5;
  float distance = defDist; 
  int onGround = 0;
  float yvel = 0;
  float jumpSpeed = (float)TS/8;
  float gravity = -(float)TS/160;
  int showDefense = 0;

  int health = 10;
  int score = 0;

  int asteroidsLen = 2;
  asteroid asteroids[100];
  for(int i=0;i<100;i++){
    asteroids[i] = (asteroid){-1,-1,-1,-1,-1,-1,-1,-1};
  }

  while (!WindowShouldClose()){
    dt = GetFrameTime();
    
    BeginDrawing();
    ClearBackground((Color){10,10,10,255});

    if(health>0){
      for(int i=0;i<asteroidsLen;i++){
        if(asteroids[i].d<=defDist && asteroids[i].d!=-1){
          asteroids[i].d = -1;
          health--;
        }

        if(showDefense && circleCollision(asteroids[i].x, asteroids[i].y, asteroids[i].r, d.x, d.y, d.height/2)){
          asteroids[i].d = -1;
          score++;
          if(asteroidsLen<100){
            asteroidsLen++;
          }
        }

        if(asteroids[i].d==-1){
          asteroids[i].d = GetRandomValue(SCREEN_WIDTH, SCREEN_WIDTH*1.5);
          asteroids[i].s = (float)GetRandomValue(1, 30)/10;
          asteroids[i].a = (float)GetRandomValue(0, 360);
          asteroids[i].r = (float)GetRandomValue(TS*0.3, TS);
          asteroids[i].t = GetRandomValue(0, 3);
          asteroids[i].ra = 0;
        }
       
        asteroids[i].ra += asteroids[i].s/2;
        asteroids[i].d -= asteroids[i].s;
        asteroids[i].x = (float)SCREEN_WIDTH/2 + (sin(asteroids[i].a/180*PI)*asteroids[i].d);
        asteroids[i].y = (float)SCREEN_HEIGHT/2 - (cos(asteroids[i].a/180*PI)*asteroids[i].d);
        // DrawCircle(asteroids[i].x, asteroids[i].y, asteroids[i].r, YELLOW);
        DrawTexturePro(tileset, (Rectangle){96+32*asteroids[i].t,0,32,32}, (Rectangle){asteroids[i].x, asteroids[i].y, asteroids[i].r*2, asteroids[i].r*2}, (Vector2){(float)asteroids[i].r, (float)asteroids[i].r}, asteroids[i].ra, RAYWHITE);
      }

      yvel += gravity;
      pa += 0.2f;

      distance+=yvel;
      if(distance<=defDist){
        yvel = 0;
        distance = defDist;
        onGround = 1;
      }
      
      p.x = (float)SCREEN_WIDTH/2 + (sin(a/180*PI)*distance);
      p.y = (float)SCREEN_HEIGHT/2 - (cos(a/180*PI)*distance);

      d.x = (float)SCREEN_WIDTH/2 + (sin(a/180*PI)*(distance+TS*3));
      d.y = (float)SCREEN_HEIGHT/2 - (cos(a/180*PI)*(distance+TS*3));

      if(p.x>SCREEN_WIDTH || p.x<0 || p.y>SCREEN_HEIGHT || p.y<0){
        health = 0;
      }

      // DrawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, defDist, RED);
      DrawTexturePro(tileset, (Rectangle){64,64,32*6,32*6}, (Rectangle){(float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2, defDist*2, defDist*2}, (Vector2){(float)defDist, (float)defDist}, pa, RAYWHITE);
      DrawText(TextFormat("Health: %d", health), (float)SCREEN_WIDTH/2-MeasureTextEx(GetFontDefault(), TextFormat("Health: %d", health), (float)TS/1.5, 1).x/2, SCREEN_HEIGHT/2, TS/1.5, BLACK);
      DrawText(TextFormat("Score: %d", score), 50, 50, TS/2, WHITE);
      if(showDefense){
        DrawTexturePro(tileset, (Rectangle){32,0,32,64}, p, (Vector2){p.width/2,p.height}, a, RAYWHITE);
        // DrawCircle(d.x, d.y, d.height/2, BLUE);
        DrawTexturePro(tileset, (Rectangle){64,0,32,32}, d, (Vector2){d.width/2,d.height/2}, a, RAYWHITE);
      }
      else{
        // DrawRectanglePro(p, (Vector2){p.width/2,p.height}, a, GREEN);
        DrawTexturePro(tileset, (Rectangle){0,0,32,64}, p, (Vector2){p.width/2,p.height}, a, RAYWHITE);
      }
      DrawText("Move with AD and SPACE + W is schield, save planet, good luck!", TS*4, 50, TS/2, GREEN);

      if(IsKeyDown(KEY_A)){
        a-=(float)TS/20;
      }
      if(IsKeyDown(KEY_D)){
        a+=(float)TS/20;
      }
      if(IsKeyDown(KEY_SPACE) && onGround){
        yvel = jumpSpeed + dt*gravity; 
      }
      if(IsKeyDown(KEY_W)){
        showDefense = 1;
      }
      else{
        showDefense = 0;
      }
    }
    else{
      DrawText("You couldn't save the earth", 50, (float)SCREEN_HEIGHT/2-TS*1.5, TS, WHITE);
      DrawText(TextFormat("Your score was %d", score), 50, SCREEN_HEIGHT/2, TS, WHITE);
      DrawText("press enter to restart!", 50, (float)SCREEN_HEIGHT/2+TS*1.5, TS, WHITE);
      if(IsKeyDown(KEY_ENTER)){
        health = 10;
        score = 0;
        for(int i=0;i<100;i++){
          asteroids[i] = (asteroid){-1,-1,-1,-1,-1,-1};
        }
        a = 0;
        p = (Rectangle){(float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2, TS, TS*2};
        d = (Rectangle){(float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2, TS*2, (float)TS/3};
        distance = defDist; 
        onGround = 0;
        yvel = 0;
      }
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
