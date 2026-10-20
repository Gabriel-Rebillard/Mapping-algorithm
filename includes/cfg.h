#define INFINITY 0xff
#define NaN (INFINITY-1)
#define UNKNOWN_ID NaN
#define WALL_ID 0
#define ROOM_ID 10
#define FRONTIER_MASK 0x80
#define VICTIM_ID 2
#define ROBOT_ID 4
#define ROBOT_BASE_ID 5
#define PORTAL_ID 7

#define isFrontier(ID) ID & FRONTIER_MASK == FRONTIER_MASK

#define WALL_SYMBOL "⬜"
#define ROOM_SYMBOL "🟩"
#define VICTIM_SYMBOL "🆘"
#define UNKNOWN_SYMBOL "⬛"
#define FRONTIER_SYMBOL "🟧"
#define ROBOT_SYMBOL "🟦"
#define ROBOT_BASE_SYMBOL "🟫"
#define PORTAL_SYMBOL "🔄"

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
#define GENERATOR_ROBOT_NUMBER 1
#define GENERATOR_PORTAL_NUMBER 1

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define TP 10