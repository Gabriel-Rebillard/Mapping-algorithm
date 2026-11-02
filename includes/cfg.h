#define INFINITY 0xff
#define NaN (INFINITY-1)

// Definition des identifiant des types de case
#define UNKNOWN_ID 127
#define WALL_ID 0
#define ROOM_ID 10
#define VICTIM_ID 2
#define ROBOT_ID 4
#define ROBOT_BASE_ID 5
#define PORTAL_ID 7

// Masque permettant de savoir si la case est une frontiere en mettant le premier bit de son type à '1'
#define FRONTIER_MASK 0b10000000 // 0x80 ; 128

// Macro permettant de savoir si la case est une frontiere (renvoie true si c'est le cas)
#define isFrontier(ID) (ID & FRONTIER_MASK) == FRONTIER_MASK
// Macro permettant de retirer le caractere de frontiere à une case
#define removeFrontier(ID) (ID & ~FRONTIER_MASK)
// Macro permettant de transformer une case en frontiere
#define addFrontier(ID) (ID | FRONTIER_MASK)

// Definition des symboles des cases
#define WALL_SYMBOL "⬜"
#define ROOM_SYMBOL "🟩"
#define VICTIM_SYMBOL "🆘"
#define UNKNOWN_SYMBOL "⬛"
#define FRONTIER_SYMBOL "🟧"
#define ROBOT_SYMBOL "🟦"
#define ROBOT_BASE_SYMBOL "🟫"
#define PORTAL_SYMBOL "🔄"
#define EMPTY_LINE "                                                            "

#define WALL_COLOR "\033[48;2;224;224;224m"    // "rgb(224 224 224)"
#define ROOM_COLOR "\033[48;2;85;255;99m"      // "rgb(85 255 99)"  
#define VICTIM_COLOR "\033[48;2;226;40;40m"    // "rgb(226 40 40)"  
#define UNKNOWN_COLOR "\033[48;2;32;32;32m"    // "#202020"         
#define FRONTIER_COLOR "\033[48;2;255;170;0m"  // "#FFAA00"         
#define ROBOT_COLOR "\033[48;2;43;43;255m"     // "#2B2BFF"         
#define ROBOT_BASE_COLOR "\033[48;2;153;56;0m" // "#993800"         
#define PORTAL_COLOR "\033[48;2;187;0;156m"    // "#BB009C"


// Definition des types entier non signé sur 8 et 16 bits
#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif


// Generation Rules
#define GENERATOR_MAX_EDGE_WEIGHT 10

#define GENERATOR_MAX_ADJACENT_WALLS 2
#define GENERATOR_WALL_PROBABILITY 0.4
#define GENERATOR_WALL_PROBABILITY_EVOLUTION_RULE(w, h) 1 + 100 / (w*h)
#define GENERATOR_PILLAR_PROBABILITY 0.05

#define GENERATOR_VICTIM_NUMBER 5
#define GENERATOR_ROBOT_NUMBER 2
#define GENERATOR_PORTAL_NUMBER 0

// Definition des directions
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define TP 10