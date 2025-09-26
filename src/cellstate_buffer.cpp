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

void CellstateBuffer::Cutoff() {
    buffer.erase(buffer.begin());
    std::cout << "Erasing front of buffer" << std::endl;
}

int CellstateBuffer::getMaxSize()
{
    return maximumSize;
}

// 600 -> 400

// remove first 200



// 900 -> 600 = 900 - 600

void CellstateBuffer::setMaxSize(int maxSize)
{
    // TODO: clean up code
    if (!useMaxSize)
    {
        std::cout << "setMaxSize() failed: useMaxSize = false" << std::endl;
        return;
    }

    int oldSize = buffer.size();
    int newSize = maxSize;
    int elementsRemoved = oldSize - newSize;

    maximumSize = newSize;

    if (buffer.size() > maximumSize)
    {
        std::cout << "buffer resized to smaller, removing first " << elementsRemoved << " elements" << std::endl;
        buffer.erase(buffer.begin(), buffer.begin() + elementsRemoved);
    }
}


// 900 -> 600 = cut off first 300

void CellstateBuffer::toggleUseMaxSize()
{
    useMaxSize = !useMaxSize;

    if (buffer.size() > maximumSize)
    {
        int removeAmount = buffer.size() - maximumSize;
        std::cout << "buffer resized to smaller, removing first " << removeAmount << " elements" << std::endl;
        buffer.erase(buffer.begin(), buffer.begin() + removeAmount);
    }
}

bool CellstateBuffer::isUseMaxSizeEnabled()
{
    return useMaxSize;
}