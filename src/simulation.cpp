//
// Created by Gjin on 7/27/25.
//

#include <vector>
#include <utility>
#include "simulation.hpp"


void Simulation::Draw(Color aliveColor, Color deadColor)
{
    grid.Draw(aliveColor, deadColor);
}

void Simulation::SetCellValue(int row, int column, int value)
{
    grid.SetValue(row, column, value);
}

int Simulation::GetCellValue(int row, int column)
{
    return grid.GetValue(row, column);
}

int Simulation::CountLiveNeighbors(int row, int column)
{
    int liveNeighbors = 0;
    std::vector<std::pair<int, int>> neighborOffsets =
        {
            {-1,0},  // Top
            {1,0},  // Down
            {0,-1},  // Left
            {0,1},  // Right
            {-1,-1},  // Top-left
            {-1,1},  // Top-right
            {1,-1},  // Down-left
            {1,1},  // Down-right
        };

    for (const auto& offset : neighborOffsets)
    {
        // has grid wrapping behavior
        int neighborRow = (row + offset.first + grid.GetRows()) % grid.GetRows();
        int neighborColumn = (column + offset.second + grid.GetColumns()) % grid.GetColumns();
        liveNeighbors += grid.GetValue(neighborRow, neighborColumn);
    }

    return liveNeighbors;
}

void Simulation::Update()
{

    if (!IsRunning())
    {
        return;
    }

    for (int row = 0; row < grid.GetRows(); row++)
    {

        for (int column = 0; column < grid.GetColumns(); column++)
        {
            int liveNeighbors = CountLiveNeighbors(row, column);
            int cellValue = grid.GetValue(row, column);

            // RULES
            if (cellValue == 1)
            {
                if (liveNeighbors >3 || liveNeighbors < 2)
                {
                    temp_grid.SetValue(row, column, 0);
                }
                else
                {
                    temp_grid.SetValue(row, column, 1);
                }
            }
            else
            {
                if (liveNeighbors == 3)
                {
                    temp_grid.SetValue(row, column, 1);
                }
                else
                {
                    temp_grid.SetValue(row, column, 0);
                }
            }
        }
    }
    // Copy temp_grid to normal grid
    grid = temp_grid;
}

void Simulation::ClearGrid()
{
    if (!IsRunning())
    {
        grid.Clear();
    }
}

void Simulation::CreateRandomState()
{
    if (!IsRunning())
    {
        grid.FillRandom();
    }
}

void Simulation::ToggleCell(int row, int column)
{
    if (!IsRunning())
    {
        grid.ToggleCell(row, column);
    }
}



