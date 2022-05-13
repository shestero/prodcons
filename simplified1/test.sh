set -o verbose

./prodcons 1 2 3 1 -2 3 0 0 0
./prodcons  -1 2 3 1 -2 -3 0 0 0
./prodcons  -1 2 3 1 -2 -3d 0 0 0
./prodcons -1 2 3 1 -2 -3 0 0 0
./prodcons -1 2 3 1 -2 -3d 0 0 0
./prodcons 0 0 0 
./prodcons  no input
./prodcons  1 2 3 1 -2 3 0 0 0
