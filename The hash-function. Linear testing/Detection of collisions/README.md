# Detection of collisions

This code is designed to test written hash functions for collision 
with double hashing. As showed the testing, this code hasn't a high 
level of collisions. 

Graph of the collisions: Result.xlsx

ABOUT HASHING
----------

The method of double hashing: linear sampling

Key format: nnSSnn, where n is number, S is symbol.

The minimum value of the hash function takes at the minimum value of 
the summands, that is, if a string key contains characters with minimal 
code. It will be the symbol 0 with ID 48 and the "A" character code 65. 
Then the minimum value the resulting hash function will be 322. The 
maximum value of the hash function, which it takes when the maximum value 
of the summands, that is, if a string key contains symbols with a maximum 
code (including subtraction 322) will be 86, and end segments of the hash 
table will never be used. Therefore, it is necessary to cause the hash 
function to the form, when the maximum value would give the maximum 
segment number of the table, equal to 2000, i.e., multiply it by the 
number 23. Now the hash function takes values in the range from 0 up to 
1996's allowable numbers of segments in the hash table.


Linear testing is reduced to the successive iterating a table with some fixed increments

* a = h(key)

It is worth noting that h(key) = (hash_one + hash_two*i) mod TAB_SIZE.

* hash_one int(const int key) { return (key % TAB_SIZE); }

* hash_two int(const int key) { return ((key % (TAB_SIZE - 1)) + 1); }

The key calculation is as follows: 
the calculation of the sum of character codes of the key code with 
each character of the key is multiplied by 3 to the extent the index 
of this code in the key (numbering is from zero).