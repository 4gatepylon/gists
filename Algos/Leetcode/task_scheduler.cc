class Solution {
public:
    // I think it is best to always select something different, but we may have to pick something
    // different in a smart way... This is just a simulator, I'm guessing it's better to pick something
    // that is frequent since it gives us a more even spread later without costing anything right now
    // (when it is possible) but I'm just not sure how to prove it mathematically... There is probably
    // also a closed-form (ish) solution here...
    int leastInterval(vector<char>& tasks, int n) {
        // Store the number of times each task appears
        # define NUM_TYPES 26
        int freq[NUM_TYPES] = {0};
        int avail_in[NUM_TYPES] = {0};
        int num_tasks = 0;
        for (int i = 0; i < tasks.size(); i++) {
            freq[tasks[i] - 'A'] ++;
            num_tasks ++;
        }
        int time = 0;
        while (num_tasks > 0) {
            // Find the most frequent available task and execute it
            // while also updating the availability of others for next turn
            int maxf = -1;
            for (int i = 0; i < NUM_TYPES; i++) { 
                if (avail_in[i] == 0 && freq[i] > 0 && (maxf == -1 || freq[maxf] < freq[i])) maxf = i; 
                if (avail_in[i] > 0) avail_in[i] --;
            }
            // Execute the task if possible
            if (maxf != -1) {
                cout << "Execute " << (char)('A' + maxf) << "\n";
                freq[maxf] --;
                avail_in[maxf] = n;
                num_tasks --;
            }
            time ++;
        }
        return time;
    }
};