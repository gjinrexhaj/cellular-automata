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
    bool showSettingsWindow = false;

    float lineThickness = 1;
    int brushSize = 1;

    int value1 = WINDOW_WIDTH;
    int value2 = WINDOW_HEIGHT;
    int value3 = cellSize;

    bool focus1 = false;
    bool focus2 = false;
    bool focus3 = false;
    bool focus4 = false;

    bool allowEditingWhileRunning = false;
    bool allowKeybindsDuringSimulation = false;
    bool darkMode = true;
    bool autoResizeEnvironment = false;


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
            value1 = GetScreenWidth();
            value2 = GetScreenHeight();
            if (autoResizeEnvironment)
            {
                value1 = GetScreenWidth();
                value2 = GetScreenHeight();
                simulation = Simulation(value1, value2, value3);
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
            showNewEnvironmentWindow = !showNewEnvironmentWindow;
        }
        else if (IsKeyPressed(KEY_P))
        {
            std::cout << "Show color picker panel" << std::endl;
            showColorPickerWindow = !showColorPickerWindow;
        }
        else if (IsKeyPressed(KEY_D))
        {
            std::cout << "Show settings panel" << std::endl;
            showSettingsWindow = !showSettingsWindow;
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
        if (showNewEnvironmentWindow)
        {
            Rectangle dialogRect = { 0, 0, 225, 250 };

            if (GuiWindowBox(dialogRect, "Create New Environment"))
            {
                showNewEnvironmentWindow = false; // Close the dialog if the close button is pressed
            }

            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 40, 90, 20}, "Boundary Width ", &value1, 1, 2000, focus1))
            {
                std::cout<<"spinner1 req focus"<<std::endl;
                focus1 = true;
                focus2 = false;
                focus3 = false;
            }

            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 70, 90, 20}, "Boundary Height ", &value2, 1, 2000, focus2))
            {
                std::cout<<"spinner2 req focus"<<std::endl;
                focus1 = false;
                focus2 = true;
                focus3 = false;
            }

            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 100, 90, 20}, "Cell Size ", &value3, 1, 50, focus3))
            {
                std::cout<<"spinner3 req focus"<<std::endl;
                focus1 = false;
                focus2 = false;
                focus3 = true;
            }

            GuiLabel({dialogRect.x + 10, dialogRect.y + 160, 240, 20 }, "Warning! This action will delete");
            GuiLabel({dialogRect.x + 10, dialogRect.y + 175, 240, 20 }, "your current environment!");

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
        if (showSettingsWindow)
        {

            Rectangle dialogRect = {0, (float)GetScreenHeight()-350, 300, 350 };

            if (GuiWindowBox(dialogRect, "Settings"))
            {
                showSettingsWindow = false; // Close the dialog if the close button is pressed
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

            // TODO: implement brush size feature
            GuiCheckBox({dialogRect.x + 15, dialogRect.y + 80, 20, 20}, " allow drawing during simulation", &allowEditingWhileRunning);
            GuiCheckBox({dialogRect.x + 15, dialogRect.y + 120, 20, 20}, " allow C/R keys during simulation", &allowKeybindsDuringSimulation);
            GuiCheckBox({dialogRect.x + 15, dialogRect.y + 160, 20, 20}, " create new envrmt on window resize", &autoResizeEnvironment);
            if (GuiSpinner({dialogRect.x + 143, dialogRect.y + 200, 100, 20}, "Brush/Eraser Size    ", &brushSize, 1, 200, focus4))
            {
                focus1 = false;
                focus2 = false;
                focus3 = false;
                focus4 = true;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}