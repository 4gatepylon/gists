#include <bits/stdc++.h>
class MedianFinder {
private:
    // https://stackoverflow.com/questions/57271271/is-there-a-maxheap-in-the-c-standard-library
    int num_elements;
    // Max heap
    priority_queue <int> below;
    // Min heap
    priority_queue<int, vector<int>, greater<int>> above;
    int median;
public:
    MedianFinder() {
        num_elements = 0;

        // By convention we can make median -1 when we don't
        // have any elements.
        median = -1;
    }
    
    void addNum(int num) {
        cout << "ADDING " << num << "\n";
        if (num_elements == 0) {
            cout << "median starts at " << num << "\n";
            median = num;
        } else if (num_elements % 2 == 0) {
            // If we have an even number of elements then we simply insert it as a median
            // or on the right side, popping the new median.
            if (num >= below.top()) {
                above.push(num);
                median = above.top();
                above.pop();
            } else {
                below.push(num);
                median = below.top();
                below.pop();
            }
            cout << "median even to odd is " << median << "\n";
        } else {
            // If we have an odd number of elements we insert it to the correct side of
            // the current median and then push the median onto the other side and
            // update the median variable to -1.
            if (num >= median) {
                above.push(num);
                below.push(median);
            } else {
                below.push(num);
                above.push(median);
            }
            cout << "median odd to even is avg of " << above.top() << " " << below.top() << "\n";
            median = -1;
        }
        num_elements ++;
    }
    
    double findMedian() {
        if (num_elements % 2 == 0) {
            return (double)(above.top() + below.top()) / 2.0;
        } else {
            return (double)median;
        }
    }
};

/**
 * Your MedianFinder object will be instantiated and called as such:
 * MedianFinder* obj = new MedianFinder();
 * obj->addNum(num);
 * double param_2 = obj->findMedian();
 */