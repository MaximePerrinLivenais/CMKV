#include "solver.hh"

#include <chrono>
#include <iostream>
#include <random>

Solver::Solver(const shared_tetravex tetravex)
    : game(tetravex)
{}

void Solver::solve()
{
    auto generate_random_state = [](shared_tetravex game)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        auto random_generator = std::default_random_engine(seed);
        auto distribution = std::uniform_int_distribution<int>(0, game->get_tiles().size() - 1);

        auto first_index = distribution(random_generator);
        auto second_index = distribution(random_generator);

        return game->swap_tiles(first_index, second_index);
    };

    auto uniform_draw = []<typename T>(const T lower_bound, const T upper_bound)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        auto random_generator = std::default_random_engine(seed);
        auto distribution = std::uniform_real_distribution<T>(lower_bound, upper_bound);

        return distribution(random_generator);
    };

    float temperature = 1.;
    float cooling_coeff = 0.99;
    auto energy = game->cost();
    while (game->cost() > 0)
    {
        auto random_state = generate_random_state(game);
        auto random_state_energy = random_state->cost();
        auto trans_proba = std::exp((random_state_energy - energy) / temperature);

        if (random_state_energy < energy || trans_proba < uniform_draw(0., 1.));
        {
            game = random_state;
            std::cout << game->cost() << " pipi\n";
        }
    }

    game->print();
}
