#include "tetravex.hh"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>

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
    return tiles.at(index)->get_movable();
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

void Tetravex::to_file(std::string file, bool shuffle, bool show_not_movable)
{
    std::ofstream file_stream(file);

    auto tiles_copy = tiles;

    if (shuffle)
    {
        unsigned seed =
            std::chrono::system_clock::now().time_since_epoch().count();
        auto random_generator = std::default_random_engine(seed);

        std::shuffle(tiles_copy.begin(), tiles_copy.end(), random_generator);

        for (size_t i = 0; i < tiles.size(); i++)
        {
            if (!tiles.at(i)->get_movable())
            {
                for (size_t j = 0; j < tiles_copy.size(); j++)
                {
                    if (*tiles.at(i) == *tiles_copy.at(j))
                    {
                        std::iter_swap(tiles_copy.begin() + i,
                                       tiles_copy.begin() + j);

                        break;
                    }
                }
            }
        }
    }

    for (auto tile : tiles_copy)
    {
        file_stream << tile->get_top() << tile->get_left() << tile->get_right()
                    << tile->get_bottom();

        if (show_not_movable && !tile->get_movable())
        {
            file_stream << " @";
        }

        file_stream << "\n";
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

shared_tetravex Tetravex::generate_random_tetravex(size_t dimension)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto random_generator = std::default_random_engine(seed);

    auto values_distribution = std::uniform_int_distribution<char>('0', '9');

    std::vector<shared_tile> tiles;

    for (size_t i = 0; i < dimension; i++)
    {
        for (size_t j = 0; j < dimension; j++)
        {
            char top = values_distribution(random_generator);
            char left = values_distribution(random_generator);
            char right = values_distribution(random_generator);
            char bottom = values_distribution(random_generator);

            // 1 / 10 will be movable
            bool movable = values_distribution(random_generator) != '9';

            if (i > 0)
            {
                top = tiles.at((i - 1) * dimension + j)->get_bottom();
            }

            if (j > 0)
            {
                left = tiles.at(i * dimension + j - 1)->get_right();
            }

            tiles.emplace_back(
                std::make_shared<Tile>(top, left, right, bottom, movable));
        }
    }

    return std::make_shared<Tetravex>(tiles, dimension);
}
