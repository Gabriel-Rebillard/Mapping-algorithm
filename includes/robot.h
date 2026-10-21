#pragma once
#include "map.h"
#include "cfg.h"

class Robot
{
private:
    Map _Map;
    std::vector<uint8_t> _CurrentPath;

    uint8_t _Position[2];
    uint8_t _StartPoint[2];

    std::vector<uint16_t> _DetectionDistance;   // Distance parcourue entre le debut de l'exploration et la nieme victime
    uint8_t _VictimCount = 0;                   // Nombre de victimes trouvées
    std::vector<uint8_t*> _VictimPositions;     // Position des victimes
    uint16_t _MoveCount = 0;                    // Nombre total de mouvements effectues

    // Augmente la taille de la carte en fonction de la direction : ajoute une colonne ou une ligne en haut/bas/gauche/droite
    // Rempli la colonne/ligne ajoutée avec des cases de type UNKNOWN
    void ExtendMap(uint8_t direction);

    // Optimise les frontieres de la carte
    // Verifie la position des frontieres reelles et supprime les frontieres non necessaires
    // ex : ilot de frontieres
    void OptimizeFrontier();

    // Cherche le chemin le plus court pour atteindre la case destination
    // Rempli le buffer de mouvement _CurrentPath
    uint16_t FindPath(uint8_t *destination);
    uint16_t FindPath(uint8_t x, uint8_t y);

    // Cherche la frontiere la plus proche et renvoi sa position
    uint8_t* FindNearestFrontier();

    // Cherche la prochaine frontiere à explorer et renvoi sa position
    uint8_t* FindNextFrontier();

    // --- Ajout Erwan : exploration -------------------------------------------
    void    DirectionVector(uint8_t direction, int &dx, int &dy);
    uint8_t GetCell(int x, int y);
    int     CountUnknown(uint8_t direction);
    // -------------------------------------------------------------------------
    
    public:
    Robot();
    
    // Rempli les cases autour du robot si elles sont inconnues
    void FillMap(Map *map, uint8_t start_point_x, uint8_t start_point_y);

    // Effectue le premier mouvement du buffer de mouvement _CurrentPath, puis supprime celui-ci
    bool Move(uint8_t direction);

    // Appelle Move() jusqu'a ce que le buffer de mouvement soit vide
    bool FollowPath();

    /* Analyse l'environnement autour du robot :
        - Agrandi la carte si besoin
        - Remplit la carte
        - Optimise les frontieres
    */
    void AnalyzeEnvironment(Map *map, uint8_t start_point_x, uint8_t start_point_y);
    void AnalyzeEnvironment(Map *map, uint8_t *start_point);

    // Calcule la prochaine destination
    uint8_t *ComputeNextDestination();

    // --- Ajout Erwan : exploration -------------------------------------------
    static const uint8_t NO_DIRECTION = 0xFF;   // aucune direction utile
    uint8_t Check();                            // direction qui découvre le plus de cases
    // -------------------------------------------------------------------------

    // Getters
    // Renvoi un pointeur vers la carte du robot
    Map *GetMap();

    // Renvoi un pointeur vers la position du robot
    uint8_t *GetPosition();

    // Renvoi un pointeur vers le point de depart du robot
    uint8_t *GetStartPoint();
};
