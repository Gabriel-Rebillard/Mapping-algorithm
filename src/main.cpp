#include "../includes/map.h"
#include "../includes/mapReader.h"
#include "../includes/mapGenerator.h"
#include "../includes/robot.h"
#include <iostream>
#include <thread>
#include <chrono>


int main(int argc, char **argv)
{
    // Help infos 
    if (argv[1] == NULL || argv[1] == "-h" || argv[1] == "--help" || argv[1] == "help"){
        std::cout << "Usage : ./main <width> <height> [seed]" << std::endl;
        return 0;
    }

    // Seed selection
    if (argv[3] != NULL)
        srand(std::stoi(argv[3]));
    else
        srand((unsigned)time(NULL));

    ClearConsole();

    
    // Map generation
    int w = std::stoi(argv[1]) , h = std::stoi(argv[2]);
    std::unique_ptr<Map> map = generateMap(w, h);


    // Display map
    DisplayMap(map.get());


    // TESTS ****************************************************************************************
    Robot robot = Robot();
    uint8_t* robotStart = findStart(map.get());
    std::cout << "robot start : x : " << (int)robotStart[0] << "\ty : " << (int)robotStart[1] << std::endl;
    robot.FillMap(map.get(), robotStart[0], robotStart[1]);


    Case temp = *robot.GetMap()->GetCase(1, 1);
    map->SetCaseType(robotStart, ROBOT_ID);
    robot.GetMap()->SetCaseType(robotStart, ROBOT_ID);
    DisplayMap(robot.GetMap());
    std::cout << std::endl;

    
    std::cin.ignore();
    ClearConsole();

    for (int i = 0; i < 500; i++){
        robot.GetMap()->SetCaseType(robot.GetPosition()[0], robot.GetPosition()[1], temp.GetValue());
        map->SetCaseType(robotStart[0] - robot.GetStartPoint()[0] + robot.GetPosition()[0], robotStart[1] - robot.GetStartPoint()[1] + robot.GetPosition()[1], temp.GetValue());
        while (!robot.Move(rand() % 4));

        robot.AnalyzeEnvironment(map.get(), robotStart);
        
        temp = *robot.GetMap()->GetCase(robot.GetPosition()[0], robot.GetPosition()[1]);
        robot.GetMap()->SetCaseType(robot.GetPosition()[0], robot.GetPosition()[1], ROBOT_ID);
        map->SetCaseType(robotStart[0] - robot.GetStartPoint()[0] + robot.GetPosition()[0], robotStart[1] - robot.GetStartPoint()[1] + robot.GetPosition()[1], ROBOT_ID);

        //ClearConsole();
        gotoxy(0, 0);
        //DisplayMap(map.get());
        DisplayMap(robot.GetMap());

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    ClearConsole();
    DisplayMap(map.get());
    std::cout << std::endl;
    DisplayMap(robot.GetMap());
    
    /*
    // neighbours verification on case (2;2)
    auto n = map->GetNeighbours(2, 2);
    for (auto neighbour = n.get()->begin(); neighbour != n.get()->end(); neighbour++){
        uint8_t* pos = map->GetCasePosition(*neighbour);
        std::cout << "x : " << (int)pos[0] << " y : " << (int)pos[1] << std::endl;
        std::cout << (int)vToC(map->GetCase(2, 2))->GetEdgeWeight(*neighbour) << std::endl;
    }*/
    
    return 0;
}