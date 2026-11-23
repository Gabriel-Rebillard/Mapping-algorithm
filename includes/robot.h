#pragma once
#include "map.h"
#include "cfg.h"
#include <iostream>
#include <vector>

// ============================================================================
// A* (ajout Erwan) : recherche du plus court chemin dans la carte du robot
// Base sur https://github.com/daancode/a-star (licence MIT, voir robot.cpp)
// ============================================================================
namespace AStar
{
    // Coordonnées en int (et non uint8_t) pour pouvoir être négatives sans boucler à 255
    struct Vec2i
    {
        int x, y;

        bool operator==(const Vec2i &other) const { return x == other.x && y == other.y; }
        friend Vec2i operator+(const Vec2i &a, const Vec2i &b) { return {a.x + b.x, a.y + b.y}; }
    };

    using uint = unsigned int;

    // Un noeud = une case explorée par A*
    struct Node
    {
        uint G;             // coût réel depuis le départ
        uint H;             // estimation du coût restant (heuristique)
        Vec2i coordinates;  // position de la case
        Node *parent;       // case d'où l'on vient par le meilleur chemin connu
        uint8_t move;       // direction prise depuis le parent pour arriver ici

        Node(Vec2i coordinates_, Node *parent_ = nullptr, uint8_t move_ = 0);
        uint getScore() const; // f = G + H
    };

    using NodeSet = std::vector<Node *>;

    class Generator
    {
    public:
        explicit Generator(Map *map_);

        // Cherche le plus court chemin de source_ à target_.
        // Si un chemin existe : remplit path_ avec les directions (UP, RIGHT, DOWN, LEFT) et renvoie true.
        // Sinon : vide path_ et renvoie false.
        bool findPath(Vec2i source_, Vec2i target_, std::vector<uint8_t> &path_);

    private:
        bool detectCollision(Vec2i coordinates_);                 // true si la case est infranchissable
        Node *findNodeOnList(NodeSet &nodes_, Vec2i coordinates_); // cherche une case dans une liste
        void releaseNodes(NodeSet &nodes_);                       // libère la mémoire d'une liste

        struct Step
        {
            uint8_t dir; // UP, RIGHT, DOWN ou LEFT
            Vec2i delta; // décalage correspondant
        };

        Map *map;
        std::vector<Step> steps;
    };

    // Distance de Manhattan : admissible pour 4 directions avec un coût de 1 par pas
    uint manhattan(Vec2i source_, Vec2i target_);
}

class Robot
{
private:
    uint8_t _RobotID;

    Map _Map;
    std::vector<uint8_t> _CurrentPath;

    uint8_t _Position[2];
    uint8_t _StartPoint[2];

    std::vector<uint16_t> _DetectionDistance;      // Distance parcourue entre le debut de l'exploration et la nieme victime
    uint8_t _VictimCount = 0;                      // Nombre de victimes trouvées
    std::vector<const uint8_t *> _VictimPositions; // Position des victimes
    uint16_t _MoveCount = 0;                       // Nombre total de mouvements effectues

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
    uint8_t *FindNearestFrontier();

    // Cherche la prochaine frontiere à explorer et renvoi sa position
    uint8_t *FindNextFrontier();

    // --- Ajout Erwan : exploration -------------------------------------------
    void    DirectionVector(uint8_t direction, int &dx, int &dy);
    uint8_t GetCell(int x, int y);
    int     CountUnknown(uint8_t direction);
    // -------------------------------------------------------------------------

public:
    Robot(uint8_t id);

    // Rempli les cases autour du robot si elles sont inconnues
    void FillMap(Map *map, uint8_t start_point_x, uint8_t start_point_y);

    // Effectue le premier mouvement du buffer de mouvement _CurrentPath, puis supprime celui-ci
    bool Move(uint8_t direction);

    // Appelle Move() jusqu'a ce que le buffer de mouvement soit vide
    bool FollowPath();

    // Renvoi le nombre de salles fermée
    int ClosedRoom(Map *map);

    /* Analyse l'environnement autour du robot :
        - Agrandi la carte si besoin
        - Remplit la carte
        - Optimise les frontieres
    */
    void AnalyzeEnvironment(Map *map, uint8_t start_point_x, uint8_t start_point_y);
    void AnalyzeEnvironment(Map *map, uint8_t *start_point);

    // Calcule la prochaine destination
    uint8_t *ComputeNextDestination();

    // Getters
    // Renvoi un pointeur vers la carte du robot
    Map *GetMap();

    // Renvoi un pointeur vers la position du robot
    uint8_t *GetPosition();

    // Renvoi un pointeur vers le point de depart du robot
    uint8_t *GetStartPoint();

    // Renvoi l'identifiant du robot
    uint8_t GetId();

    // Renvoi le nombre de victimes trouvées
    uint8_t GetVictimCount();

    // Renvoi un pointeur vers la position des victimes
    const std::vector<const uint8_t *> *GetVictimPositions();

    // --- Ajout Erwan : exploration -------------------------------------------
    static const uint8_t NO_DIRECTION = 0xFF;   // aucune direction utile
    uint8_t Check();                            // direction qui découvre le plus de cases

    // A* : calcule le chemin vers (x, y) dans _CurrentPath ; false si inaccessible
    static const uint16_t NO_PATH = 0xFFFF;
    bool GoTo(uint8_t x, uint8_t y);
    bool HasPath();                             // true s'il reste des mouvements dans _CurrentPath
    // -------------------------------------------------------------------------
};
