#include "bomb/bomb.h"
#include "map_object/map_object.h"
#include <stdio.h>
#include <stdlib.h>
#define BOARD_SIZE 5

//https://stackoverflow.com/questions/8194250/polymorphism-in-c

typedef struct point {
    int x;
    int y;
} Point;

typedef struct player {
    Point b_p; // before position
    Point c_p; // current position
    int l_b; // left bomb
} Player;

typedef struct map {
    MapObject board[BOARD_SIZE][BOARD_SIZE + 1];
    Bomb bombs[BOARD_SIZE][BOARD_SIZE + 1];
    char fire[BOARD_SIZE][BOARD_SIZE + 1];
} Map;

void move_player(Player *p, char c, const char board[][BOARD_SIZE + 1]) {
    p->b_p = p->c_p;
    switch(c) {
      case ('d'):
        if(p->c_p.x+1>=BOARD_SIZE)return;
        if(board[p->c_p.y][p->c_p.x+1] == '-') ++p->c_p.x;
        break;
      case ('w'):
        if(p->c_p.y-1<0)return;
        if(board[p->c_p.y-1][p->c_p.x] == '-') --p->c_p.y;
        break;
      case ('a'):
        if(p->c_p.x-1<0)return;
        if(board[p->c_p.y][p->c_p.x-1] == '-') --p->c_p.x;
        break;
      case ('s'):
        if(p->c_p.y+1>=BOARD_SIZE)return;
        if(board[p->c_p.y+1][p->c_p.x] == '-') ++p->c_p.y;
        break;
    }
}

void put_bomb(Player *p, Map *m) {
    if( p->l_b == 0) return;
    Bomb b = { NORMAL
             , .t = 6
             };
    m->bombs[p->c_p.y][p->c_p.x] = b;
    p->l_b--;
}

void reflect_bombs(Player *p, Map *m) {
    for(int x = 0; x < BOARD_SIZE; x++) {
      for(int y = 0; y < BOARD_SIZE; y++) {
        if(m->bombs[y][x].t>0)m->bombs[y][x].t--;
        if(m->bombs[y][x].t==0)p->l_b++;
      }
    }
}

void reflect_fire(Map *m) {
    for(int x = 0; x < BOARD_SIZE; x++) {
      for(int y = 0; y < BOARD_SIZE; y++) {
        m->fire[y][x]=0;
      }
    }
    for(int x = 0; x < BOARD_SIZE; x++) {
      for(int y = 0; y < BOARD_SIZE; y++) {
        if(m->bombs[y][x].t==0){
          int dx = x;
          int dy = y;
          m->fire[dy][dx]=1;
          for(int i=0; i<2; i++){
            dx--;
            if (dx<0 || dx>=BOARD_SIZE || object(&(m->board[dy][dx]))=='#') break;
            if(m->bombs[dy][dx].t>0){
              m->bombs[dy][dx].t=0;
              reflect_fire(m);
              return;
            }
            m->fire[dy][dx]=1;
          }
          dx=x;
          for(int i=0; i<2; i++){
            dx++;
            if (dx<0 || dx>=BOARD_SIZE || object(&(m->board[dy][dx]))=='#') break;
            if(m->bombs[dy][dx].t>0){
              m->bombs[dy][dx].t=0;
              reflect_fire(m);
              return;
            }
            m->fire[dy][dx]=1;
          }
          dx=x;
          for(int i=0; i<2; i++){
            dy--;
            if (dy<0 || dy>=BOARD_SIZE || object(&(m->board[dy][dx]))=='#') break;
            if(m->bombs[dy][dx].t>0){
              m->bombs[dy][dx].t=0;
              reflect_fire(m);
              return;
            }
            m->fire[dy][dx]=1;
          }
          dy=y;
          for(int i=0; i<2; i++){
            dy++;
            if (dy<0 || dy>=BOARD_SIZE || object(&(m->board[dy][dx]))=='#') break;
            if(m->bombs[dy][dx].t>0){
              m->bombs[dy][dx].t=0;
              reflect_fire(m);
              return;
            }
            m->fire[dy][dx]=1;
          }
          m->bombs[y][x].t--;
        }
      }
    }
}


void reflect_board(Map *m, const Player p) {
    for(int x = 0; x < BOARD_SIZE; x++) {
      for(int y = 0; y < BOARD_SIZE; y++) {
        if(m->bombs[y][x].t>0) m->board[y][x] = MapObject { Bomb };
        if(m->fire[y][x]) m->board[y][x] = 'F';
        else if(m->board[y][x] == 'F') m->board[y][x] = '-';
      }
    }
    m->board[p.c_p.y][p.c_p.x] = 'o';
}

int reflect_dead(const Player p, const Map m) {
    return m.fire[p.c_p.y][p.c_p.x];
}

void print_board(const Map m) {
    for(int y = 0; y < BOARD_SIZE; ++y) {
      for(int x = 0; x < BOARD_SIZE; ++x) {
        printf(bomb_color(&(m.bombs[y][x])));
        printf("%c", m.board[y][x]);
        printf("\x1b[49m");
      }
      printf("\r\n");
    }
}

void initialize(Bomb bombs[][BOARD_SIZE + 1]) {
    for(int x = 0; x < BOARD_SIZE; x++) {
      for(int y = 0; y < BOARD_SIZE; y++) {
        bombs[y][x].t = -1;
      }
    }
}

int main (int argc, char *argv[]) {
    printf("Press any key to conitnue\n");

    Map m = {
      .board =
        {
          "-----",
          "-#-#-",
          "-----",
          "-#-#-",
          "-----"
        }

    };
    initialize(m.bombs);

    Player p = { .b_p = {.x = 0, .y = 0}
               , .c_p = {.x = 0, .y = 0}
               , .l_b = 1
               };  // 自分の位置
    int c;

    system("/bin/stty raw onlcr");  // enterを押さなくてもキー入力を受け付けるようになる

    while((c = getchar()) != '.') {   // '.' を押すと抜ける
        system("clear");
        printf("Press '.' to close\r\n");
        printf("You pressed '%c'\r\n", c);
        move_player(&p, c, m.board);

        reflect_bombs(&p, &m);

        if(c==' ') put_bomb(&p, &m);
        reflect_fire(&m);
        reflect_board(&m, p);
        int dead = reflect_dead(p, m);
        print_board(m);
        if(dead) printf("Game over!\r\n");

    }

    system("/bin/stty cooked");  // 後始末

    return 0;
}
