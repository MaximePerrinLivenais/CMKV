#pragma once

#include "tile.hh"

class Tetravex;

using shared_tetravex = std::shared_ptr<Tetravex>;

class Tetravex
{
    public:
        explicit Tetravex(std::vector<shared_tile> tiles, const size_t dimension);

        shared_tetravex swap_tiles(const size_t first_index, const size_t second_index);
        int cost() const;

        void print() const;
        const std::vector<shared_tile> get_tiles() const;

        static shared_tetravex parse_from_file(const char* filepath);

    private:
        std::vector<shared_tile> tiles;
        const size_t dimension;
};
