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
#define UPROADY 17
#define MIDROAD 1
#define MIDROADY 30
#define DOWNROAD 2
#define DOWNROADY 42

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

#define MOTIONLESS 75

#define PRACTICE 1
#define HISTORY 2
#define TWOPLAYER 3

#define BULLETNUMBER 300

#endif