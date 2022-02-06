#include "solver.hh"

#include <chrono>
#include <iostream>
#include <random>

#include "tetravex.hh"

Solver::Solver(unique_tetravex tetravex)
    : game(std::move(tetravex))
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    random_generator_ = std::default_random_engine(seed);

    int_distribution_ =
        std::uniform_int_distribution<int>(0, game->get_tiles().size() - 1);

    real_distribution_ = std::uniform_real_distribution<double>(0., 1.);
}

void Solver::generate_random_state()
{
    auto first_index = int_distribution_(random_generator_);
    while (!game->is_tile_movable(first_index))
        first_index = int_distribution_(random_generator_);

    auto second_index = int_distribution_(random_generator_);
    while (!game->is_tile_movable(second_index) || second_index == first_index)
        second_index = int_distribution_(random_generator_);

    game->swap_tiles(first_index, second_index);
}

double Solver::uniform_draw()
{
    return real_distribution_(random_generator_);
}

bool Solver::is_temperature_uniform(double temperature)
{
    double nb_change = 0;
    double nb_iteration = 200;

    for (size_t i = 0; i < nb_iteration; i++)
    {
        auto cost1 = game->cost();
        generate_random_state();
        auto cost2 = game->cost();

        if (std::exp(-std::abs(cost2 - cost1) / temperature) > uniform_draw())
            nb_change += 1;
    }

    return (nb_change / nb_iteration) > 0.95;
}

double Solver::init_temperature()
{
    double Tmin = 0.01;
    double Tmax = 1000;

    double T = (Tmax + Tmin) / 2;

    while (Tmax - Tmin > 1)
    {
        if (is_temperature_uniform(T))
            Tmax = T;
        else
            Tmin = T;

        T = (Tmax + Tmin) / 2;
    }

    return Tmax;
}

unique_tetravex Solver::solve()
{
    double base_temperature = init_temperature();

    // std::cout << "initial temperature = " << base_temperature << "\n";
    double cooling_coeff = 0.50;

    auto energy = game->cost();

    while (energy > 0)
    {
        double temperature = base_temperature;
        // std::cout << "cooling coef = " << cooling_coeff << "\n";
        // std::cout << "current energy = " << energy << "\n";
        // game->print();

        while (energy > 0 && temperature > 0.00001)
        {
            // std::cout << "========\n";
            generate_random_state();
            auto random_state_energy = game->cost();
            auto trans_proba =
                std::exp(-std::abs(random_state_energy - energy) / temperature);

            auto draw = uniform_draw();

            // std::cout << "energy: " << energy
            //           << ", new_state energy: " << random_state_energy <<
            //           "\n";

            // std::cout << "transition proba: " << trans_proba
            //           << ", draw: " << draw << "\n";

            if (random_state_energy < energy || trans_proba > draw)
            {
                energy = random_state_energy;
                // std::cout << "accepted new state\n";
            }
            else
            {
                game->revert_swap();
            }

            temperature *= cooling_coeff;
        }

        cooling_coeff = 1 - ((1 - cooling_coeff) / 2);
    }

    return std::move(game);
}
