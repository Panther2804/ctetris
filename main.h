//
// Created by ppasch on 26.05.18.
//

#ifndef TETRISCLION_MAIN_H
#define TETRISCLION_MAIN_H

int main();

void setup();
void loop();
void playfieldinit();
void minit();
void mprint();
void transfer();
bool mput(int ,int , int, bool);
void undoplayfield();
bool draw(const bool[2][4],int);

#endif //TETRISCLION_MAIN_H
