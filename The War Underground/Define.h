#pragma once
#ifndef DEFINE_H
#define DEFINE_H

// state
#define UNUSED -1
#define DIE 0
#define MOVE 1
#define ATTACK 2

#define NORMAL 1 << 0

// road
#define UPROAD 0
#define MIDROAD 1
#define DOWNROAD 2

// pipe coordinates
#define PIPEA 30
#define PIPEB 89
#define PIPEC 148
#define PIPED 50
#define PIPEE 128

// positive infinity
#define INF 1000000000

// the size of window
#define nScreenWidth 179
#define nScreenHeight 52

// the length of card
#define CARDLEN 15

#define BASE 0
#define SOLDIER 1
#define TANK 2
#define BAZOOKA 3
#define BUNKER 4
#define PLANT 5
#define BOMBER 6
#define ARTILLERY 7
#define GUARD 8
#define SONIC 9
#define GOLIATH 10
#define MISSILE 11

#define MOTIONLESS 75

#endif