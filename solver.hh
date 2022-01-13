#pragma once

#include <random>

#include "tetravex.hh"

class Solver
{
public:
    explicit Solver(const shared_tetravex tetravex);

    shared_tetravex solve();

private:
    shared_tetravex generate_random_state(shared_tetravex game);
    bool is_temperature_uniform(double T, shared_tetravex game);
    double uniform_draw();
    double init_temperature();

private:
    shared_tetravex game;

    std::uniform_int_distribution<int> int_distribution_;
    std::uniform_real_distribution<double> real_distribution_;

    std::default_random_engine random_generator_;
};
