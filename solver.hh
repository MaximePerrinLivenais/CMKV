#pragma once

#include "tetravex.hh"

class Solver
{
public:
    explicit Solver(const shared_tetravex tetravex);

    void solve();

private:
    shared_tetravex game;
};
