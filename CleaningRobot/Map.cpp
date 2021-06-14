#include "Map.h"

Map::Map() {
    this->map2d = {};
    this->rows = ROWS;
    this->columns = COLUMNS;
}

Map::Map(my_array& map2d) {
    this->map2d = map2d;
    this->rows = map2d.size();
    this->columns = map2d[0].size();
}

int Map::getRows() {
    return this->rows;
}

int Map::getColumns() {
    return this->columns;
}

my_array Map::getMap() {
    return this->map2d;
}

void Map::setMap(my_array& map2d) {
    this->map2d = map2d;
    this->rows = map2d.size();
    this->columns = map2d[0].size();
}

void Map::setValue(int x, int y, int value) {
    this->map2d[y][x] = value;
}

void Map::addWalls() {
    for (size_t x = 0; x < columns; ++x) {
        map2d[0][x] = 1;
        map2d[rows - 1][x] = 1;
    }
    for (size_t y = 1; y < rows - 1; ++y) {
        map2d[y][0] = 1;
        map2d[y][columns - 1] = 1;
    }
}

void Map::addObstacle(const size_t x1, const size_t y1, const size_t x2, const size_t y2)
{
    for (size_t i = y1; i <= y2; ++i) {
        for (size_t j = x1; j <= x2; ++j) {
            map2d[i][j] = 1;
        }
    }
}

bool Map::checkIfAllCleaned() {
    for (const auto& j : map2d) {
        for (auto i : j) {
            if (i == 0) {
                return false;
            }
        }
    }
    return true;
}

bool Map::allSidesUnavailable(const size_t x1, const size_t y1) {
    if (map2d[y1][x1 - 1] == 0 || map2d[y1][x1 + 1] == 0 || map2d[y1 + 1][x1] == 0 || map2d[y1 - 1][x1] == 0)
        return false;
    else
        return true;
}

void Map::printMap() {
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            std::cout << map2d[row][column] << " ";
        }
        std::cout << std::endl;
    }
}

bool Map::operator==(my_array& m2) {
    return this->map2d == m2;
}