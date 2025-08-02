//
// Created by Gjin on 7/26/25.
//

#include <raylib.h>
#include "grid.hpp"


void Grid::Draw(Color aliveColor, Color deadColor, float lineThickness)
{
    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; column++)
        {
            Color color = cells[row][column] ? aliveColor : deadColor;
            DrawRectangle(column * cellSize, row * cellSize, cellSize - lineThickness, cellSize - lineThickness, color);
        }
    }
}

void Grid::SetValue(int row, int column, int value)
{
    if (IsWithinBounds(row, column))
    {
        cells[row][column] = value;
    }
}

int Grid::GetValue(int row, int column)
{
    if (IsWithinBounds(row, column))
    {
        return cells[row][column];
    }
    return 0;
}

bool Grid::IsWithinBounds(int row, int column)
{
    if (row >= 0 && row < rows && column >= 0 && column < columns)
    {
        return true;
    }
    return false;
}

void Grid::FillRandom(int density)
{
    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; column++)
        {
            // default density = 4 | lower == higher
            int random = GetRandomValue(0, density);
            cells[row][column] = (random == density) ? 1 : 0;
        }
    }
}

void Grid::Clear()
{
    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; column++)
        {
            cells[row][column] = 0;
        }
    }
}

void Grid::ToggleCell(int row, int column)
{
    if (IsWithinBounds(row, column))
    {
        cells[row][column] = !cells[row][column];
    }
}

