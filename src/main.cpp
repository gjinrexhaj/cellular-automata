//
// Created by Gjin on 7/26/25.
//

#define RAYGUI_IMPLEMENTATION
#include "../external/raygui.h"
#include <raylib.h>
#include <iostream>
#include "simulation.hpp"


// TODO: work on brush size implementation
// TODO: implement frame advancement undo feature - also need to implement FA buffer
// TODO: optimizations + other performance improvements (maybe use multi-threading)
// TODO: remove guiFocus boolean spaghetti code


int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    auto gridlineColor = Color(20, 20, 20 , 255);
    auto aliveColor = Color(185, 185, 185, 225);
    auto deadColor = Color(40, 40, 40, 255);
    auto fontColor = Color(255, 255, 255, 255);

    int windowWidth = 950;
    int windowHeight = 650;
    int cellSize = 10;
    int fps = 30;
    int chosenSimulationType = 0;

    float lineThickness = 1;
    int brushSize = 1;
    int rngDensity = 6;
    int advanceFactor = 1;

    bool running = false;
    bool showText = true;
    bool showFps = true;

    bool showCreateNewEnvironmentPanel = false;
    bool showColorPickerPanel = false;
    bool showSettingsPanel = false;
    bool showFrameAdvancePanel = false;

    bool guiFocus1 = false;
    bool guiFocus2 = false;
    bool guiFocus3 = false;
    bool guiFocus4 = false;
    bool guiFocus5 = false;
    bool guiFocus6 = false;
    bool guiFocus7 = false;
    bool guiFocus8 = false;

    bool allowEditingWhileRunning = false;
    bool allowKeybindsDuringSimulation = false;
    bool darkMode = true;
    bool autoResizeEnvironment = false;
    bool additiveFill = false;

    bool enableFrameAdvance = false;
    int selectedDropdownItem = 0;

    auto selectedSimulationType = SimulationType::GAME_OF_LIFE;

    InitWindow(windowWidth, windowHeight, "Automata Engine: IDLE");
    GuiLoadStyle("../styles/custom-dark.rgs");

    SetTargetFPS(fps);
    Simulation simulation(windowWidth, windowHeight, cellSize, selectedSimulationType);

    std::string controls =
        "--- MENU CONTROLS ---\n"
        "M: frame advance menu\n"
        "P: color picker menu\n"
        "Q: create new environment menu\n"
        "Z: general settings menu\n"
        "\n\n"
        "----- HOTKEYS -----\n"
        "A: decrease fps cap\n"
        "D: increase fps cap\n"
        "C: clear grid\n"
        "R: randomize grid\n"
        "LMOUSE: add cell\n"
        "LMOUSE: remove cell\n"
        "ENTER: idle/run engine\n"
        "\n"
        "H: hide this text\n"
        "F: hide fps counter\n";

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
                simulation = Simulation(windowWidth, windowHeight, cellSize, selectedSimulationType);
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

        // check for hotkey input
        if (IsKeyPressed(KEY_ENTER))
        {
            guiFocus1 = false;
            guiFocus2 = false;
            guiFocus3 = false;


            running = !running;

            if (running)
            {
                SetWindowTitle("Automata Engine: RUNNING");
                simulation.Start();
            }
            else
            {
                SetWindowTitle("Automata Engine: IDLE");
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
                simulation.CreateRandomState(rngDensity, additiveFill);
            }
        }
        else if (IsKeyPressed(KEY_C))
        {
            if (!simulation.IsRunning() || allowKeybindsDuringSimulation) {
                simulation.ClearGrid();
            }
        }
        else if (IsKeyPressed(KEY_H))
        {
            showText = !showText;
        }
        else if (IsKeyPressed(KEY_F))
        {
            showFps = !showFps;
        }
        else if (IsKeyPressed(KEY_Q))
        {
            std::cout << "Showing new environment panel" << std::endl;
            showCreateNewEnvironmentPanel = !showCreateNewEnvironmentPanel;
        }
        else if (IsKeyPressed(KEY_P))
        {
            std::cout << "Showing color picker panel" << std::endl;
            showColorPickerPanel = !showColorPickerPanel;
        }
        else if (IsKeyPressed(KEY_Z))
        {
            std::cout << "Showing general settings panel" << std::endl;
            showSettingsPanel = !showSettingsPanel;
        }
        else if (IsKeyPressed(KEY_M))
        {
            std::cout << "Showing frame advance panel" << std::endl;
            showFrameAdvancePanel = !showFrameAdvancePanel;
        }
        else if (enableFrameAdvance && IsKeyPressed(KEY_RIGHT))
        {
            bool simWasRunning = simulation.IsRunning();
            std::cout << "Frame advance for: " << advanceFactor << " frames" << std::endl;

            if (!simWasRunning)
            {
                simulation.Start();
            }

            for (int i = 0; i < advanceFactor; i++)
            {
                simulation.Update();
                ClearBackground(gridlineColor);
                simulation.Draw(aliveColor, deadColor, lineThickness);
            }

            if (!simWasRunning)
            {
                simulation.Stop();
            }
        }
        else if (enableFrameAdvance && IsKeyPressed(KEY_LEFT))
        {
            std::cerr << "Frame regress not yet implemented" << std::endl;
        }

        // Update State
        simulation.Update();

        // Drawing to Screen
        BeginDrawing();
        ClearBackground(gridlineColor);
        simulation.Draw(aliveColor, deadColor, lineThickness);
        if (showText)
        {
            int textSize = MeasureText(controls.c_str(), 20);
            DrawText(controls.c_str(), (GetScreenWidth() - textSize)/2, (GetScreenHeight() - textSize)/2, 20, fontColor);
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
            Rectangle dialogRect = { 0, 0, 225, 280 };

            if (GuiWindowBox(dialogRect, "Create New Environment [Q]"))
            {
                showCreateNewEnvironmentPanel = false; // Close the dialog if the close button is pressed
            }

            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 40, 90, 20}, "Boundary Width ", &windowWidth, 1, 2000, guiFocus1))
            {
                std::cout<<"Boundary width spinner req focus"<<std::endl;
                guiFocus1 = true;
                guiFocus2 = false;
                guiFocus3 = false;
                guiFocus4 = false;
                guiFocus5 = false;
                guiFocus6 = false;
                guiFocus7 = false;
                guiFocus8 = false;
            }

            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 70, 90, 20}, "Boundary Height ", &windowHeight, 1, 2000, guiFocus2))
            {
                std::cout<<"Boundary height req focus"<<std::endl;
                guiFocus1 = false;
                guiFocus2 = true;
                guiFocus3 = false;
                guiFocus4 = false;
                guiFocus5 = false;
                guiFocus6 = false;
                guiFocus7 = false;
                guiFocus8 = false;
            }

            if (GuiSpinner({dialogRect.x + 120, dialogRect.y + 100, 90, 20}, "Cell Size ", &cellSize, 1, 50, guiFocus3))
            {
                std::cout<<"Cell size spinner req focus"<<std::endl;
                guiFocus1 = false;
                guiFocus2 = false;
                guiFocus3 = true;
                guiFocus4 = false;
                guiFocus5 = false;
                guiFocus6 = false;
                guiFocus7 = false;
                guiFocus8 = false;
            }

            GuiLabel({dialogRect.x + 10, dialogRect.y + 190, 240, 20 }, "Warning! This action will delete");
            GuiLabel({dialogRect.x + 10, dialogRect.y + 205, 240, 20 }, "your current environment!");

            if (GuiButton({ dialogRect.x + 50, dialogRect.y + 230, 100, 30 }, "CREATE"))
            {
                std::cout<<"CREATE button pressed"<<std::endl;
                std::cout << "windowWidth: " << windowWidth << std::endl;
                std::cout << "windowHeight: " << windowHeight << std::endl;
                std::cout << "cellSize: " << cellSize << std::endl;
                std::cout << "chosenSimulationType: " << chosenSimulationType << std::endl;
                SetWindowTitle("Automata Engine: IDLE");

                SetWindowSize(windowWidth, windowHeight);

                simulation = Simulation(windowWidth, windowHeight, cellSize, selectedSimulationType);
                running = false;
            }

            // simulation mode dropdown
            if ((selectedDropdownItem = GuiDropdownBox({dialogRect.x + 20, dialogRect.y + 130, 185, 20}, "Game of life;Torpedo;Diamond Growth;High Life", &chosenSimulationType, guiFocus7)))
            {
                std::cout<<"Simulation type dropdown req focus"<<std::endl;
                guiFocus1 = false;
                guiFocus2 = false;
                guiFocus3 = false;
                guiFocus4 = false;
                guiFocus5 = false;
                guiFocus6 = false;
                guiFocus7 ? guiFocus7 = false : guiFocus7 = true;
                guiFocus8 = false;

                // update selectedSimulationType enum
                switch (chosenSimulationType)
                {
                    case 0:
                        std::cout<<"GAME_OF_LIFE RULESET SELECTED" << std::endl;
                        selectedSimulationType = SimulationType::GAME_OF_LIFE;
                        break;
                    case 1:
                        std::cout<<"TORPEDO RULESET SELECTED" << std::endl;
                        selectedSimulationType = SimulationType::TORPEDO;
                        break;
                    case 2:
                        std::cout<<"DIAMOND GROWTH RULESET SELECTED" << std::endl;
                        selectedSimulationType = SimulationType::DIAMOND_GROWTH;
                        break;
                    case 3:
                        std::cout<<"HIGH_LIFE RULESET SELECTED" << std::endl;
                        selectedSimulationType = SimulationType::HIGH_LIFE;
                        break;
                    default:
                        std::cerr<<"Warning: Unknown simulation type selected!"<<std::endl;
                }
            }

            if (selectedDropdownItem == 1)
            {
                guiFocus1 = false;
            }


        }

        // COLOR PICKER DIALOG MENU
        if (showColorPickerPanel)
        {
            Rectangle dialogRect = { (float)GetScreenWidth()-250, 0, 250, 350 };

            if (GuiWindowBox(dialogRect, "Color Picker [P]"))
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

            if (GuiWindowBox(dialogRect, "General Settings [Z]"))
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
            GuiCheckBox({dialogRect.x + 15, dialogRect.y + 200, 20, 20}, " additive fill", &additiveFill);
            if (GuiSpinner({dialogRect.x + 143, dialogRect.y + 240, 100, 20}, "Brush/Eraser Size    ", &brushSize, 1, 300, guiFocus4))
            {
                guiFocus1 = false;
                guiFocus2 = false;
                guiFocus3 = false;
                guiFocus4 = true;
                guiFocus5 = false;
                guiFocus6 = false;
                guiFocus7 = false;
                guiFocus8 = false;
            }

            if (GuiSpinner({dialogRect.x + 143, dialogRect.y + 280, 100, 20}, "RNG sparsity    ", &rngDensity, 0, 200, guiFocus5))
            {
                guiFocus1 = false;
                guiFocus2 = false;
                guiFocus3 = false;
                guiFocus4 = false;
                guiFocus5 = true;
                guiFocus6 = false;
                guiFocus7 = false;
                guiFocus8 = false;
            }
        }

        if (showFrameAdvancePanel)
        {
            Rectangle dialogRect = { (float)GetScreenWidth()-250, (float)GetScreenHeight()-300, 250, 300 };

            if (GuiWindowBox(dialogRect, "Frame Advancement [M]"))
            {
                showFrameAdvancePanel = false; // Close the dialog if the close button is pressed
            }

            GuiLabel({dialogRect.x +10, dialogRect.y +15, 250, 50}, "RIGHT_ARROW: Forwards");
            GuiLabel({dialogRect.x +10, dialogRect.y +35, 250, 50}, "LEFT_ARROW: Backwards");
            GuiCheckBox({dialogRect.x +10, dialogRect.y +85, 20, 20}, "Enable Frame Advance", &enableFrameAdvance);

            if (GuiSpinner({dialogRect.x +110, dialogRect.y +115, 90, 20}, "Advance Factor ", &advanceFactor, 1, 400, guiFocus6))
            {
                guiFocus1 = false;
                guiFocus2 = false;
                guiFocus3 = false;
                guiFocus4 = false;
                guiFocus5 = false;
                guiFocus6 = true;
                guiFocus7 = false;
                guiFocus8 = false;
            }

            int prevFPS = fps;

            if (GuiSpinner({dialogRect.x +110, dialogRect.y +145, 90, 20}, "Current FPS Cap ", &fps, 5, 600, guiFocus8))
            {
                guiFocus1 = false;
                guiFocus2 = false;
                guiFocus3 = false;
                guiFocus4 = false;
                guiFocus5 = false;
                guiFocus6 = false;
                guiFocus7 = false;
                guiFocus8 = true;
            }

            if (prevFPS != fps)
            {
                std::cout << "FPS changed from FA menu" << std::endl;
                SetTargetFPS(fps);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}