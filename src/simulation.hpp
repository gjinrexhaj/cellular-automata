//
// Created by Gjin on 7/27/25.
//

#pragma once
#include "grid.hpp"
#include "raylib.h"

class Simulation {
    public:
        Simulation(int width, int height, int cellSize)
            : grid(width, height, cellSize), temp_grid(width, height, cellSize), run(false) {};
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
    private:
        Grid grid;
        Grid temp_grid;
        bool run;
};

