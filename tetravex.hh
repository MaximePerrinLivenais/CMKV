#pragma once

#include "tile.hh"

class Tetravex;

using shared_tetravex = std::shared_ptr<Tetravex>;
using unique_tetravex = std::unique_ptr<Tetravex>;

class Tetravex
{
public:
    explicit Tetravex(std::vector<unique_tile> tiles, const size_t dimension);

    void swap_tiles(const size_t first_index, const size_t second_index);
    void revert_swap();

    bool is_tile_movable(const size_t index) const;
    int cost() const;

    void to_file(std::string file, bool shuffle = false,
                 bool show_not_movable = false);

    void print() const;
    const std::vector<unique_tile>& get_tiles() const;

    static unique_tetravex parse_from_file(const char* filepath);
    static unique_tetravex generate_random_tetravex(size_t dimension);

private:
    std::vector<unique_tile> tiles;
    const size_t dimension;

    std::pair<size_t, size_t> prev_swap_;
};
