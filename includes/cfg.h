#define Nan 9
#define UNKNOWN_ID Nan
#define WALL_ID 0
#define ROOM_ID 4
#define FRONTIER_ID 1
#define VICTIM_ID 2
#define ROBOT_ID 8

#define WALL_SYMBOL "⬜"
#define ROOM_SYMBOL "🟩"
#define VICTIM_SYMBOL "🟥"
#define UNKNOWN_SYMBOL "⬛"
#define FRONTIER_SYMBOL "🟨"
#define ROBOT_SYMBOL "🟦"

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#define INFINITY 0xffff

#define GENERATOR_MAX_EDGE_WEIGHT 10
#define GENERATOR_MAX_ADJACENT_WALLS 1