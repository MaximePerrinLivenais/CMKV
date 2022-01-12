#pragma once

#include <memory>
#include <string>
#include <vector>

class Tile;

using shared_tile = std::shared_ptr<Tile>;

class Tile
{
    public:
        explicit Tile(char top, char left, char right, char bottom, bool movable);

        void print() const;

        static shared_tile parse_from_line(const std::string& line);
        static std::vector<shared_tile> parse_from_file(const char* filepath);

        const std::string get_top_line() const;
        const std::string get_center_line() const;
        const std::string get_bottom_line() const;

        const char get_top() const;
        const char get_left() const;
        const char get_right() const;
        const char get_bottom() const;

    private:
        const char top;
        const char left;
        const char right;
        const char bottom;

        const bool movable;
};
