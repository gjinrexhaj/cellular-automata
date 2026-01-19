//
// Created by Gjin on 7/27/25.
//

#pragma once
#include "grid.hpp"
#include "raylib.h"


enum class SimulationType
{
    GAME_OF_LIFE,
    TORPEDO,
    DIAMOND_GROWTH,
    HIGH_LIFE,
};

class Simulation {
    public:
        Simulation(int width, int height, int cellSize, SimulationType simulationType)
            : grid(width, height, cellSize), temp_grid(width, height, cellSize), simulationType(simulationType), run(false) {};
        void Draw(Color aliveColor, Color deadColor, float lineThickness);
        void SetCellValue(int row, int column, int value);
        int GetCellValue(int row, int column);
        int CountLiveNeighbors(int row, int column);
        void Update();
        bool IsRunning() { return run; }
        void Start() {run = true;}
        void Stop() {run = false;}
        void ClearGrid();
        void CreateRandomState(int density, bool additiveFill);
        void ToggleCell(int row, int column, int brushSize);
        void SetSimulationType(SimulationType simulationType);
        Grid GetCurrentGrid();
        void OverwriteGrid(const std::vector<std::vector<int>>&);

    private:
        Grid grid;
        Grid temp_grid;
        bool run;
        SimulationType simulationType;

        void ParametrizedAlgorithm();
        void GameOfLifeAlgorithm();
        void TorpedoAlgorithm();
        void DiamondGrowthAlgorithm();
        void HighLifeAlgorithm();
};

