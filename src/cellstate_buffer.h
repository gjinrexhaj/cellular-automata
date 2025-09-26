//
// Created by Gjin on 9/25/25.
//

#pragma once
#include <vector>


class CellstateBuffer
{
    public:
        CellstateBuffer();
        void Advance(std::vector<std::vector<int>> cells);
        std::vector<std::vector<int>> Regress();
        void Log();
        void Clear();
        unsigned long Size();

    private:
        std::vector<std::vector<std::vector<int>>> buffer;
};

