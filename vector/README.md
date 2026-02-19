# Algorithms on Vectors Collection

Some problems about algorithms on vectors are proposed in this collection.

## Proposed Problems

You should attempt to solve the problems in the order they are presented, since they build on top of each other.

1. ***Fill Vector***: 
    * Implement an algorithm that fills a vector using multiple *worker*s: each _worker_ will receive a segment of the vector that it's going to populate randomly with values between `0` and an arbitrary exclusive upper bound.
2. ***Find Zero***:
    * Implement an algorithm that looks for `0` in a vector that had been randomly populated. The _main thread_ is going to spawn multiple *worker*s, each of them will receive a segment of the vector into which look for, and the _main thread_ will cancel the alive *worker*s as soon as `0` has been found. Use a shared global variable which will be declared as volatile to flag if `0` had been found.
    * Discuss why it is sufficient to use a volatile variable instead of an atomic variable.
    * A _validation algorithm_ is not required for this exercise because it is very likely that `0` is going to be generated at least one time.
3. ***Bubble Sort***:
    * Implement an algorithm that bubble sorts the segments of a vector which had been randomly populated. The _main thread_ will spawn multiple *worker*s: each _worker_ will receive a segment of the vector to bubble sort. The _main thread_ will wait for all the *worker*s to finish.
4. ***Bubble Sort and Merge***:
    * By exploiting the solution for the previous exercise, implement an algorithm that will merge the bubble sorted segments of the vector on the _main thread_.

When you think that you have reached a good solution, test it out to check that there are neither bugs nor memory-safety issues.

Then, comment and document the code (remember that good documentations and comments are not verbose ones).

## Resources

You can use this website to have access to explanations of some elementary algorithms that you may need to know to solve the exercises.

* [GeeksforGeeks' Bubble Sort Algorithm Walkthrough](https://www.geeksforgeeks.org/dsa/bubble-sort-algorithm/): it presents the bubble sort algorithm and provides an implementation in various languages.
* [GeeksforGeeks' Merge Algorithm Walkthrough](https://www.geeksforgeeks.org/dsa/merge-two-sorted-arrays/): it presents the merge algorithm between two sorted arrays and provides an implementation in various languages.
