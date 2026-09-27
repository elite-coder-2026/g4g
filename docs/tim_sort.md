#### Time Sort
TimeSort is a hybrid sorting algorithm that uses the ideas of Merge Sort and 
Insertion sort.

- used as the default sorting algorithn in python and java
- the key idea behind TimSort is to identify small sorted segemnts of the array,
called runs, and then merge them efficiently to form the fully sorted array

#### how it works
timesort works in three main steps by combining ideas from merge sort and 
insertion sort

- identify runs: it scans the array to find small segements that are already 
sorted, called runs. if a run is in descending order, it's revered to make
it ascending
- sort small runs: if a run is shorter than a fixed size (usualy 32), it is
sorted using insertion sort, which is fast for small or nearly sorted data
- merge runs: finally timesort merges these runs using rules that keep marging
-  balanced, and efficient similar merge sort, but optimized for real-world data

#### understand the time complexity
timsort's complexity comes form the combination of two phases -- sorting small
runs using insertion sort merging those runs using merge sort like process

- *sorting part* detecting the runs takes linear time. O(n). each run is sorted
using insertion sort, shich takes O(k^2) for a run of size k. since there are about
n/k such runs, the total cose of sorting all run becomes O(n*k) , wich simplifies to
O(n) because k is a small constant.

- *merging part* merging to runs of total length m take O(m) time. every element
in the array participates in each level of merging once, so each marge level cost
O(n). as runs double in size after every merge, the number of merge levels is 
approximately log(n/k). hence the total merging becomes O(n log(n/k)), which 
simplifies to O(n log n)

combining
