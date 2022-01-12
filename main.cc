#include "solver.hh"

#include <iostream>

int main(int argc, char* argv[])
{
    auto tetravex = Tetravex::parse_from_file(argv[1]);
    auto solver = std::make_shared<Solver>(tetravex);
    solver->solve();
}
