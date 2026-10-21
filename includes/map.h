#pragma once

#include "cfg.h"
#include "graph.h"

// Converting vertex to case
#define vToC(v) static_cast<Case *>(v)


// Case herits from vertex to be able to store it in a graph
class Case : public Vertex
{
private:
    uint8_t _VisitedCount = 0;  // Nombre de passages sur la cases

public:
    Case(uint8_t type = NaN);
    uint8_t GetVisitedCount();  // Retourne le nombre de fois que la case a été visite
};

class Map
{
private:
    Graph _Graph;
    uint8_t _Width;
    uint8_t _Height;

public:
    Map(uint8_t width, uint8_t height);         // Constructeur
    ~Map();

    const Graph &GetMap();                      // Retourne la carte sous forme de graphe

    void ExtendMap(uint8_t dir);                // Augmente la taille de la carte en fonction de la direction : ajoute une colonne ou une ligne en haut/bas/gauche/droite

    uint8_t GetWidth();                         // Retourne la largeur de la carte                                                                                       
    uint8_t GetHeight();                        // Retourne la hauteur de la carte                                                                                       

    std::unique_ptr<uint8_t[]> GetMap_Matrix(); // Retourne la carte sous forme de liste de type de longueur _Width * _Height


    // Interface avec les cases de la carte
    Case *GetCase(uint8_t x, uint8_t y);                  // Retourne un pointeur sur la case de coordonnées (x, y)
    Case *GetCase(uint8_t *position);                    

    uint8_t GetCasePositionX(Case *c);                    // Retourne la coordonnée x de la case sur la carte      
    uint8_t GetCasePositionY(Case *c);                    // Retourne la coordonnée y de la case sur la carte      
    uint8_t *GetCasePosition(Case *c);                    // Retourne les coordonnées de la case sur la carte      

    uint8_t GetCaseType(uint8_t *position);               // Retourne le type de la case                           
    uint8_t GetCaseType(uint8_t x, uint8_t y);           

    bool SetCaseType(uint8_t x, uint8_t y, uint8_t type); // Modifie le type de la case
    bool SetCaseType(uint8_t *position, uint8_t type);


    // Interface avec les liens/distances/passages reliant les cases aux autres cases
    bool AddNeighbour(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t weight);   // Ajoute un passage entre deux cases                     
    bool AddNeighbour(uint8_t *position1, uint8_t *position2, uint8_t weight);          

    bool RemoveNeighbour(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);                // Supprime un passage entre deux cases                   
    bool RemoveNeighbour(uint8_t *position1, uint8_t *position2);                       

    std::unique_ptr<std::list<Case *>> GetNeighbours(uint8_t x, uint8_t y);              // Renvoie la liste des cases adjacentes/reliées a la case
    std::unique_ptr<std::list<Case *>> GetNeighbours(uint8_t *position);                
                                                                                        
    bool ModifyDistance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t weight); // Modifie la distance entre deux cases                   
    bool ModifyDistance(uint8_t *position1, uint8_t *position2, uint8_t weight);        

    uint8_t GetDistance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);                 // Renvoie la distance entre deux cases
    uint8_t GetDistance(uint8_t *position1, uint8_t *position2);
};