//
// Created by Gjin on 9/25/25.
//

#include "cellstate_buffer.h"

#include <iostream>
#include <ostream>

CellstateBuffer::CellstateBuffer()
{
    std::cout << "CellstateBuffer created" << std::endl;
}

void CellstateBuffer::Advance(std::vector<std::vector<int>> cells)
{
    buffer.push_back(cells);
}

std::vector<std::vector<int>> CellstateBuffer::Regress()
{
    if (buffer.size() <= 1)
    {
        std::cout << "Regress failed, buffer is empty" << std::endl;
        return buffer.back();
    }

    std::vector<std::vector<int>> back;
    back = buffer.back();

    buffer.pop_back();
    return back;
}

void CellstateBuffer::Log()
{
    std::cout << "--- CELLSTATE BUFFER LOG ---" << std::endl;
    std::cout << "buffer.size(): " << buffer.size() << std::endl;

    for (size_t i = 0; i < buffer.size(); ++i) {
        std::cout << "Block " << i << ":\n";
        for (size_t j = 0; j < buffer[i].size(); ++j) {
            std::cout << "  Row " << j << ": ";
            for (size_t k = 0; k < buffer[i][j].size(); ++k) {
                std::cout << buffer[i][j][k] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n"; // Add a line break between blocks for readability
    }
}

void CellstateBuffer::Clear()
{
    buffer.clear();
}

unsigned long CellstateBuffer::Size()
{
    return buffer.size();
}