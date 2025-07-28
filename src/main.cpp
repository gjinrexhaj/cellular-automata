//
// Created by Gjin on 7/26/25.
//

#define RAYGUI_IMPLEMENTATION
#include "../external/raygui.h"
#include <raylib.h>
#include <iostream>
#include "simulation.hpp"


// TODO: decouple keyobard event handler framerate from rendering framerate to prevent unresponsiveness at low fps


int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Color gridlineColor = Color(20, 20, 20 , 255);
    Color aliveColor = Color(255, 255, 255, 255);
    Color deadColor = Color(40, 40, 40, 255);
    Color fontColor = Color(255, 255, 255, 255);

    const int WINDOW_WIDTH = 950;
    const int WINDOW_HEIGHT = 650;
    int cellSize = 10;
    int fps = 30;
    bool running = false;
    bool showText = true;
    bool showFps = true;
    bool showNewEnvironmentWindow = false;
    bool showColorPickerWindow = false;

    char textBuf1[64] = "Boundary Width ";
    char textBuf2[64] = "Boundary Height ";
    char textBuf3[64] = "Cell Size ";
    char textBuf4[64] = "Gridline Color ";
    char textBuf5[64] = "Alive Color ";
    char textBuf6[64] = "Dead Color ";
    char textBuf7[64] = "Font Color ";
    char textBuf8[64] = "";
    char textBuf9[64] = "Gridline Thickness";

    float lineThickness = 1;
    int value1 = WINDOW_WIDTH;
    int value2 = WINDOW_HEIGHT;
    int value3 = cellSize;

    bool focus1 = false;
    bool focus2 = false;
    bool focus3 = false;


    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GoL Canvas - IDLE");
    GuiLoadStyle("../styles/custom-dark.rgs");

    SetTargetFPS(fps);
    Simulation simulation(WINDOW_WIDTH, WINDOW_HEIGHT, cellSize);

    std::string controls =
        "A: decrease fps cap\n"
        "D: increase fps cap\n"
        "C: clear grid (sim must be stopped)\n"
        "P: toggle color picker\n"
        "R: randomize grid (sim must be stopped)\n"
        "F: toggle fps counter\n"
        "N: change simulation environment\n"
        "LMOUSE: draw cells (sim must be stopped)\n"
        "RMOUSE: erase cells (sim must be stopped)\n"
        "ENTER: toggle simulation\n"
        "ESC: quit program\n"
        "Q: show/hide this text";

    // Simulation Loop
    while (!WindowShouldClose())
    {
        // Handle Events
        if (IsWindowResized())
        {
            value1 = GetScreenWidth();
            value2 = GetScreenHeight();
        }

        // place cell
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePosition = GetMousePosition();
            int row = mousePosition.y / cellSize;
            int column = mousePosition.x / cellSize;
            if (simulation.GetCellValue(row, column) == 0)
            {
                simulation.ToggleCell(row, column);
            }
        }
        // remove cell
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        {
            Vector2 mousePosition = GetMousePosition();
            int row = mousePosition.y / cellSize;
            int column = mousePosition.x / cellSize;
            if (simulation.GetCellValue(row, column) == 1)
            {
                simulation.ToggleCell(row, column);
            }
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            focus1 = false;
            focus2 = false;
            focus3 = false;


            running = !running;

            if (running)
            {
                SetWindowTitle("GoL Canvas - RUNNING");
                simulation.Start();
            }
            else
            {
                SetWindowTitle("GoL Canvas - IDLE");
                simulation.Stop();
            }
        }
        else if (IsKeyPressed(KEY_S))
        {
            fps += 2;
            SetTargetFPS(fps);
        }
        else if (IsKeyPressed(KEY_A))
        {
            if (fps > 5)
            {
                fps -= 2;
                SetTargetFPS(fps);
            }
        }
        else if (IsKeyPressed(KEY_R))
        {
            simulation.CreateRandomState();
        }
        else if (IsKeyPressed(KEY_C))
        {
            simulation.ClearGrid();
        }
        else if (IsKeyPressed(KEY_Q))
        {
            showText = !showText;
        }
        else if (IsKeyPressed(KEY_F))
        {
            showFps = !showFps;
        }
        else if (IsKeyPressed(KEY_N))
        {
            std::cout << "Show new environment panel" << std::endl;
            showNewEnvironmentWindow = !showNewEnvironmentWindow;
        }
        else if (IsKeyPressed(KEY_P))
        {
            std::cout << "Show color picker panel" << std::endl;
            showColorPickerWindow = !showColorPickerWindow;
        }

        // Update State
        simulation.Update();

        // Drawing to Screen
        BeginDrawing();
        ClearBackground(gridlineColor);
        simulation.Draw(aliveColor, deadColor, lineThickness);
        if (showText)
        {
            DrawText(controls.c_str(), 10, GetScreenHeight() - 265, 20, fontColor);
        }
        if (showFps)
        {
            DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, fontColor);
            if (running)
            {
                DrawText(TextFormat("Running"), GetScreenWidth() - 90, 10, 20, fontColor);
            }
            else
            {
                DrawText(TextFormat("Idle"), GetScreenWidth() - 50, 10, 20, fontColor);
            }
        }

        // NEW WINDOW DIALOG MENU
        if (showNewEnvironmentWindow)
        {
            Rectangle dialogRect = { 0, 0, 225, 250 };

            if (GuiWindowBox(dialogRect, "Create New Environment"))
            {
                showNewEnvironmentWindow = false; // Close the dialog if the close button is pressed
            }



            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 40, 90, 20}, textBuf1, &value1, 1, 2000, focus1))
            {
                std::cout<<"spinner1 req focus"<<std::endl;
                focus1 = true;
                focus2 = false;
                focus3 = false;
            }

            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 70, 90, 20}, textBuf2, &value2, 1, 2000, focus2))
            {
                std::cout<<"spinner2 req focus"<<std::endl;
                focus1 = false;
                focus2 = true;
                focus3 = false;
            }

            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 100, 90, 20}, textBuf3, &value3, 1, 50, focus3))
            {
                std::cout<<"spinner3 req focus"<<std::endl;
                focus1 = false;
                focus2 = false;
                focus3 = true;
            }

            GuiLabel({dialogRect.x + 10, dialogRect.y + 160, 240, 20 }, "Warning! This action will delete\nyour current environment!");


            if (GuiButton((Rectangle){ dialogRect.x + 50, dialogRect.y + 200, 100, 30 }, "CREATE"))
            {
                std::cout << "value1: " << value1 << std::endl;
                std::cout << "value2: " << value2 << std::endl;
                std::cout << "value3: " << value3 << std::endl;
                SetWindowTitle("GoL Canvas - IDLE");
                cellSize = value3;

                SetWindowSize(value1, value2);

                simulation = Simulation(value1, value2, value3);
                running = false;
            }
        }

        // COLOR PICKER DIALOG MENU
        if (showColorPickerWindow)
        {
            Rectangle dialogRect = { (float)GetScreenWidth()-250, 0, 250, 350 };

            if (GuiWindowBox(dialogRect, "Color Picker"))
            {
                showColorPickerWindow = false; // Close the dialog if the close button is pressed
            }



            GuiColorPicker({dialogRect.x + 10, dialogRect.y + 40, 80, 50}, textBuf4, &gridlineColor);
            GuiLabel({dialogRect.x + 120, dialogRect.y + 40, 120, 50}, textBuf4);

            GuiColorPicker({dialogRect.x + 10, dialogRect.y + 100, 80, 50}, textBuf5, &aliveColor);
            GuiLabel({dialogRect.x + 120, dialogRect.y + 100, 120, 50}, textBuf5);

            GuiColorPicker({dialogRect.x + 10, dialogRect.y + 160, 80, 50}, textBuf6, &deadColor);
            GuiLabel({dialogRect.x + 120, dialogRect.y + 160, 120, 50}, textBuf6);

            GuiColorPicker({dialogRect.x + 10, dialogRect.y + 220, 80, 50}, textBuf7, &fontColor);
            GuiLabel({dialogRect.x + 120, dialogRect.y + 220, 120, 50}, textBuf7);

            GuiSlider({dialogRect.x + 10, dialogRect.y + 280, 80, 50}, textBuf8, textBuf8, &lineThickness, 0, 3);
            GuiLabel({dialogRect.x + 100, dialogRect.y + 280, 120, 50}, textBuf9);

        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}