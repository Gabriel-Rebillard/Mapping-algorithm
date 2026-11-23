#include "../includes/map.h"
#include "../includes/mapReader.h"
#include "../includes/mapGenerator.h"
#include "../includes/robot.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <algorithm>

// ============================================================================
// Fonctions d'exploration (ajout Erwan)
// ============================================================================

// true si (x, y) est dans la carte et que la case est inconnue
bool EstInconnue(Map *carte, int x, int y)
{
    if (x < 0 || y < 0 || x >= carte->GetWidth() || y >= carte->GetHeight())
        return false;
    return carte->GetCaseType(x, y) == UNKNOWN_ID;
}

// true si, une fois sur la frontière (fx, fy), le robot découvrirait quelque chose :
// au moins une case inconnue dans son champ de vision 3x3.
// Une frontière sans rien d'inconnu autour est inutile : on sait déjà tout
// d'elle (y compris s'il y a une victime), y aller ne ferait rien découvrir.
bool FrontiereUtile(Map *carte, int fx, int fy)
{
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            int x = fx + dx;
            int y = fy + dy;

            // Hors de la carte du robot : terrain jamais vu, donc utile
            if (x < 0 || y < 0 || x >= carte->GetWidth() || y >= carte->GetHeight())
                return true;

            if (EstInconnue(carte, x, y))
                return true;
        }
    }
    return false;
}

// Cherche la frontière la plus proche du robot (distance de Manhattan),
// en ignorant celles déjà essayées et celles sans rien à découvrir.
// Renvoie false s'il n'en reste aucune.
bool ChercherFrontiereLaPlusProche(Robot &robot, std::vector<std::vector<bool>> &essayee, int &fx, int &fy)
{
    Map *carte = robot.GetMap();
    auto graphe = carte->GetMap().GetGraph(); // toutes les cases, rangées ligne par ligne
    int largeur = carte->GetWidth();
    int rx = robot.GetPosition()[0];
    int ry = robot.GetPosition()[1];
    int meilleureDistance = -1;

    // La condition donnée à find_if : "cette case est-elle une frontière ?"
    auto estFrontiere = [](const std::unique_ptr<Vertex> &v)
    {
        return isFrontier(v->GetValue());
    };

    // Première frontière de la carte
    auto it = std::find_if(graphe->begin(), graphe->end(), estFrontiere);

    while (it != graphe->end())
    {
        // Position de la case à partir de sa place dans le tableau
        int indice = std::distance(graphe->begin(), it);
        int x = indice % largeur;
        int y = indice / largeur;

        if (!(x == rx && y == ry) && !essayee[y][x] && FrontiereUtile(carte, x, y))
        {
            int distance = std::abs(x - rx) + std::abs(y - ry);
            if (meilleureDistance == -1 || distance < meilleureDistance)
            {
                meilleureDistance = distance;
                fx = x;
                fy = y;
            }
        }

        // Frontière suivante, en repartant juste après celle-ci
        it = std::find_if(it + 1, graphe->end(), estFrontiere);
    }
    return meilleureDistance != -1;
}

// Lance A* vers la frontière la plus proche.
// Si elle est inaccessible, essaie la suivante, et ainsi de suite.
// Les frontières sans rien à découvrir autour sont ignorées.
// Renvoie false s'il n'y a plus aucune frontière utile accessible : exploration terminée.
bool AllerFrontiereLaPlusProche(Robot &robot)
{
    Map *carte = robot.GetMap();
    std::vector<std::vector<bool>> essayee(carte->GetHeight(), std::vector<bool>(carte->GetWidth(), false));
    int fx, fy;

    while (ChercherFrontiereLaPlusProche(robot, essayee, fx, fy))
    {
        if (robot.GoTo(fx, fy))
            return true;       // chemin trouvé : il est dans le robot
        essayee[fy][fx] = true; // inaccessible : on passe à la suivante
    }
    return false;
}

// Nombre de cases inconnues restantes dans la carte du robot
int CompterInconnues(Robot &robot)
{
    Map *carte = robot.GetMap();
    int n = 0;
    for (int y = 0; y < carte->GetHeight(); y++)
        for (int x = 0; x < carte->GetWidth(); x++)
            if (carte->GetCaseType(x, y) == UNKNOWN_ID)
                n++;
    return n;
}

// Nombre de frontières qui valent encore le déplacement
// (celles qui ont encore quelque chose à découvrir autour)
int CompterFrontieresUtiles(Robot &robot)
{
    Map *carte = robot.GetMap();
    int n = 0;
    for (int y = 0; y < carte->GetHeight(); y++)
        for (int x = 0; x < carte->GetWidth(); x++)
        {
            bool estFrontiere = isFrontier(carte->GetCaseType(x, y));
            if (estFrontiere && FrontiereUtile(carte, x, y))
                n++;
        }
    return n;
}

// Ce qu'on suit pour chaque robot pendant l'exploration
struct Exploration
{
    int pasCheck = 0;     // pas choisis par Check()
    int pasAStar = 0;     // pas faits en suivant un chemin A*
    bool termine = false; // true quand il n'y a plus de frontière utile accessible
};

// Fait faire un pas au robot :
//   1. un chemin A* est en cours : on le continue
//   2. sinon, on demande à Check() une direction utile
//   3. si Check() ne trouve rien, A* calcule un chemin vers la frontière la plus proche
//   4. s'il n'y a plus de frontière utile accessible, l'exploration est terminée
void FairePas(Robot &robot, Map *map, Case &temp, uint8_t *robotStart, Exploration &etat)
{
    if (etat.termine)
        return;

    // 1. Choisir le prochain mouvement
    bool parAStar = robot.HasPath();
    uint8_t d = Robot::NO_DIRECTION;
    if (!parAStar)
    {
        d = robot.Check();
        if (d == Robot::NO_DIRECTION)
        {
            if (!AllerFrontiereLaPlusProche(robot))
            {
                etat.termine = true;
                return;
            }
            parAStar = true;
        }
    }

    // 2. Quitter la case actuelle (fonction du collègue)
    RemoveDisplayedRobot(map, robot, temp, robotStart);

    // 3. Se déplacer
    if (parAStar)
    {
        robot.FollowPath(); // fait le premier mouvement du chemin A*
        etat.pasAStar++;
    }
    else
    {
        robot.Move(d);
        etat.pasCheck++;
    }

    // 4. Regarder autour, puis s'afficher sur la nouvelle case (fonction du collègue)
    robot.AnalyzeEnvironment(map, robotStart);
    DisplayRobot(map, robot, temp, robotStart);
}

// Affiche le bilan d'un robot
void AfficherBilan(Robot &robot, Exploration &etat, const char *nom)
{
    std::cout << nom << " : " << etat.pasCheck + etat.pasAStar << " pas ("
              << etat.pasCheck << " avec Check, " << etat.pasAStar << " avec A*)";
    if (etat.termine)
        std::cout << ", exploration terminee";
    std::cout << std::endl;
    std::cout << "    victimes trouvees : " << (int)robot.GetVictimCount()
              << " | frontieres utiles restantes : " << CompterFrontieresUtiles(robot)
              << " | cases inconnues restantes : " << CompterInconnues(robot) << std::endl;
}

// ============================================================================

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

    // --- Exploration : Check() + A* pour les deux robots (ajout Erwan) ---
    const int MAX_TOURS = 2000; // sécurité contre une boucle infinie
    Exploration etat1, etat2;
    int tour = 0;

    while (tour < MAX_TOURS && !(etat1.termine && etat2.termine))
    {
        // Chaque robot fait un pas à son tour
        FairePas(robot1, map.get(), temp1, robot1Start, etat1);
        FairePas(robot2, map.get(), temp2, robot2Start, etat2);
        tour++;

        // Affichage (comme le collègue)
        gotoxy(0, 0);
        DisplayMapSVG(map.get());
        PrintEmptyLine(2);
        DisplayMapSVG(robot1.GetMap());
        PrintEmptyLine(2);
        DisplayMapSVG(robot2.GetMap());
        std::cout << "Tour " << tour << "      " << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(robotSpeed));
    }

    // Bilan
    ClearConsole();
    DisplayMapSVG(map.get());
    std::cout << std::endl;
    DisplayMapSVG(robot1.GetMap());
    std::cout << std::endl << std::endl;
    DisplayMapSVG(robot2.GetMap());

    std::cout << std::endl << "Fin apres " << tour << " tours";
    if (tour >= MAX_TOURS)
        std::cout << " (limite de " << MAX_TOURS << " tours atteinte)";
    std::cout << std::endl;
    AfficherBilan(robot1, etat1, "Robot 1");
    AfficherBilan(robot2, etat2, "Robot 2");


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
