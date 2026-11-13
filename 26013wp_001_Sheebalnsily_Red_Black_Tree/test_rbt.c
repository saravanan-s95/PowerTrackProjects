#include "rbt.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    RBTree t = {0};
    assert(rbt_init(&t));
    int present[301] = {0};
    srand(7);
    for (int i = 0; i < 20000; ++i) {
        int key = (rand() % 301) - 150;
        if (rand() % 2) {
            bool expected = !present[key + 150];
            bool got = rbt_insert(&t, key);
            assert(got == expected);
            present[key + 150] = 1;
        } else {
            bool expected = present[key + 150] != 0;
            bool got = rbt_delete(&t, key);
            assert(got == expected);
            present[key + 150] = 0;
        }
        assert(rbt_validate(&t));
    }
    for (int i = -150; i <= 150; ++i) {
        assert(rbt_search(&t, i) == (present[i + 150] != 0));
    }
    while (rbt_minimum(&t, &(int){0})) {
        int key;
        assert(rbt_delete_minimum(&t, &key));
        assert(rbt_validate(&t));
        present[key + 150] = 0;
    }
    assert(!rbt_maximum(&t, &(int){0}));
    rbt_destroy(&t);
    puts("Randomized operations and invariant checks passed.");
    return 0;
}
