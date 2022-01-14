#pragma once

#include <random>

#include "tetravex.hh"

class Solver
{
public:
    explicit Solver(unique_tetravex tetravex);

    unique_tetravex solve();

private:
    void generate_random_state();
    bool is_temperature_uniform(double T);
    double uniform_draw();
    double init_temperature();

private:
    unique_tetravex game;

    std::uniform_int_distribution<int> int_distribution_;
    std::uniform_real_distribution<double> real_distribution_;

    std::default_random_engine random_generator_;
};
