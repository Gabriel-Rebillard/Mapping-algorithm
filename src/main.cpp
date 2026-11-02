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
        std::cout << "Usage : ./main <width> <height> [robot_speed] [seed]" << std::endl;
        return 0;
    }

    // Seed selection
    if (argc > 4)
        srand(std::stoi(argv[4]));
    else
        srand((unsigned)time(NULL));

    unsigned short robotSpeed = 100;
    if (argc > 3)
        robotSpeed = std::stoi(argv[3]);

    ClearConsole();

    
    // Map generation
    int w = std::stoi(argv[1]) , h = std::stoi(argv[2]);
    std::unique_ptr<Map> map = generateMap(w, h);

    Case c(10);

    std::cout << (int)c.GetVisitedCount() << std::endl; 
    std::cout << (int)map->GetCase(3, 3)->GetVisitedCount() << std::endl;
    std::cout << (int)map->GetCase(3, 3)->GetValue() << std::endl;

    // Display map
    DisplayMapSVG(map.get());
    
    // TESTS ****************************************************************************************
    Robot robot1 = Robot(0);
    Robot robot2 = Robot(1);
    uint8_t* robot1Start = findStart(map.get(), 0);
    uint8_t* robot2Start = findStart(map.get(), 1);

    std::cout << (int)map->GetCaseType(robot1Start[0], robot1Start[1]) << std::endl;
    std::cout << (int)map->GetCaseType(robot2Start[0], robot2Start[1]) << std::endl;

    std::cout << "robot1 start : x : " << (int)robot1Start[0] << "\ty : " << (int)robot1Start[1] << std::endl;
    std::cout << "robot2 start : x : " << (int)robot2Start[0] << "\ty : " << (int)robot2Start[1] << std::endl;
    robot1.AnalyzeEnvironment(map.get(), robot1Start);
    robot2.AnalyzeEnvironment(map.get(), robot2Start);


    Case temp1, temp2;

    DisplayRobot(map.get(), robot1, temp1, robot1Start);
    DisplayRobot(map.get(), robot2, temp2, robot2Start);

    DisplayMap(robot1.GetMap());
    std::cout << std::endl << std::endl;
    DisplayMap(robot2.GetMap());
    std::cout << std::endl;

    
    std::cin.ignore();
    ClearConsole();

    for (int i = 0; i < 2000; i++){
        RemoveDisplayedRobot(map.get(), robot1, temp1, robot1Start);
        RemoveDisplayedRobot(map.get(), robot2, temp2, robot2Start);

        while (!robot1.Move(rand() % 4) || !robot2.Move(rand() % 4));

        robot1.AnalyzeEnvironment(map.get(), robot1Start);
        robot2.AnalyzeEnvironment(map.get(), robot2Start);
        
        DisplayRobot(map.get(), robot1, temp1, robot1Start);
        DisplayRobot(map.get(), robot2, temp2, robot2Start);

        //ClearConsole();
        gotoxy(0, 0);
        DisplayMapSVG(map.get());
        PrintEmptyLine(2);
        DisplayMapSVG(robot1.GetMap());
        std::cout << (int)robot1.GetMap()->GetCaseType(2, 2) << "  " << (int)robot1.GetMap()->GetCase(2, 2)->GetVisitedCount() << std::endl;
        PrintEmptyLine(2);
        DisplayMapSVG(robot2.GetMap());

        std::this_thread::sleep_for(std::chrono::milliseconds(robotSpeed));
    }
    ClearConsole();
    DisplayMapSVG(map.get());
    std::cout << std::endl;
    DisplayMapSVG(robot1.GetMap());
    std::cout << std::endl << std::endl;
    DisplayMapSVG(robot2.GetMap());

    
    
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