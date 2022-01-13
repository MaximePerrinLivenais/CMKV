#pragma once

#include "tetravex.hh"

class Solver
{
public:
    explicit Solver(const shared_tetravex tetravex);

    void solve();

private:
    shared_tetravex generate_random_state(shared_tetravex game);
    bool is_temperature_uniform(float T, shared_tetravex game);
    double uniform_draw(const double lower_bound, const double upper_bound);
    float init_temperature();

private:
    shared_tetravex game;
};
