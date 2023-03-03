#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;
// Compile with
/*
g++ A.cc && cat A.txt | ./a.out
*/

int main() {
    // Load the number of tests from the input stream
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    // For each test find the size of the matrix, then find the most beautiful matrix of that
    // size of a permutation of the numbers 1 to n^2
    for (int i = 0; i < num_tests; i++) {
        long first_cost = -1; // a
        cin >> first_cost;
        assert(first_cost >= 1);

        long second_cost = -1; // b
        cin >> second_cost;
        assert(second_cost >= 1);

        long want_to_buy_qt = -1; // n
        cin >> want_to_buy_qt;
        assert(want_to_buy_qt >= 1);
        
        long get_one_free_qt = -1; // m
        cin >> get_one_free_qt;
        assert(get_one_free_qt >= 1);

        // cout << "first_cost: " << first_cost << endl;
        // cout << "second_cost: " << second_cost << endl;
        // cout << "want_to_buy_qt: " << want_to_buy_qt << endl;
        // cout << "get_one_free_qt: " << get_one_free_qt << endl;

        long coins = -1;
        if (get_one_free_qt < want_to_buy_qt && first_cost * get_one_free_qt < second_cost * (get_one_free_qt + 1)) {
            // Buy as much as possible from the first, then possibly from the second
            long bulk_purchase_with_first_qt = want_to_buy_qt / (get_one_free_qt + 1);
            long small_purchase_with_cheapest_qt = want_to_buy_qt % (get_one_free_qt + 1);
            assert(small_purchase_with_cheapest_qt + bulk_purchase_with_first_qt * (get_one_free_qt + 1) == want_to_buy_qt);

            long bulk_cost_with_first = get_one_free_qt * first_cost;
            long small_cost_with_cheapest = min(first_cost, second_cost);

            // cout << "buying " << bulk_purchase_with_first_qt << " bulks from first of size " << (get_one_free_qt + 1) << " for " << bulk_cost_with_first << endl;
            // cout << "buying " << small_purchase_with_cheapest_qt << " singletons from cheapest for " << small_cost_with_cheapest << endl;

            coins = bulk_cost_with_first * bulk_purchase_with_first_qt + small_cost_with_cheapest * small_purchase_with_cheapest_qt;
        } else {
            // Buy all from the cheaper option
            coins = min(first_cost, second_cost) * want_to_buy_qt;

            // cout << "buying all from cheapest for " << coins << endl;
        }
        assert(coins != -1);
        assert(coins <= want_to_buy_qt * min(first_cost, second_cost));
        cout << coins << endl;

    }
    return 0;
}