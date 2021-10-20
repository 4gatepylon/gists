#ifndef NULL
#define NULL 0
#endif

struct LocationTuple {
    int row;
    int col;
    int dist;
};

typedef struct LocationTuple Tuple;

// In the future we will want to use the preprocessor to have type flexibility here (or just void *)
struct IntegerArrayList {
    Tuple* array;
    int cap;
    int len;
};
typedef struct IntegerArrayList ArrayList;

int Pop(ArrayList* list, Tuple* item) {
    if (list->len < 1) {
        return 1;
    }
    *item = (list->array)[-- list->len];
    return 0;
}

// Either set the last element of the list and increment the length, or double the length
// of the array, copy it, and then set the last element.
int Append(ArrayList* list, Tuple item) {
    if (list->len < list->cap) {
        (list->array)[list->len ++] = item;
        return 0;
    }
    int cap = list->cap;
    int new_cap = cap << 1;
    Tuple* array = list->array;
    Tuple* new_array = malloc(new_cap * sizeof(Tuple));
    
    for (int i = 0; i < cap; i++) {
        new_array[i] = array[i];
    }
    free((void *) list->array);

    new_array[list->len ++] = item;
    list->array = new_array;
    list->cap = new_cap;
    return 0;
}

int Free(ArrayList* list) {
    free((void *)(list->array));
    free((void *)list);
    return 0;
}

int Set(ArrayList* list, int idx, Tuple val) {
    if (idx >= list->len) {
        return 1;
    }
    (list->array)[idx] = val;
    return 0;
}

int Get(ArrayList* list, int idx, Tuple* val) {
    if (idx >= list->len) {
        return 1;
    }
    *val = (list->array)[idx];
    return 0;
}

// NOTE: all heap operations are for a min heap; for a max heap use negative numbers;
// for another quantity use a hashtable of some kind to map the desired quantity to an int
int HeapPush(ArrayList* heap, Tuple val) {
    printf("heap push\n");
    // Append to the end, then swap up until it is no longer smaller
    // Remember that append already increments "len"
    Append(heap, val);
    printf("HEAP: [\t");
    for (int i = 0; i < heap->len; i++) {
        Tuple printer = heap->array[i];
        printf("%d %d %d,\t", printer.row, printer.col, printer.dist);
    }
    printf("]\n");

    Tuple* array = heap->array;

    int idx = heap->len - 1;
    int parent_idx = (idx - 1) >> 1;
    Tuple* parent = array + parent_idx * sizeof(Tuple);
    while (idx > 0 && parent->dist < val.dist) {
        array[parent_idx] = val;
        array[idx] = *parent;
        idx = parent_idx;

        parent_idx = (idx - 1) >> 1;
        parent = array + parent_idx * sizeof(Tuple);
    }
    return 0;
}

int HeapPop(ArrayList* heap, Tuple* val) {
    printf("heap pop\n");
    if (heap->len < 1) {
        // If it's empty: nothing to pop
        return 1;
    }
    *val = (heap->array)[0];
    Tuple* array = heap->array;
    
    int idx = 0;
    int left_child_idx = 1;
    int right_child_idx = 2;
    Tuple right_child;
    Tuple left_child;
    // printf("entering first loop\n");
    while (idx < heap->len - 1) {
        left_child = array[left_child_idx];
        if (right_child_idx < heap->len) {
            right_child = array[right_child_idx];
        }
        
        if (right_child_idx < heap->len && (left_child.dist > right_child.dist)) {
            array[idx] = right_child;
            idx = right_child_idx;
        } else {
            array[idx] = left_child;
            idx = left_child_idx;
        }

        right_child_idx = (idx + 1) << 1;
        left_child_idx = left_child_idx - 1;
    }
    // printf("done with first loop\n");
    // If we luckily removed the last element of the array
    // there is no need to swap and propagate up
    if (idx != heap->len - 1) {
        // Else swap the hole with the last element of the array and propagate up
        // just how is done in heap push.
        Tuple child = array[heap->len - 1];
        array[idx] = child;

        int parent_idx = (idx - 1) >> 1;
        Tuple parent = array[parent_idx];
        while (idx > 0 && parent.dist > child.dist) {
            array[idx] = parent;
            array[parent_idx] = child;
            idx = parent_idx;

            parent_idx = (idx - 1) >> 1;
            parent = array[parent_idx];
        }
    }
    heap->len = heap->len - 1;
    return 0;
}

int HeapPeek(ArrayList* heap, Tuple* val) {
    if (heap->len <= 0) {
        return 1;
    }
    *val = (heap->array)[0];
    return 0;
}

// THIS IS FROM LEETCODE
int minPathSum(int** grid, int gridSize, int* gridColSize){
    ArrayList queue = {
        .array = (Tuple*) malloc((gridSize * gridColSize[0]) * sizeof(Tuple)),
        .cap = gridSize * gridColSize[0],
        .len = 0,
    };
    int min = 0;
    Tuple current = {
        .row = 0,
        .col = 0,
        .dist = grid[0][0],
    };
    HeapPush(&queue, current);
    while (queue.len > 0) {
        printf("heap len starts at %d\n", queue.len);
        HeapPop(&queue, &current);
        printf("current row col dist is %d %d %d\t heap len %d\n", current.row, current.col, current.dist, queue.len);
        if (current.row == gridSize - 1 && current.col == gridColSize[0] - 1) {
            return current.dist;
        }
        if (current.row < gridSize - 1) {
            HeapPush(&queue, (Tuple) {
                .dist = current.dist + grid[current.row + 1][current.col],
                .row = current.row + 1,
                .col = current.col,
            });
        }
        if (current.col < gridColSize[0] - 1) {
            HeapPush(&queue, (Tuple) {
                .dist = current.dist + grid[current.row][current.col + 1],
                .row = current.row,
                .col = current.col + 1,
            });
        }
    }
    
    return -1;
}