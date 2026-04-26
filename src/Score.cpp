#include "Score.h"

Score::Score(){

    monstresTues = 0;
    objetsRecuperes = 0;
    pnjsSauves = 0;
    pieces = 0;

}

int Score::calculerScore() {
    int score;
    return score = monstresTues + objetsRecuperes + pnjsSauves + pieces;
}