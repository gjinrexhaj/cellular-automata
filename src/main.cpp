//
// Created by Gjin on 7/26/25.
//

#define RAYGUI_IMPLEMENTATION
#include "../external/raygui.h"
#include <raylib.h>
#include <iostream>
#include "simulation.hpp"


// TODO: decouple keyobard event handler framerate from rendering framerate to prevent unresponsiveness at low fps
// TODO: implement rng density
// TODO: work on brush size implementation
// TODO: implement "additive" toggle for fill-random


int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    auto gridlineColor = Color(20, 20, 20 , 255);
    auto aliveColor = Color(255, 255, 255, 255);
    auto deadColor = Color(40, 40, 40, 255);
    auto fontColor = Color(255, 255, 255, 255);

    int windowWidth = 950;
    int windowHeight = 650;
    int cellSize = 10;
    int fps = 30;

    float lineThickness = 1;
    int brushSize = 1;

    bool running = false;
    bool showText = true;
    bool showFps = true;

    bool showCreateNewEnvironmentPanel = false;
    bool showColorPickerPanel = false;
    bool showSettingsPanel = false;

    bool guiFocus1 = false;
    bool guiFocus2 = false;
    bool guiFocus3 = false;
    bool guiFocus4 = false;

    bool allowEditingWhileRunning = false;
    bool allowKeybindsDuringSimulation = false;
    bool darkMode = true;
    bool autoResizeEnvironment = false;


    InitWindow(windowWidth, windowHeight, "GoL Canvas - IDLE");
    GuiLoadStyle("../styles/custom-dark.rgs");

    SetTargetFPS(fps);
    Simulation simulation(windowWidth, windowHeight, cellSize);

    std::string controls =
        "A: decrease fps cap\n"
        "D: increase fps cap\n"
        "C: clear grid (sim must be stopped)\n"
        "P: toggle color picker\n"
        "R: randomize grid (sim must be stopped)\n"
        "F: toggle fps counter\n"
        "N: change simulation environment\n"
        "D: change settings\n"
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
            windowWidth = GetScreenWidth();
            windowHeight = GetScreenHeight();
            if (autoResizeEnvironment)
            {
                simulation = Simulation(windowWidth, windowHeight, cellSize);
            }
        }

        // place cell
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePosition = GetMousePosition();
            int row = mousePosition.y / cellSize;
            int column = mousePosition.x / cellSize;

            if (!simulation.IsRunning() || allowEditingWhileRunning)
            {
                if (simulation.GetCellValue(row, column) == 0)
                {
                    simulation.ToggleCell(row, column, brushSize);
                }
            }
        }
        // remove cell
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        {
            Vector2 mousePosition = GetMousePosition();
            int row = mousePosition.y / cellSize;
            int column = mousePosition.x / cellSize;

            if (!simulation.IsRunning() || allowEditingWhileRunning)
            {
                if (simulation.GetCellValue(row, column) == 1)
                {
                    simulation.ToggleCell(row, column, brushSize);
                }
            }
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            guiFocus1 = false;
            guiFocus2 = false;
            guiFocus3 = false;


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
            if (!simulation.IsRunning() || allowKeybindsDuringSimulation) {
                simulation.CreateRandomState();
            }
        }
        else if (IsKeyPressed(KEY_C))
        {
            if (!simulation.IsRunning() || allowKeybindsDuringSimulation) {
                simulation.ClearGrid();
            }
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
            showCreateNewEnvironmentPanel = !showCreateNewEnvironmentPanel;
        }
        else if (IsKeyPressed(KEY_P))
        {
            std::cout << "Show color picker panel" << std::endl;
            showColorPickerPanel = !showColorPickerPanel;
        }
        else if (IsKeyPressed(KEY_D))
        {
            std::cout << "Show settings panel" << std::endl;
            showSettingsPanel = !showSettingsPanel;
        }

        // Update State
        simulation.Update();

        // Drawing to Screen
        BeginDrawing();
        ClearBackground(gridlineColor);
        simulation.Draw(aliveColor, deadColor, lineThickness);
        if (showText)
        {
            DrawText(controls.c_str(), 10, GetScreenHeight() - 290, 20, fontColor);
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
        if (showCreateNewEnvironmentPanel)
        {
            Rectangle dialogRect = { 0, 0, 225, 250 };

            if (GuiWindowBox(dialogRect, "Create New Environment"))
            {
                showCreateNewEnvironmentPanel = false; // Close the dialog if the close button is pressed
            }

            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 40, 90, 20}, "Boundary Width ", &windowWidth, 1, 2000, guiFocus1))
            {
                std::cout<<"spinner1 req focus"<<std::endl;
                guiFocus1 = true;
                guiFocus2 = false;
                guiFocus3 = false;
            }

            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 70, 90, 20}, "Boundary Height ", &windowHeight, 1, 2000, guiFocus2))
            {
                std::cout<<"spinner2 req focus"<<std::endl;
                guiFocus1 = false;
                guiFocus2 = true;
                guiFocus3 = false;
            }

            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 100, 90, 20}, "Cell Size ", &cellSize, 1, 50, guiFocus3))
            {
                std::cout<<"spinner3 req focus"<<std::endl;
                guiFocus1 = false;
                guiFocus2 = false;
                guiFocus3 = true;
            }

            GuiLabel({dialogRect.x + 10, dialogRect.y + 160, 240, 20 }, "Warning! This action will delete");
            GuiLabel({dialogRect.x + 10, dialogRect.y + 175, 240, 20 }, "your current environment!");

            if (GuiButton((Rectangle){ dialogRect.x + 50, dialogRect.y + 200, 100, 30 }, "CREATE"))
            {
                std::cout << "value1: " << windowWidth << std::endl;
                std::cout << "value2: " << windowHeight << std::endl;
                std::cout << "value3: " << cellSize << std::endl;
                SetWindowTitle("GoL Canvas - IDLE");

                SetWindowSize(windowWidth, windowHeight);

                simulation = Simulation(windowWidth, windowHeight, cellSize);
                running = false;
            }
        }

        // COLOR PICKER DIALOG MENU
        if (showColorPickerPanel)
        {
            Rectangle dialogRect = { (float)GetScreenWidth()-250, 0, 250, 350 };

            if (GuiWindowBox(dialogRect, "Color Picker"))
            {
                showColorPickerPanel = false; // Close the dialog if the close button is pressed
            }

            GuiColorPicker({dialogRect.x + 10, dialogRect.y + 40, 80, 50}, "Gridline Color ", &gridlineColor);
            GuiLabel({dialogRect.x + 120, dialogRect.y + 40, 120, 50}, "Gridline Color ");
            GuiColorPicker({dialogRect.x + 10, dialogRect.y + 100, 80, 50}, "Alive Color ", &aliveColor);
            GuiLabel({dialogRect.x + 120, dialogRect.y + 100, 120, 50}, "Alive Color ");
            GuiColorPicker({dialogRect.x + 10, dialogRect.y + 160, 80, 50}, "Dead Color ", &deadColor);
            GuiLabel({dialogRect.x + 120, dialogRect.y + 160, 120, 50}, "Dead Color ");
            GuiColorPicker({dialogRect.x + 10, dialogRect.y + 220, 80, 50}, "Font Color ", &fontColor);
            GuiLabel({dialogRect.x + 120, dialogRect.y + 220, 120, 50}, "Font Color");
            GuiSlider({dialogRect.x + 10, dialogRect.y + 280, 80, 50}, "", "", &lineThickness, 0, 3);
            GuiLabel({dialogRect.x + 100, dialogRect.y + 280, 130, 50}, "Gridline Thickness");
        }

        // SETTINGS DIALOG MENU
        if (showSettingsPanel)
        {

            Rectangle dialogRect = {0, (float)GetScreenHeight()-350, 300, 350 };

            if (GuiWindowBox(dialogRect, "Settings"))
            {
                showSettingsPanel = false; // Close the dialog if the close button is pressed
            }

            // add brush shape, options, etc,
            if (GuiCheckBox({dialogRect.x + 15, dialogRect.y + 40, 20, 20}, " Dark mode", &darkMode))
            {
                if (darkMode)
                {
                    GuiLoadStyleDefault();
                    GuiLoadStyle("../styles/custom-dark.rgs");
                } else
                {
                    GuiLoadStyleDefault();
                    GuiLoadStyle("../styles/custom-light.rgs");
                }
            }

            // TODO: fully implement brush size feature
            GuiCheckBox({dialogRect.x + 15, dialogRect.y + 80, 20, 20}, " allow drawing during simulation", &allowEditingWhileRunning);
            GuiCheckBox({dialogRect.x + 15, dialogRect.y + 120, 20, 20}, " allow C/R keys during simulation", &allowKeybindsDuringSimulation);
            GuiCheckBox({dialogRect.x + 15, dialogRect.y + 160, 20, 20}, " create new envrmt on window resize", &autoResizeEnvironment);
            if (GuiSpinner({dialogRect.x + 143, dialogRect.y + 200, 100, 20}, "Brush/Eraser Size    ", &brushSize, 1, 300, guiFocus4))
            {
                guiFocus1 = false;
                guiFocus2 = false;
                guiFocus3 = false;
                guiFocus4 = true;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}