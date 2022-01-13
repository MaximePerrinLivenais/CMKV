#include "tile.hh"

#include <fstream>
#include <iostream>

Tile::Tile(char top, char left, char right, char bottom, bool movable)
    : top(top)
    , left(left)
    , right(right)
    , bottom(bottom)
    , movable(movable)
{}

void Tile::print() const
{
    std::cout << (char)(top + '0') << (char)(left + '0') << (char)(right + '0')
              << (char)(bottom + '0') << movable << '\n';
}

shared_tile Tile::parse_from_line(const std::string& line)
{
    char top = line[0];
    char left = line[1];
    char right = line[2];
    char bottom = line[3];
    bool movable = line.length() <= 4;

    return std::make_shared<Tile>(top, left, right, bottom, movable);
}

std::vector<shared_tile> Tile::parse_from_file(const char* filepath)
{
    std::ifstream file_stream;
    file_stream.open(filepath);

    auto vec = std::vector<shared_tile>();

    std::string line;
    while (std::getline(file_stream, line))
    {
        vec.push_back(Tile::parse_from_line(line));
    }

    return vec;
}

std::string Tile::get_top_line() const
{
    const std::string spacing = "  ";
    return spacing + top + spacing;
}

std::string Tile::get_center_line() const
{
    const std::string spacing = " ";
    return spacing + left + spacing + right + spacing;
}

std::string Tile::get_bottom_line() const
{
    const std::string spacing = "  ";
    return spacing + bottom + spacing;
}

char Tile::get_top() const
{
    return top;
}

char Tile::get_left() const
{
    return left;
}

char Tile::get_right() const
{
    return right;
}

char Tile::get_bottom() const
{
    return bottom;
}

bool Tile::get_movable() const
{
    return movable;
}

bool Tile::operator==(Tile& other) const
{
    return top == other.top && left == other.left && right == other.right
        && bottom == other.bottom && movable == other.movable;
}
