#pragma once

#include <memory>
#include <string>
#include <vector>

class Tile;

using shared_tile = std::shared_ptr<Tile>;
using unique_tile = std::unique_ptr<Tile>;

class Tile
{
public:
    explicit Tile(char top, char left, char right, char bottom, bool movable);

    Tile(const Tile&) = default;

    void print() const;

    static unique_tile parse_from_line(const std::string& line);
    static std::vector<unique_tile> parse_from_file(const char* filepath);

    std::string get_top_line() const;
    std::string get_center_line() const;
    std::string get_bottom_line() const;

    char get_top() const;
    char get_left() const;
    char get_right() const;
    char get_bottom() const;

    bool get_movable() const;

    bool operator==(Tile& other) const;

private:
    const char top;
    const char left;
    const char right;
    const char bottom;

    const bool movable;
};
