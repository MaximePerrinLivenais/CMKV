#include "solver.hh"

#include <chrono>
#include <iostream>
#include <random>

#include "tetravex.hh"

Solver::Solver(const shared_tetravex tetravex)
    : game(tetravex)
{}

shared_tetravex Solver::generate_random_state(shared_tetravex game)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto random_generator = std::default_random_engine(seed);
    auto distribution =
        std::uniform_int_distribution<int>(0, game->get_tiles().size() - 1);

    auto first_index = distribution(random_generator);
    while (!game->is_tile_movable(first_index))
        first_index = distribution(random_generator);

    auto second_index = distribution(random_generator);
    while (!game->is_tile_movable(second_index) || second_index == first_index)
        second_index = distribution(random_generator);

    return game->swap_tiles(first_index, second_index);
}

double Solver::uniform_draw(const double lower_bound, const double upper_bound)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto random_generator = std::default_random_engine(seed);
    auto distribution =
        std::uniform_real_distribution<double>(lower_bound, upper_bound);

    return distribution(random_generator);
}

bool Solver::is_temperature_uniform(float T, shared_tetravex game)
{
    float nb_change = 0;
    float nb_iteration = 20;

    for (size_t i = 0; i < nb_iteration; i++)
    {
        auto cost1 = game->cost();
        game = generate_random_state(game);
        auto cost2 = game->cost();

        if (std::exp(-std::abs(cost2 - cost1) / T) > uniform_draw(0., 1.))
            nb_change += 1;
    }

    return (nb_change / nb_iteration) > 0.9;
}

float Solver::init_temperature()
{
    float Tmin = 0.1;
    float Tmax = 500;

    float T = (Tmax + Tmin) / 2;

    while (Tmax - Tmin > 0.1)
    {
        if (is_temperature_uniform(T, game))
            Tmax = T;
        else
            Tmin = T;

        T = (Tmax + Tmin) / 2;
    }

    return T;
}

void Solver::solve()
{
    float temperature = init_temperature();
    std::cout << "initial temperature = " << temperature << "\n";
    float cooling_coeff = 0.99;

    auto energy = game->cost();

    while (game->cost() > 0)
    {
        std::cout << "========\n";
        auto random_state = generate_random_state(game);
        auto random_state_energy = random_state->cost();
        auto trans_proba =
            std::exp(-std::abs(random_state_energy - energy) / temperature);
        auto draw = uniform_draw(0., 1.);

        random_state->print();
        std::cout << "energy: " << energy
                  << ", new_state energy: " << random_state_energy << "\n";

        std::cout << "transition proba: " << trans_proba << ", draw: " << draw
                  << "\n ";

        if (random_state_energy < energy || trans_proba > draw)
        {
            game = random_state;
            std::cout << "accepted new state\n";
        }

        temperature *= cooling_coeff;
        std::cout << "temperature = " << temperature << "\n";
    }

    game->print();
}
