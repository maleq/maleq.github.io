//
// Created by Sharon Boddu on 11/2/2024.
//

#ifndef UTIL_H  // FILE GUARD
#define UTIL_H

// Includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>
#include <stdint.h>
#include <iomanip>
#include <string>
#include <set>
#include <unordered_map>
#include <map>


using Vertex = uint64_t;
using VSize = uint64_t;

using namespace std;

#define Min(x, y) (((x) < (y)) ? (x) : (y))
#define Max(x, y) (((x) > (y)) ? (x) : (y))
#define Sqr(x) ((x) * (x))

template <typename T>
struct pair_hash {
    template <class U, class V>
    size_t operator()(const std::pair<U, V>& p) const {
        auto hash1 = std::hash<U>{}(p.first);
        auto hash2 = std::hash<V>{}(p.second);
        return hash1 ^ hash2;
    }
};


/*.....................................................
        Used in Timer
......................................................*/
template <class result_t   = std::chrono::microseconds,
          class clock_t    = std::chrono::steady_clock,
          class duration_t = std::chrono::microseconds>
auto since(std::chrono::time_point<clock_t, duration_t> const& start) {
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

/*.....................................................
        Initialize iterable container
......................................................*/
template<class ForwardIt, class T>
void fill_(ForwardIt first, ForwardIt last, T value) {
    for (; first != last; ++first)
        *first = value;
}

template<class ForwardIt, class T>
void fill_sequence( ForwardIt first, ForwardIt last, T value) {
    for (; first != last; ++first, ++value)
        *first = value;
}

template <typename It, typename Cmp>
It max_element_(It begin, It end, Cmp comp=[](const auto& a, const auto& b)
                                                              { return a.second < b.second;}) {
    if (begin == end) {
        return end;
    }
    It maxIt = begin;
    ++begin;
    for (; begin != end; ++begin) {
        if (comp(*maxIt, *begin)) {
            maxIt = begin;
        }
    }
    return maxIt;
}

template <typename It, typename Cmp>
It max_element(It begin, It end, Cmp comp) {
    if (begin == end) {
        return end;
    }
    It maxIt = begin;
    ++begin;
    for (; begin != end; ++begin) {
        if (comp(*maxIt, *begin)) {
            maxIt = begin;
        }
    }
    return maxIt;
}


/*.....................................................
        Function to print to file
......................................................*/
static inline void filePutContents(const string &name, const string &content,
                                   bool append = false) {
    ofstream outfile;
    if (append)
        outfile.open(name.c_str(), ios::app);
    else
        outfile.open(name.c_str());
    outfile << content;
    outfile.close();
}
template<typename T, typename SizeT>
static inline void filePutContents(std::string name, T* arr, SizeT size) {
    std::ofstream outfile;
    outfile.open(name, std::ios_base::app);
    for (SizeT i = 0; i < size; ++i) outfile << std::setw(16) << std::left << arr[i];
    outfile << std::endl << std::endl;
    outfile.close();
}
template<typename T>
static inline void filePutContents(std::string name, T x) {
    std::ofstream outfile;
    outfile.open(name, std::ios_base::app);
    outfile << std::setw(16) << std::left << x;
    outfile.close();
}

/*........................................................................
        Print arr to console
.........................................................................*/
template<typename T, typename SizeT>
void printArray(T* arr, SizeT size) {
    for (SizeT i = 0; i < size; ++i)
        cout << arr[i] << " ";
    cout << endl;
}
template<typename T, typename SizeT>
void print2DArray(T* arr, SizeT size1, SizeT size2) {
    for (SizeT i = 0; i < size1; ++i)
        printArray(arr[i], size2);
    cout << endl;
}
template<typename T, typename SizeT>
size_t arrayUniqueCount(T* arr, SizeT size) {
    const std::set<int> s(arr, arr + size);
    return s.size();
}

/*........................................................................
        Print arr to File
.........................................................................*/
template<typename T, typename SizeT>
void printArray2File(string outFile, T* arr, SizeT size) {
    ofstream f;
    f.open (outFile, ofstream::out | ofstream::app);

    for (SizeT i = 0; i < size; ++i)
        f << arr[i] << " ";
    f << endl;

    f.close();
}

template<typename T, typename SizeT>
void print2DArray2File(string outFile, T* arr, SizeT size1, SizeT size2) {
    ofstream f;
    f.open (outFile, ofstream::out | ofstream::app);

    for (SizeT i = 0; i < size1; ++i)
        printArray(arr[i], size2);
    f << endl;

    f.close();
}


/*.....................................................
		Find Max or Min
......................................................*/
template <typename T, typename SizeT>
void findMax(T *d, SizeT size, T* max, SizeT* ind) { //TODO: different version
    T Max = d[0];
    SizeT Ind = 0;
    for (SizeT i=1; i<size; ++i)
        if (Max < d[i]) {
            Max = d[i];
            Ind = i;
        }
    *max = Max;
    *ind = Ind;
    return;
}

template <typename T, typename SizeT>
T findMax(T *d, SizeT  size) {
    T Max = d[0];
    for (SizeT i=1; i<size; ++i)
        if (Max < d[i])
            Max = d[i];
    return Max;
}

template <typename T, typename SizeT>
void findMin(T *d, SizeT  size, T* min, SizeT* ind) {
    T Min = d[0];
    SizeT Ind = 0;
    for (SizeT i=1; i<size; ++i)
        if (Min > d[i]) {
            Min = d[i];
            Ind = i;
        }
    *min = Min;
    *ind = Ind;
    return;
}

template <typename T, typename SizeT>
T findMin(T *d, SizeT  size) {
    T Min = d[0];
    for (SizeT i=1; i<size; ++i)
        if (Min > d[i])
            Min = d[i];
    return Min;
}

/*.....................................................
		Round to nearest int
......................................................*/
inline int Round(double x) {
    int ix = static_cast<int>(x);
    if (x - ix < 0.5)
        return ix;
    return (ix+1);
}

/*.....................................................
		Find Sum
......................................................*/
template <typename T, typename SizeT>
T sumArray(T *d, SizeT i1, SizeT i2) {
    T sum = 0;
    for (SizeT i=i1; i<i2; ++i)
        sum += d[i];
    return sum;
}

template <typename T, typename SizeT>
T sumArray(T *d, SizeT size) {
    T sum = 0;
    for (SizeT i=0; i<size; ++i)
        sum += d[i];
    return sum;
}

/*.....................................................
		Swap contents of x and y
......................................................*/
template <typename T>
void swap(T *x, T *y) {
    T temp = *x;
    *x = *y;
    *y = temp;
}
template <typename T, typename SizeT>
void swapInArray(T* arr, SizeT i1, SizeT i2) {
    T temp = arr[i1];
    arr[i1] = arr[i2];
    arr[i2] = temp;
}
/*--------------------- Swap the content of x and y ------------------------*/
template <typename T>
inline void Swap(T &x, T &y) {
    T tmp = x;
    x = y;
    y = tmp;
}

/*........................................................................
		Swap and map
.........................................................................*/
template<typename T, typename SizeT>
void swapInMap(T* arr, SizeT i1, SizeT i2, std::unordered_map<T, SizeT>& mp) {
    mp[arr[i1]] = i2;
    mp[arr[i2]] = i1;
    swapInArray(arr, i1, i2);
}



/*........................................................................
		Copy from arr2 onto arr1
.........................................................................*/
template<typename T, typename SizeT>
void copyArray(T* arr1, T* arr2, SizeT size) {
    for (SizeT i = 0; i < size; ++i)
        arr1[i] = arr2[i];
}
template<typename T, typename SizeT>
void normArray(T* arr1, SizeT size, T val) {
    for (SizeT i = 0; i < size; ++i)
        arr1[i] /= val;
}
template<typename T, typename SizeT>
void copy2DArray(T** arr1, T** arr2, SizeT n1, SizeT n2) {
    for (SizeT i = 0; i < n1; ++i)
        for (SizeT j = 0; j < n2; ++j)
            arr1[i][j] = arr2[i][j];
}
template<typename T, typename SizeT>
void clear2DArray(T** arr, SizeT size) {
    for (SizeT i = 0; i < size; ++i)
        delete[] arr[i];
    delete[] arr;
}

template<typename SizeT>
void clear2DArray(std::string* arr, SizeT size) {
    for (SizeT i = 0; i < size; ++i)
        arr[i].clear();
    delete[] arr;
}

/*........................................................................
        Initialize Array
.........................................................................*/
template<typename T, typename SizeT>
void arrayInit(T* arr, SizeT size, T value) {
    for (SizeT i = 0; i < size; ++i)
        arr[i] = value;
}
template<typename T, typename SizeT>
void arrayInit(T* arr, SizeT size) {
    for (SizeT i = 0; i < size; ++i)
        arr[i] = i;
}

template<typename T, typename SizeT>
void arrayInit(T* arr1, SizeT size, T* arr2) {
    for (SizeT i = 0; i < size; ++i)
        arr1[i] = arr2[i];
}

/*........................................................................
        Compare Array
.........................................................................*/
template<typename T, typename SizeT>
bool compareArray(T* arr1, T* arr2, SizeT size) {
    std::map<T, SizeT> mp;
    for (SizeT i = 0; i < size; ++i) {
        ++mp[arr1[i]];
        ++mp[arr2[i]];
    }
    bool res = true;
    for (auto i : mp) {
        if (i.second != 2) res = false;
    }
    return res;
}
template<typename T, typename SizeT>
bool compareVector(T* arr1, T* arr2, SizeT size) {
    bool res = true;

    for (SizeT i = 0; i < size; ++i) {
        if (arr1[i] != arr2[i])
            res = false;
    }

    return res;
}

/*........................................................................
        Initialize Array
.........................................................................*/

template<typename T, typename SizeT>
void arrayMapInit(T* arr, SizeT size, std::unordered_map<T, SizeT>& mp) {
    for (SizeT i = 0; i < size; ++i) {
        arr[i] = i;
        mp[i] = i;
    }
}


/*........................................................................
        Right Rotate Array
.........................................................................*/
template<typename T, typename SizeT>
void arrayRotate(T* arr, SizeT size) {
    T temp = arr[size - 1];
    for (SizeT i = size - 2; i >= 0; --i) {
        arr[i + 1] = arr[i];
        if (i == 0) break;
    }

    arr[0] = temp;
}
template<typename T, typename SizeT>
void arrayRotate(T* arr, SizeT size, int k) {
    for (int i = 0; i < k; ++i) arrayRotate(arr, size);
}

/*........................................................................
		Set Map
.........................................................................*/
template<typename T, typename SizeT>
void setMap(T* arr, SizeT size, std::unordered_map<T, SizeT>& mp) {
    for (SizeT i = 0; i < size; ++i) {
        mp[arr[i]] = i;
    }
}



/*........................................................................
		Random Permutation of array
.........................................................................*/
template<typename T, typename SizeT>
void randomPermute(T* arr, std::map<T, SizeT>& mp, SizeT size) {
    for (SizeT i = 0; i < size; ++i) arr[i] = i, mp[i] = i;
    for (SizeT i = 0; i < size; ++i) {
        SizeT j = i + rand() % (size - i);
        swap(&arr[i], &arr[j]);
        swap(&mp[arr[i]], &mp[arr[j]]);
    }
}
template<typename T, typename SizeT>
void randomPermute(T* arr, std::unordered_map<T, SizeT>& mp, SizeT size) {
    for (SizeT i = 0; i < size; ++i) arr[i] = i, mp[i] = i;

    for (SizeT i = 0; i < size; ++i) {
        SizeT j = i + rand() % (size - i);
        swap(&arr[i], &arr[j]);
        swap(&mp[arr[i]], &mp[arr[j]]);
    }
}
template<typename T, typename SizeT>
void randomPermute(T* arr, SizeT size) {
    for (SizeT i = 0; i < size; ++i) arr[i] = i;

    for (SizeT i = 0; i < size; ++i) {
        SizeT j = i + rand() % (size - i);
        swap(&arr[i], &arr[j]);
    }
}

/*........................................................................
		sort array
.........................................................................*/
template<typename T, typename SizeT>
void Sort(T *d, SizeT  left, SizeT right) {
    SizeT  i, k;

    for (k=left, i=left+1; i <= right; ++i) {
        if (d[i] < d[left]) {
            ++k;
            Swap(d[i], d[k]);
        }
    }
    Swap(d[k], d[left]);
    if (left+1 < k) Sort(d, left, k-1);
    if (k+1 < right) Sort(d, k+1, right);
    return;
}

template <typename T, typename SizeT>
void RecSort(T *c, T *d, SizeT  left, SizeT right) {// c = key, d = value; sort based on d
    SizeT  i, k;

    for (k=left, i=left+1; i <= right; ++i) {
        if (d[i] < d[left]) {
            ++k;
            Swap(d[i], d[k]);
            Swap(c[i], c[k]);
        }
    }
    Swap(d[k], d[left]);
    Swap(c[k], c[left]);
    if (left+1 < k) RecSort(c, d, left, k-1);
    if (k+1 < right) RecSort(c, d, k+1, right);
    return;
}

template <typename T, typename SizeT>
void RecSort(T *d, SizeT  left, SizeT right){
    SizeT  i, k;

    for (k=left, i=left+1; i <= right; ++i) {
        if (d[i].Key() < d[left].Key()) {
            ++k;
            Swap(d[i], d[k]);
        }
    }
    Swap(d[k], d[left]);
    if (left+1 < k) RecSort(d, left, k-1);
    if (k+1 < right) RecSort(d, k+1, right);
    return;
}

/*---------------- Sort elements in d in descending order ------------------*/
template <typename T, typename SizeT>
void SortD(T *d, SizeT  left, SizeT right) {
    SizeT  i, k;

    for (k=left, i=left+1; i <= right; ++i) {
        if (d[i] > d[left]) {
            ++k;
            Swap(d[i], d[k]);
        }
    }
    Swap(d[k], d[left]);
    if (left+1 < k) SortD(d, left, k-1);
    if (k+1 < right) SortD(d, k+1, right);
    return;
}


/*----- Sort elements in d based on: first its label, then itself ----------*/
template <typename T, typename SizeT, typename LabelT>
void Sort(T *d, SizeT  left, SizeT right, LabelT *label) {
    SizeT  i, k;

    for (k=left, i=left+1; i <= right; ++i) {
        if ( (label[d[i]] < label[d[left]])
            || (label[d[i]]==label[d[left]] && d[i]<d[left]) )
        {
            ++k;
            Swap(d[i], d[k]);
        }
    }
    Swap(d[k], d[left]);
    if (left+1 < k) Sort(d, left, k-1, label);
    if (k+1 < right) Sort(d, k+1, right, label);
    return;
}



/*-------------- Sort elements in d based on label only --------------------*/
template <typename T, typename SizeT, typename LabelT>
void LSort(T *d, SizeT  left, SizeT right, LabelT *label) {
    SizeT  i, k;

    for (k=left, i=left+1; i <= right; ++i) {
        if (label[d[i]] < label[d[left]]) {
            ++k;
            Swap(d[i], d[k]);
        }
    }
    Swap(d[k], d[left]);
    if (left+1 < k) LSort(d, left, k-1, label);
    if (k+1 < right) LSort(d, k+1, right, label);
    return;
}



/*-------------- Sort records in d based on label only --------------------*/
template <typename T, typename SizeT, typename LabelT>
void LRecSort(T *d, SizeT  left, SizeT right, LabelT *label) {
    SizeT  i, k;

    for (k=left, i=left+1; i <= right; ++i) {
        if (label[d[i].Key()] < label[d[left].Key()]) {
            ++k;
            Swap(d[i], d[k]);
        }
    }
    Swap(d[k], d[left]);
    if (left+1 < k) LRecSort(d, left, k-1, label);
    if (k+1 < right) LRecSort(d, k+1, right, label);
    return;
}

template <typename ArrayT, typename T>
void PrintList(ArrayT *list, T size) {
    cout << "(" << size << "): ";
    for (T i=0; i<size; ++i)
        cout << static_cast<int>(list[i]) << " ";
    cout << endl;
}

/*--------------------- Print an array of records ----------------------*/
template <typename ArrayT, typename T>
void PrintRecList(ArrayT *list, T size) {
    cout << "(" << size << "): ";
    for (T i=0; i<size; ++i)
        list[i].Print();
}

/*----------------------- Fill array a with value -------------------------*/
template <typename T, typename SizeT>
inline void Fill(T *a, SizeT size, T value) {
    for (SizeT i=0; i<size; ++i)
        a[i] = value;
}



/*---- MappedFill fills only the mapped elements (defined by map) of a ----*/
template <typename T, typename MapT, typename SizeT>
inline void MFill(T *a, MapT *map, SizeT size, T value) {
    for (SizeT i=0; i<size; ++i)
        a[map[i]] = value;
}


/*-------------------------- Copy array b to a -----------------------------*/
template <typename T, typename SizeT>
inline void Copy(T *a, T *b, SizeT size) {
	for (SizeT i=0; i<size; ++i)
		a[i] = b[i];
}



/*------- Find intersection of sorted arrays a and b, and store in a ------*/
template <typename T, typename SizeT>
inline SizeT Intersection(T *a, SizeT sa, T *b, SizeT sb) {
	SizeT s, i, k;

	for (s=i=k=0; k<sb && i<sa; ++i) {
		while (k<sb && b[k]<a[i]) ++k;
		if (k<sb && b[k]==a[i]) a[s++] = a[i];
	}
	return s;
}



/*------- Find intersection of sorted arrays a and b, and store in c ------*/
template <typename T, typename SizeT>
inline SizeT Intersection(T *c, T *a, SizeT sa, T *b, SizeT sb) {
	SizeT s, i, k;

	for (s=i=k=0; k<sb && i<sa; ++i) {
		while (k<sb && b[k]<a[i]) ++k;
		if (k<sb && b[k]==a[i]) c[s++] = a[i];
	}
	return s;
}


/*------------------ search item in UNSORTED array a ----------------------*/
template <typename T, typename SizeT>
inline bool LSearch(T *a, SizeT size, T item) {
	for (SizeT i=0; i<size; ++i)
		if (a[i]==item) return true;
	return false;
}

/*------------------ search item in UNSORTED array a ----------------------*/
template <typename T, typename SizeT>
inline int LinearSearch(T *a, SizeT size, T item) {
    for (SizeT i=0; i<size; ++i)
        if (a[i]==item) return i;
    return -1;
}


/*------------- search item in UNSORTED array a of records ----------------*/
template <typename T, typename SizeT, typename KeyT>
inline bool LRecSearch(T *a, SizeT size, KeyT item) {
	for (SizeT i=0; i<size; ++i)
		if (a[i].Key()==item) return true;
	return false;
}


/*-------- Binary search item in SORTED array a ---------------------------*/
template <typename T, typename SizeT>
inline SizeT BSearch(T *a, SizeT left, SizeT right, T item) {
	SizeT mid;
	while (left <= right) {
		mid = (left + right) / 2;
		if (item == a[mid]) return (mid+1);
		if (item < a[mid]) right = mid - 1;
		else left = mid + 1;
	}
	return 0;
}

/*-------- Binary search item in SORTED array a ---------------------------*/
template <typename T, typename SizeT>
inline SizeT BinarySearch(T *a, SizeT left, SizeT right, T item) {
    SizeT mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (item == a[mid]) return (mid+1);
        if (item < a[mid]) right = mid - 1;
        else left = mid + 1;
    }
    return 0;
}


/*-------- Binary search item in array a SORTED in descending order -------*/
template <typename T, typename SizeT>
inline SizeT BSearchD(T *a, SizeT left, SizeT right, T item) {
	SizeT mid;
	while (left <= right) {
		mid = (left + right) / 2;
		if (item == a[mid]) return (mid+1);
		if (item > a[mid]) right = mid - 1;
		else left = mid + 1;
	}
	return 0;
}



/*-------- Binary search item in SORTED array a ---------------------------*/
template <typename T, typename SizeT, typename KeyT>
inline bool BRecSearch(T *a, SizeT left, SizeT right, KeyT item) {
	SizeT mid;
	while (left <= right) {
		mid = (left + right) / 2;
		if (item == a[mid].Key()) return true;
		if (item < a[mid].Key()) right = mid - 1;
		else left = mid + 1;
	}
	return false;
}




/*-------- Binary range search item in SORTED array a ---------------------*/
// returns 0 if item < a[0], returns i if a[i-1] <= item < a[i]
// input item should be less than a[right], returns right if item >= a[right]
template <typename T, typename SizeT>
inline SizeT BRSearch(T *a, SizeT left, SizeT right, T item) {
	SizeT mid;
	while (left < right) {
		mid = (left + right) / 2;
		if (item < a[mid]) right = mid;
		else left = mid+1;
	}
	return left;
}


/*-- Binary range search LABELED item in SORTED (based on label) array a ---*/
// returns left if item < a[left], returns i if a[i-1] <= item < a[i]
// input item should be less than a[right], returns right if item >= a[right]
template <typename T, typename SizeT, typename LT>
inline SizeT BRSearch(T *a, SizeT left, SizeT right, LT item, LT *label) {
	SizeT mid;
	while (left < right) {
		mid = (left + right) / 2;
		if (item < label[a[mid]]) right = mid;
		else left = mid+1;
	}
	return left;
}


/*-- Binary range search LABELED item in SORTED (based on label) array a ---*/
// returns left if item < a[left], returns i if a[i-1] <= item < a[i]
// input item should be less than a[right], returns right if item >= a[right]
template <typename T, typename SizeT, typename LT>
inline SizeT BRRecSearch(T *a, SizeT left, SizeT right, LT item, LT *label) {
	SizeT mid;
	while (left < right) {
		mid = (left + right) / 2;
		if (item < label[a[mid].Key()]) right = mid;
		else left = mid+1;
	}
	return left;
}


/*---------------- Remove item from array d -------------------------------*/
template <typename T, typename SizeT>
inline SizeT  Remove(T *d, SizeT  left, SizeT right, T item) {
	SizeT  i, k;

	for (k=right+1, i=right; i >= left; --i)
		if (d[i]==item)  d[i] = d[--k];
	return k;
}


/*---------------- Remove item from tail of array d -----------------------*/
template <typename T, typename SizeT>
inline SizeT  RemoveTail(T *d, SizeT  left, SizeT right, T item) {
	SizeT  i;

	for (i=right; i>left && d[i]==item; --i) {}
    if (d[i]==0)
		return i;
	return (i+1);
}


/*---------------- FreeMem frees memory pointed by mem --------------------*/
template <typename T>
inline void  FreeMem(T &mem) {
	if (mem)
		delete [] mem;
	mem = nullptr;
}



/*------- FreeAll frees memory pointed by itself and its element -----------*/
template <typename T, typename SizeT>
inline void  FreeAll(T &mem, SizeT s) {
	if (mem) {
		for (SizeT i=0; i<s; ++i)
			delete [] mem[i];
		delete [] mem;
	}
	mem = NULL;
}


/*--------------- Fact returns the factorial of n -------------------------*/
template <typename T>
inline T  Factorial(T n) {
	T result=1, i;
	for (i=2; i<=n; ++i)
		result *= i;
	return result;
}


/*--------------- Skip next white spaces in a file ------------------------*/

template <typename FilePtr>
inline void SkipWhiteSpaces(FilePtr &fp) {
	char c = fp.peek();
	while (isspace(c)) {
		fp.ignore();
		c = fp.peek();
	}
}


template <typename T, typename SizeT, typename LabelT>
void Sort_LV(T *d, SizeT  left, SizeT right, LabelT *label) {
    SizeT  i, k;

    for (k=left, i=left+1; i <= right; ++i) {
        if ( (label[i] < label[left])
             || (label[i]==label[left] && d[i]<d[left]) )
        {
            ++k;
            Swap(d[i], d[k]);
            Swap(label[i], label[k]);
        }
    }
    Swap(d[k], d[left]);
    Swap(label[k], label[left]);
    if (left+1 < k) Sort_LV(d, left, k-1, label);
    if (k+1 < right) Sort_LV(d, k+1, right, label);
}





#endif //UTIL_H FILE GUARD
