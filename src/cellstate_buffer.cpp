//
// Created by Gjin on 9/25/25.
//

#include "cellstate_buffer.h"

#include <iostream>
#include <ostream>

#include "simulation.hpp"

CellstateBuffer::CellstateBuffer()
{
    std::cout << "CellstateBuffer created" << std::endl;
    maximumSize = 600;
    useMaxSize = true;
}

void CellstateBuffer::Advance(std::vector<std::vector<int>> cells)
{
    if (useMaxSize && buffer.size() == maximumSize)
    {
        std::cout << "MAX SIZE REACHED, REMOVING FIRST ELEMENT" << std::endl;
        buffer.erase(buffer.begin());
    }
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
    buffer.shrink_to_fit();
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
    std::vector<std::vector<std::vector<int>>>().swap(buffer);
    buffer.shrink_to_fit();
}

unsigned long CellstateBuffer::Size()
{
    return buffer.size();
}

void CellstateBuffer::Cutoff() {
    buffer.erase(buffer.begin());
    std::cout << "Erasing front of buffer" << std::endl;
    buffer.shrink_to_fit();
}

int CellstateBuffer::getMaxSize()
{
    return maximumSize;
}

void CellstateBuffer::setMaxSize(int maxSize)
{
    if (!useMaxSize)
    {
        std::cout << "setMaxSize() failed: useMaxSize = false" << std::endl;
        return;
    }

    if (maxSize == maximumSize)
    {
        return;
    }

    std::cout << "executed setMaxSize(" << maxSize << ");" << std::endl;

    int oldSize = buffer.size();
    int newSize = maxSize;
    int elementsRemoved = oldSize - newSize;

    maximumSize = newSize;

    if (buffer.size() > maximumSize)
    {
        std::cout << "buffer resized to smaller, removing first " << elementsRemoved << " elements" << std::endl;
        buffer.erase(buffer.begin(), buffer.begin() + elementsRemoved);
    }
    buffer.shrink_to_fit();
}

void CellstateBuffer::toggleUseMaxSize()
{
    useMaxSize = !useMaxSize;

    if (buffer.size() > maximumSize)
    {
        int removeAmount = buffer.size() - maximumSize;
        std::cout << "buffer resized to smaller, removing first " << removeAmount << " elements" << std::endl;
        buffer.erase(buffer.begin(), buffer.begin() + removeAmount);
    }
    buffer.shrink_to_fit();
}

bool CellstateBuffer::isUseMaxSizeEnabled()
{
    return useMaxSize;
}

size_t CellstateBuffer::getBytesUsed()
{
    size_t total_bytes = 0;
    total_bytes += sizeof(buffer);

    // Iterate through each element of the vector
    for (const auto& inner_vec : buffer) {
        total_bytes += sizeof(inner_vec);
        for (const auto& innermost_vec : inner_vec) {
            total_bytes += sizeof(innermost_vec);
            total_bytes += innermost_vec.size() * sizeof(std::vector<int>::value_type);
        }
    }

    return total_bytes;
}
