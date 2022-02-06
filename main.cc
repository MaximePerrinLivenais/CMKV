#include <iostream>
#include <string>

#include "solver.hh"

void generate_tetravex_input()
{
    for (int dim = 2; dim < 7; dim++)
    {
        for (char i = 'a'; i < 'd'; i++)
        {
            auto tetravex = Tetravex::generate_random_tetravex(dim);

            tetravex->to_file("radojicic" + std::to_string(dim)
                                  + std::string(1, i) + "-ref.txt",
                              false, false);

            tetravex->to_file("radojicic" + std::to_string(dim)
                                  + std::string(1, i) + ".txt",
                              true, true);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "usage: " << argv[0] << " input output\n";
        return 1;
    }
    auto tetravex = Tetravex::parse_from_file(argv[1]);
    auto solver = std::make_shared<Solver>(std::move(tetravex));
    auto solved_tetravex = solver->solve();

    // solved_tetravex->print();

    solved_tetravex->to_file(argv[2]);

    // generate_tetravex_input();
}
