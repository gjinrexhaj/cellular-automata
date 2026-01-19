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
        case SimulationType::TORPEDO:
            TorpedoAlgorithm();
            break;
        case SimulationType::DIAMOND_GROWTH:
            DiamondGrowthAlgorithm();
            break;
        case SimulationType::HIGH_LIFE:
            HighLifeAlgorithm();
            break;
    }
}

void Simulation::ClearGrid()
{
    grid.Clear();
    temp_grid.Clear();
}

void Simulation::CreateRandomState(int density, bool additiveFill)
{
    grid.FillRandom(density, additiveFill);
    temp_grid = grid;
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

    temp_grid = grid;
}

void Simulation::SetSimulationType(SimulationType type)
{
    simulationType = type;
}

Grid Simulation::GetCurrentGrid()
{
    return grid;
}

void Simulation::OverwriteGrid(const std::vector<std::vector<int>>& cellState)
{
    grid.setCellState(cellState);
    temp_grid.setCellState(cellState);
}


////////////////////////////////////
//  CELLULAR AUTOMATA ALGORITHMS  //
////////////////////////////////////

// TODO: impl parametrized algorithm given a ruleset string
//  example - 4/3,2 (survive if 4 neighbots, born if 3 or 2 neighbors)
//  also have assymetric parameter, which determines when grid copying occurs
//  takes in a ruleset object as a parameter, hardcode rs for now
void Simulation::ParametrizedAlgorithm()
{
    std::vector<int> survivalConditions;
    survivalConditions.push_back(4);

    std::vector<int> birthConditions;
    birthConditions.push_back(3);
    birthConditions.push_back(2);

    bool asymetric = false;

    // Loop through entire grid
    for (int row = 0; row < grid.GetRows(); row++)
    {
        for (int column = 0; column < grid.GetColumns(); column++)
        {
            int liveNeighbors = CountLiveNeighbors(row, column);
            int cellValue = grid.GetValue(row, column);

            // check the survival condtions if alive
            if (cellValue == 1)
            {
                bool metCondition = false;
                for (auto survivalCondition : survivalConditions)
                {
                    if (liveNeighbors == survivalCondition)
                    {
                        temp_grid.SetValue(row, column, 1);
                        metCondition = true;
                    }
                }
                if (!metCondition)
                {
                    temp_grid.SetValue(row, column, 0);
                }
            }
            // else if dead, check the birth condition
            else if (liveNeighbors == 0)
            {
                for (auto birthCondition : birthConditions)
                {
                    if (liveNeighbors == birthCondition)
                    {
                        temp_grid.SetValue(row, column, 1);
                    }
                }
            }
        }
        if (asymetric)
        {
            grid = temp_grid;
        }
    }
    if (!asymetric)
    {
        grid = temp_grid;
    }
}


void Simulation::GameOfLifeAlgorithm()
{
    // Game of life rules
    // B3, S26
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

void Simulation::TorpedoAlgorithm()
{
    // Torpedo, same as high life but grid swap occurs within outer loop
    // Less performant, but leads to cool results!
    for (int row = 0; row < grid.GetRows(); row++)
    {
        for (int column = 0; column < grid.GetColumns(); column++)
        {
            int liveNeighbors = CountLiveNeighbors(row, column);
            int cellValue = grid.GetValue(row, column);

            // IF CELL IS ALIVE
            if (cellValue == 1)
            {
                // AND HAS 2 OR 3 NEIGHBORS
                if (liveNeighbors == 2 || liveNeighbors == 3)
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
            // IF CELL IS DEAD
            else
            {
                // AND HAS > 3 AND < 6 NEIGHBORS
                if (liveNeighbors == 3 || liveNeighbors == 6)
                {
                    temp_grid.SetValue(row, column, 1);
                }
            }
        }
        // copy temp grid to normal grid once done, do so inside the outer loop to introduce asymmetrical chaos
        grid = temp_grid;
    }
}

void Simulation::DiamondGrowthAlgorithm()
{
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
                // SURVIVAL RULE (S23)
                if (liveNeighbors == 2 || liveNeighbors == 3)
                {
                    temp_grid.SetValue(row, column, 1);
                }
                // AND HAS < 3 OR > 2 NEIGHBORS
                else if (liveNeighbors >3 || liveNeighbors < 2)
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
                // SURVIVAL RULE (S23)
                else if (liveNeighbors == 2)
                {
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

    // replace grid
    grid = temp_grid;
}


void Simulation::HighLifeAlgorithm()
{
    // Game of life rules, but with
    // B36 , S23
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
                if (liveNeighbors == 3 || liveNeighbors == 6)
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