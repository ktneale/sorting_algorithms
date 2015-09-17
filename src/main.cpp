/*
 * Written by Kevin Neale (C) 2015.
 *
 * A collection of sorting algorithms implemented in C++.
 * Feel free to use these, but i'm sure that there are probably much better implementatons
 * out there...
 *
 * Notes
 * -----
 * The various .dat files contain lists of unsorted integers.
 * - integers.dat - Example list from a text book comparing the Bubble and Shuttle sort.
 * - integers_qs.dat - Example list from wikipedia used to check the Quick Sort implementation.
 * - integers_10000.dat - A list of 10000 randomly sorted integers.
 * - integers_worst_case_10000.dat - A list of 10000 integers in descending order. 
 *   Useful for testing worst case performance.
 *
 * If the algorithm is 'stable', it perserves the relative order of equal elements
 * e.g. 8,1,3,5,5,7,2.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sys/time.h>

using namespace std;

//Useful utility functions.
long timevaldiff(struct timeval *starttime, struct timeval *finishtime);
void write_list(const char *output_file, int count);

//Vector utility functions.
void dump_vector(vector < int >&v, const char *output_file);
void print_vector(vector < int >&v);


//Algorithms
/*
 * The Bubble Sort is a simple sorting algorithm.
 * It's name is due to the fact that the smallest elements rise or
 * 'bubble' to the top of the list on each pass.
 * The largest element in the list fall to the bottom on each pass.
 *
 * It's not really practical as it is too slow. 
 * An inherent problem is that a complete pass is needed to ensure that 
 * sorting has completed.
 *
 * Class: Comparison sort/Exchange sort
 * Alternative names: Sinking/Sifting sort
 * Best case: O(n)
 * Worst case: O(n^2)
 * Average case: O(n^2)
 * Stable: true
 */
void bubble_sort(vector < int >&v)
{
    bool debug = true;

    //Statistics for the algorithm.
    int comparisons = 0, swaps = 0, swaps_total = 0, comparisons_total =
        0, pass = 1;

    int tmp = 0;

    vector < int >::iterator e1 = v.begin();    //Element 1
    vector < int >::iterator e2 = e1 + 1;       //Element 2

    //After each pass, the largest element should be at the bottom.
    //Therefore it can be ignored on the subsequent pass.
    vector < int >::iterator end = v.end();

    if (v.size() > 10) {
        cout <<
            "List is greater than 10 elements. Debug is switched off!\n";
        debug = false;
    }

    while (1) {
        while (e2 < end) {
            comparisons++;
            if (*e2 < *e1) {
                tmp = *e1;
                *e1 = *e2;
                *e2 = tmp;
                swaps++;
            }

            e1++;
            e2++;
        }

        e1 = v.begin();
        e2 = e1 + 1;

        if (debug) {
            cout << "\nPass: " << pass << endl;
            cout << "Comparisons: " << comparisons << endl;
            cout << "Swaps: " << swaps << endl;
            print_vector(v);
        }
        //Preserve the running totals.
        swaps_total += swaps;
        comparisons_total += comparisons;

        //This condition indicates that the sort is complete.
        if (swaps == 0)
            break;

        //Prepare for the next pass.
        comparisons = 0;
        swaps = 0;
        pass++;

        end--;                  //Ignore the last element in the list.
    }

    cout << "-------------------------------\n";
    cout << "Total Comparisons: " << comparisons_total << endl;
    cout << "Total Swaps: " << swaps_total << endl;
    cout << "-------------------------------\n";

    return;
}

/*
 * The Shuttle Sort is another simple sorting algorithm.
 * Essentially, it's a bidirectional bubble sort.
 * Elements can be swapped up and down the list or 'shuttled' in one pass.
 * Hence the name 'Shuttle Sort'.
 *
 * It's not really practical as again it's too slow. 
 * Requires fewer comparisons than the bubble sort, but ultimately the performance 
 * improvement is marginal.
 *
 * Class: Exchange sort, Comparison sort
 * Alternative names: bidirectional bubble sort, cocktail shaker sort, shaker sort, 
 *                    ripple sort, shuffle sort.
 *
 * Best case: O(n)
 * Worst case: O(n^2)
 * Average case: O(n^2)
 * Stable: true
 */
void shuttle_sort(vector < int >&v)
{
    bool debug = true;

    //Statistics for the algorithm.
    int swaps = 0, comparisons = 0, swaps_total = 0, comparisons_total =
        0, pass = 1, n = v.size();

    int tmp = 0;
    int start_index = 0;        //The index to resume sorting from on the next pass.

    vector < int >::iterator e1 = v.begin() + start_index;      //Element 1
    vector < int >::iterator e2 = e1 + 1;       //Element 2

    if (v.size() > 10) {
        cout <<
            "List is greater than 10 elements. Debug is switched off!\n";
        debug = false;
    }

    while (pass <= n - 1)       //The terminating condition.
    {
        e1 = v.begin() + start_index;
        e2 = e1 + 1;

        while (e2 != v.end()) {
            comparisons++;

            if (*e2 < *e1) {
                tmp = *e1;
                *e1 = *e2;
                *e2 = tmp;
                swaps++;
            } else {
                break;
            }

            //Reached the top of the list, nothing left to sort on this pass.
            if (e1 == v.begin())
                break;

            //Start comparing elements above by tranversing back up the list.
            if (e1 > v.begin()) {
                e1--;
                e2--;
            }
        }

        //Preserve the running totals.
        swaps_total += swaps;
        comparisons_total += comparisons;

        //Move on the next pair of elements in the list to compare.
        start_index++;

        if (debug) {
            cout << "\nPass: " << pass << endl;
            cout << "Comparisons: " << comparisons << endl;
            cout << "Swaps: " << swaps << endl;
            print_vector(v);
        }
        //Prepare for the next pass.
        comparisons = 0;
        swaps = 0;
        pass++;
    }

    cout << "-------------------------------\n";
    cout << "Total Comparisons: " << comparisons_total << endl;
    cout << "Total Swaps: " << swaps_total << endl;
    cout << "-------------------------------\n";

    return;
}

/*
 * The Quick Sort algorithm.
 * On average, the algorithm will sort a list of numbers more quickly than most simpler
 * algorithms e.g. bubble, shuttle and shell etc.  Hence the name 'Quick Sort'.
 *
 * However, its worst case performance is quite poor and approaches that of the bubble 
 * or shuttle sort i.e. it is of the order n^2.
 *
 * Implementation based on the algorithm described at: https://en.wikipedia.org/wiki/Quicksort
 *
 * This implementation always places the pivot on the far RHS of the array and starts 
 * comparing from the far LHS.  The array/vector is modifed in place for efficiency.
 *
 * Class: Exchange sort, Comparison sort, Partitioning sort
 * Alternative names: partition-exchange sort
 *
 * Best case: O(n log n)
 * Worst case: O(n^2)
 * Average case: O(n log n)
 * Stable: false
 */
void quick_sort(vector < int >&v, vector < int >::iterator start,
                vector < int >::iterator end)
{
    bool debug = true;

    int tmp = 0;

    vector < int >::iterator pivot_it = end;    // Iterator pointing to the pivot
    vector < int >::iterator e1 = start;        //Start of the array to begin sorting.
    vector < int >::iterator e2 = pivot_it - 1; //Points to the element immediately to the LHS of the pivot.

    //cout << "pivot_value: " << *pivot_it << endl;

    //Check to ensure terminate the sort if the array has just 1 element.
    if (end - start < 1)
        return;

    if (v.size() > 10) {
        debug = false;
    }

    while (e1 != pivot_it)      //Valid terminating condition.
    {
        if (*e1 > *pivot_it) {
            /* The current element 'e1' (a larger element) needs to moved to the RHS of the pivot.
               You'll need to make space for it as the array is of fixed size. */

            /* Step 1 - Take a copy of it and swap it with the element closest to the pivot on the 
               LHS (this element will be evaluted later on). */
            tmp = *e1;
            *e1 = *e2;

            /* Step 2 - Move the pivot one space to the left to make space on the other side of it. */
            *e2 = *pivot_it;

            /* Step 3 - Swap the 'larger' element to the RHS of the pivot. */
            *pivot_it = tmp;

            /* Step 4 */
            pivot_it = e2;      //Reset the iterator to point to the pivot's new location.
            e2 = (pivot_it - 1);        //Update this iterator also as the pivot has moved.

            if (debug)
                print_vector(v);
        } else {
            /* Only iterate if the above comparison is false, as you'll need to evaluate the 
               element swapped out at Step 1. */
            e1++;
        }
    }

    //Recursively sort the remaining sub arrays.
    quick_sort(v, pivot_it + 1, end);
    quick_sort(v, start, e2);
}


//Useful utility functions.
long timevaldiff(struct timeval *starttime, struct timeval *finishtime)
{
    long msec;
    msec = (finishtime->tv_sec - starttime->tv_sec) * 1000;
    msec += (finishtime->tv_usec - starttime->tv_usec) / 1000;
    return msec;
}

void write_list(const char *output_file, int count)
{
    fstream out;

    out.open(output_file, ios::out);

    if (!out.is_open()) {
        cout << "Error! Could not open file" << endl;
        return;
    }
    //Write a reverse ordered list.  Useful for evaluation of an algorithm's 
    //worst case performance i.e. maximum number of comparisons and swaps required.
    int i = 0;

    for (i = count; i > 0; i--) {
        out << i << endl;
    }

    out.close();
}


//Vector utility functions.
void dump_vector(vector < int >&v, const char *output_file)
{
    fstream out;

    out.open(output_file, ios::out);

    if (!out.is_open()) {
        cout << "Error! Could not open file" << endl;
        return;
    }

    vector < int >::iterator it;

    for (it = v.begin(); it != v.end(); it++) {
        out << *it << "\n";
    }

    out.close();
}

void print_vector(vector < int >&v)
{
    vector < int >::iterator it;

    cout << "[ ";

    for (it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";
    }

    cout << "]\n";
}


//Main driver program used to test.
int main(int argc, char **argv)
{
    int number = 0;

    vector < int >v1, v2, v3;
    fstream data;

    //Load data
    data.open(argv[1]);

    if (!data.is_open()) {
        cout << "Error! Could not open file" << endl;
        return 0;
    }

    while (data >> number) {
        v1.push_back(number);
        v2.push_back(number);
        v3.push_back(number);
    }

    data.close();

    //Sort
    struct timeval t1, t2;


    cout << "\nSorting using the Bubble Sort.\n";
    gettimeofday(&t1, NULL);
    bubble_sort(v1);
    gettimeofday(&t2, NULL);

    cout << "-------------------------------\n";
    cout << "Time taken (ms): " << timevaldiff(&t1, &t2) << endl;
    cout << "-------------------------------\n";
    dump_vector(v1, "out1.dat");


    cout << "\nSorting using the Shuttle Sort.\n";
    gettimeofday(&t1, NULL);
    shuttle_sort(v2);
    gettimeofday(&t2, NULL);

    cout << "-------------------------------\n";
    cout << "Time taken (ms): " << timevaldiff(&t1, &t2) << endl;
    cout << "-------------------------------\n";
    dump_vector(v2, "out2.dat");


    cout << "\nSorting using the Quick Sort.\n\n";
    gettimeofday(&t1, NULL);
    quick_sort(v3, v3.begin(), v3.end() - 1);
    gettimeofday(&t2, NULL);

    cout << "-------------------------------\n";
    cout << "Time taken (ms): " << timevaldiff(&t1, &t2) << endl;
    cout << "-------------------------------\n";
    dump_vector(v3, "out3.dat");

    return 0;
}

/* EOF */
