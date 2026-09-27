#### Bubble sort
this sort algorithm is the simplest sorting algorithnm that works by repeatedly 
swapping the adjacent elments if they are in the wrong order. this algorithm is
not efficient for alrge data sets as its average and worst case time complexity 
are quite high

- sorting the array using multiple passes. after the first pass,, the maximum goes
to the second last position and so on.
- in every pass, process only those that have already not moved to correct position.
after k passes, the largest k must have been moved to the last k positions.
- in a pass, we consider remaining elements and compare all adjacent and swap larger
elements is before a smaller element. if we keep doing this, we get the largest at its
correct position.

#### complexity analysis of bubble sort
- time complexity is O(n2)
- auxilary space O(1)

#### advantages of bubble sort
- bubble sort is easy to understand and implement
- it does not require any additional memory space
- it is a stable sorting algorithm, meanining that elements with the same key value
maintain their relative order in the sorted output.

#### disadvantages of bubble sort
- bubble sort has a time complexity of O(n^2) which makes it very slow for large
data sets
- it has almost no or limited real world applications. it is mostly used in
academics to teach different ways of sorting
