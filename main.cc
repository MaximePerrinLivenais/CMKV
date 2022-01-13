#include <iostream>
#include <string>

#include "solver.hh"

void generate_tetravex_input()
{
    for (int dim = 2; dim < 7; dim++)
    {
        for (int i = 0; i < 10; i++)
        {
            auto tetravex = Tetravex::generate_random_tetravex(dim);
            tetravex->to_file("out_dim" + std::to_string(dim) + "x"
                                  + std::to_string(dim) + "_"
                                  + std::to_string(i) + ".txt",
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
    // generate_tetravex_input();
    auto tetravex = Tetravex::parse_from_file(argv[1]);
    auto solver = std::make_shared<Solver>(tetravex);
    auto solved_tetravex = solver->solve();

    solved_tetravex->print();

    solved_tetravex->to_file(argv[2]);
}
