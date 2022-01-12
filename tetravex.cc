#include "tetravex.hh"

#include <algorithm>
#include <cmath>
#include <iostream>

Tetravex::Tetravex(std::vector<shared_tile> tiles, const size_t dimension)
    : tiles(tiles)
    , dimension(dimension)
{}

shared_tetravex Tetravex::swap_tiles(const size_t first_index,
                                     const size_t second_index)
{
    auto copy = tiles;
    auto begin = copy.begin();
    std::iter_swap(begin + first_index, begin + second_index);

    return std::make_shared<Tetravex>(copy, dimension);
}

bool Tetravex::is_tile_movable(const size_t index) const
{
    return tiles[index]->get_movable();
}

int Tetravex::cost() const
{
    int sum = 0;
    for (size_t i = 0; i < dimension; i++)
    {
        for (size_t j = 0; j < dimension; j++)
        {
            if (((i + j) % 2) > 0)
                continue;

            if (i > 0)
                sum += tiles.at(i * dimension + j)->get_top()
                    != tiles.at((i - 1) * dimension + j)->get_bottom();
            if (j > 0)
                sum += tiles.at(i * dimension + j)->get_left()
                    != tiles.at(i * dimension + j - 1)->get_right();
            if (i < (dimension - 1))
                sum += tiles.at(i * dimension + j)->get_bottom()
                    != tiles.at((i + 1) * dimension + j)->get_top();
            if (j < (dimension - 1))
                sum += tiles.at(i * dimension + j)->get_right()
                    != tiles.at(i * dimension + j + 1)->get_left();
        }
    }
    return sum;
}

void Tetravex::print() const
{
    std::string separating_line = "+";
    for (size_t i = 0; i < dimension; i++)
        separating_line += "-----+";

    std::cout << separating_line << '\n';
    for (size_t i = 0; i < dimension; i++)
    {
        std::string line = "|";
        for (size_t j = 0; j < dimension; j++)
        {
            line += tiles.at(dimension * i + j)->get_top_line() + "|";
        }
        std::cout << line << '\n';

        line = "|";
        for (size_t j = 0; j < dimension; j++)
        {
            line += tiles.at(dimension * i + j)->get_center_line() + "|";
        }
        std::cout << line << '\n';

        line = "|";
        for (size_t j = 0; j < dimension; j++)
        {
            line += tiles.at(dimension * i + j)->get_bottom_line() + "|";
        }
        std::cout << line << '\n' << separating_line << '\n';
    }
}

const std::vector<shared_tile> Tetravex::get_tiles() const
{
    return tiles;
}

shared_tetravex Tetravex::parse_from_file(const char* filepath)
{
    auto tiles = Tile::parse_from_file(filepath);
    size_t dimension = std::sqrt(tiles.size());

    return std::make_shared<Tetravex>(tiles, dimension);
}
