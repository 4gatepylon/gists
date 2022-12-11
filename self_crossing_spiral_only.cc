#include <vector>

using namespace std;
// much harder if you are trying to do general self-crossing

// Cases
// Cross in 4 moves - pretty obvious how this happens (if you go too far in)
// Cross in 5 moves - reach an axis perpendicular to where you reached before,
//   then reach it or go further (or in the case of )
// Cross in 6 moves
bool crosses(vector<int>& moves) {
    if (moves.size() <= 3) return false;

    // This is a cache of the last boundaries that rotates with the POV
    // of the robot (lmao some karel shit)
    int bound_front = 0;
    int bound_left = moves[0];
    int bound_back = moves[1]; // This is what we immediately are just coming from
    int bound_right = moves[2]; // This is our current last taken step
    for (int i = 3; i < moves.size(); i++) {
        // We would be hitting moves[i-3]
        if (
            // Facing to the right or donw
            bound_back < bound_front && bound_back + moves[i] > bound_front ||
            // Facing to the left or up
            bound_back > bound_front && bound_back - moves[i] < bound_front
            ) return true;
        // else we rotate to the left so the caches shift accordingly
        bound_front = bound_left;
        bound_left = bound_back;
        bound_back = bound_right;
        bound_right = moves[i];
    }
    return false;
}