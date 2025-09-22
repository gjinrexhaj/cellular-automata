//
// Created by Gjin on 7/27/25.
//

#include <vector>
#include <utility>
#include "simulation.hpp"

#include <iostream>
#include <ostream>


void Simulation::Draw(Color aliveColor, Color deadColor, float lineThickness)
{
    grid.Draw(aliveColor, deadColor, lineThickness);
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

    // Check which simulation type is active, then apply the rules
    switch (simulationType)
    {
        case SimulationType::GAME_OF_LIFE:
            GameOfLifeAlgorithm();
            break;
        case SimulationType::HIGH_LIFE:
            HighLifeAlgorithm();
            break;
    }
}

void Simulation::ClearGrid()
{
    grid.Clear();
}

void Simulation::CreateRandomState(int density, bool additiveFill)
{
    grid.FillRandom(density, additiveFill);
}

void Simulation::ToggleCell(int row, int column, int brushSize)
{
    // TODO: implement brush size here
    grid.ToggleCell(row, column);



    for (int i = 1; i < brushSize; i++)
    {
        // up
        grid.ToggleCell(row-i, column);
        // down
        grid.ToggleCell(row+i, column);
        // left
        grid.ToggleCell(row, column-i);
        // right
        grid.ToggleCell(row, column+i);
        // up-right
        grid.ToggleCell(row-i, column+i);
        // up-left
        grid.ToggleCell(row-i, column-i);
        // down-right
        grid.ToggleCell(row+i, column+i);
        // down-left
        grid.ToggleCell(row+i, column-i);
    }

}

void Simulation::SetSimulationType(SimulationType type)
{
    simulationType = type;
}


////////////////////////////////////
//  CELLULAR AUTOMATA ALGORITHMS  //
////////////////////////////////////
void Simulation::GameOfLifeAlgorithm()
{
    // Game of life rules
    for (int row = 0; row < grid.GetRows(); row++)
    {

        for (int column = 0; column < grid.GetColumns(); column++)
        {
            // GET NUM NEIGHBORS AND CURRENT CELL STATE
            int liveNeighbors = CountLiveNeighbors(row, column);
            int cellValue = grid.GetValue(row, column);

            // IF CELL IS ALIVE
            if (cellValue == 1)
            {
                // AND HAS < 3 OR > 2 NEIGHBORS
                if (liveNeighbors >3 || liveNeighbors < 2)
                {
                    // CELL IS DEAD
                    temp_grid.SetValue(row, column, 0);
                }
                // OTHERWISE
                else
                {
                    // CELL IS ALIVE
                    temp_grid.SetValue(row, column, 1);
                }
            }
            // IF CELL IS DEAD
            else
            {
                // AND HAS 3 NEIGHBORS
                if (liveNeighbors == 3)
                {
                    // CELL IS ALIVE
                    temp_grid.SetValue(row, column, 1);
                }
                // OTHERWISE
                else
                {
                    // CELL IS DEAD
                    temp_grid.SetValue(row, column, 0);
                }
            }
        }
    }

    // Copy temp_grid to normal grid, once done
    grid = temp_grid;
}

void Simulation::HighLifeAlgorithm()
{
    // TODO: implement HighLifeAlgorithm()

    // High Life rules
    // if alive and < 2 neighbors, cell is dead
    // if alive and > 3 neigbors, cell is dead
    // otherwise, cell is alive
    // id dead and == 3 neighbor, cell is alive
    std::cout << "Simulation::HighLifeAlgorithm() - NOT YET IMPLEMENTED!!!" << std::endl;
}

