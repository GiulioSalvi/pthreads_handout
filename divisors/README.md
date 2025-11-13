# Primality Check and Divisibility Algorithms Collection

Some problems about primality check and divisibility algorithms are proposed in this collection.

## Proposed Problems

You should attempt to solve the problems in the order they are presented, since they build on top of each other.

1. ***Basic Primality Check***: 
    * Implement a basic primality check that stops the search as soon as one _worker_ finds a divisor and print that divisor. Use a shared global variable which will be declared as volatile to flag if a divisor had been found.
    * Discuss why it is sufficient to use a volatile variable instead of an atomic variable.
    * The number 815774583313 is not prime and is the product of two prime numbers. Which are they?
2. ***Primality Check***:
    * Improve the code from the previous exercise: you are not going anymore to use a shared volatile global variable; instead, use a condition variable to signal to the _main thread_ that a divisor had been found, then the _main thread_ will proceed to cancel the alive *worker*s.
    * Discuss why you may need to use some techniques that grant you thread-safety.
    * The number 815774583313 is not prime and is the product of two prime numbers. Which are they?
3. ***Divisors and Primality Check***:
    * Implement a primality check that also lists all the divisors of a number. You shouldn't need and use any global variable.
    * The number 1113231719 is not prime. Check that the list of divisors of this number provided by your solution is complete and correct.

When you think that you have reached a good solution, test it out to check that there are neither bugs nor memory-safety issues.

Then, comment and document the code (remember that good documentations and comments are not verbose ones).

## Resources

You can use these websites to have access to list of prime numbers or to generate a prime number with `n` digits.

* [Prime Curios](https://t5k.org/curios/index.php): it lists prime numbers, and some composite numbers too, by their number of digits.
* [BIG Primes](https://bigprimes.org): it generates `m` prime numbers with `n` digits each.
