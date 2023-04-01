#include <vector>
#include <iostream>

// #define DEBUG

using namespace std;

static inline void print_xys(int* xs, int* ys) {
    for (int i = 0; i < 7; i++) {
        cout << "xy[" << i << "] = " << xs[i] << ", " << ys[i] << "\n";
    }
    cout << "\n";
}

static inline void update_xys(int* xs, int* ys, vector<int>& moves, int move) {
    assert(move < moves.size());
    for (int i = 0; i < 6; i++) {
        xs[i] = xs[i+1];
        ys[i] = ys[i+1];
    }
    xs[6] = (move % 2 == 0) ? xs[5] : ((move % 4 == 1) ? xs[5] - moves[move] : xs[5] + moves[move]);
    ys[6] = (move % 2 == 1) ? ys[5] : ((move % 4 == 0) ? ys[5] + moves[move] : ys[5] - moves[move]);
}

static inline bool is_self_crossing(vector<int>& moves) {
    // Store the past 5 x's and y's including the current one at
    // xs[4], ys[4] (and the transitions from x[i], y[i] to x[i+1], y[i+1])
    // corresponds to move[j] where in the beginning j = 0, but then it will
    // be shifted by one each time
    int xs[7];
    int ys[7];
    // Before any move
    xs[0] = 0;
    ys[0] = 0;

    if (moves.size() <= 3) return false;
    assert(moves.size() >= 4);
    // Move 1
    xs[1] = xs[0];
    ys[1] = ys[0] + moves[0];
    // Move 2
    xs[2] = xs[1] - moves[1];
    ys[2] = ys[1];
    // Move 3
    xs[3] = xs[2];
    ys[3] = ys[2] - moves[2];
    // Move 4
    xs[4] = xs[3] + moves[3];
    ys[4] = ys[3];
    
    assert(xs[0] == xs[1] && xs[1] > xs[2] && xs[2] == xs[3] && xs[3] < xs[4]);
    // assert(ys[0] < ys[1] && ys[1] == ys[2] && ys[2] > ys[3] && ys[3] == ys[4]);

    // In four moves you can only end by touching crossing the first line
    bool crossed_1st_line = 0 <= ys[4] && 0 <= xs[4];
    if (moves.size() == 4 || crossed_1st_line) return crossed_1st_line;

    assert(moves.size() >= 5);
    xs[5] = xs[4];
    ys[5] = ys[4] + moves[4];

    // In five moves you can only end by touching teh 2nd line from the bottom
    // or crosssing the second line
    bool touched_1st_line_tip_or_crossed_2nd = (xs[5] == 0 && ys[5] >= 0) || (xs[5] < 0 && ys[5] > ys[1]);
    if (moves.size() == 5 || touched_1st_line_tip_or_crossed_2nd) return touched_1st_line_tip_or_crossed_2nd;
    
    assert(moves.size() >= 6);
    xs[6] = xs[5] - moves[5];
    ys[6] = ys[5];
    // In six moves you can only end by touching the first line or the third
    bool touched_1st_line_or_crossed_3rd = (xs[0] <= xs[5] && xs[6] <= xs[0] && ys[0] <= ys[6] && ys[6] <= ys[1]) || (xs[6] <= xs[2] && ys[6] <= ys[2]);
    if (moves.size() == 6 || touched_1st_line_or_crossed_3rd) return touched_1st_line_or_crossed_3rd;
    // At this point, depending on whether we are above or below zero, we have to change our update
    // based on whether we coming into the rectangle we just drew or outside/around it (hitting a wall)
    // We can always tell those last two horizontal segments (image we went down) and we know also if we are
    // outside or inside from our relative position so we can tell if we are crossing one of them. Because of that
    // quirk, we start to use the last part of the xys
    assert(moves.size() >= 7);
    for (int move = 6; move < moves.size(); move++) {
        update_xys(xs, ys, moves, move);
        #ifdef DEBUG
        print_xys(xs, ys);
        #endif
        // as is our last direction of movement, while bs is perpendicular
        int *as = xs, *bs = ys;                  // Default Left/Right Move
        if (move % 2 == 0) { as = ys; bs = xs; } // Up/Down Move
        int front_start = min(bs[0], bs[1]), front_end = max(bs[0], bs[1]);
        int back_start = min(bs[2], bs[3]), back_end = max(bs[2], bs[3]);
        bool crossed_front = (as[5] <= as[0] && as[6] >= as[0]) || (as[6] >= as[0] && as[6] <= as[0]);
        bool crossed_back = (as[5] <= as[2] && as[6] >= as[2]) || (as[5] >= as[2] && as[6] <= as[2]);

        // If you crossed the front wall or back wall then we crossed (front wall only possible if you're outside)
        if (front_start <= bs[6] && bs[6] <= front_end && crossed_front) return true;
        if (back_start <= bs[6] && bs[6] <= back_end && crossed_back) return true;
    }
    
    return false;
}

// Compile and run with
// rm -rf && clang++ self_crossing.cc -std=c++11 -stdlib=libc++ && ./a.out
int main() {
    // Should yield 1 0 1
    auto v1 = vector<int>{2,1,1,2};
    auto v2 = vector<int>{1,2,3,4};
    auto v3 =vector<int>{1,1,1,1};
    auto v4 = vector<int>{3,3,3,2,1,1};
    auto v5 = vector<int>{1,1,2,2,3,3,4,4,10,4,4,3,3,2,2,1,1};
    auto v6 = vector<int>{1,1,2,2,3,1,1};
    cout << is_self_crossing(v1) << "\n";
    cout << is_self_crossing(v2) << "\n";
    cout << is_self_crossing(v3) << "\n";
    cout << is_self_crossing(v4) << "\n";
    cout << is_self_crossing(v5) << "\n";
    cout << is_self_crossing(v6) << "\n";
}