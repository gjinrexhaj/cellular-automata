//
// Created by Gjin on 7/26/25.
//

#pragma once
#include <vector>
#include "raylib.h"

class Grid {
    public:
        Grid(int width, int height, int cellSize)
            : rows(height/cellSize), columns(width/cellSize), cellSize(cellSize),cells(rows, std::vector<int>(columns, 0)) {};
        void Draw(Color aliveColor, Color deadColor, float lineThickness);
        void SetValue(int row, int column, int value);
        int GetValue(int row, int column);
        int GetRows() {return rows;}
        int GetColumns() {return columns;}
        void FillRandom(int density);
        void Clear();
        void ToggleCell(int row, int column);

    private:
        int rows = 0;
        int columns = 0;
        int cellSize = 0;
        std::vector<std::vector<int>> cells;
        bool IsWithinBounds(int row, int column);
        Color aliveColor;
        Color deadColor;
};
