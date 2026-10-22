# Mapping-algorithm
A project of mapping algorithm for a simulated rescue robot


Random map generation, supporting random wall placements, bitmap pattern, random victim/robot base generation

#### To create a bitmap pattern :
        Open the /data/bitMaps.txt file

        start the pattern with '*'
        then write width and lenght of the bitmap, each on their line
        then write the bitmap according to the size specified, 1 for a wall, 0 for an accessible space

        example :
        "*
        4
        4
        0000
        0110
        0010
        0000
        "

**/!\ WRITE AN EMPTY LINE AT THE END OF THE FILE**

NB : surround the bitmap with 0 to not interfere with wall generation

#### To run the programm :
Command : ./main \<width> \<height> [seed]

seed : number used for pseudo-random generation : same seed = same generation
