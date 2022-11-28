#pragma

#include <iterator>
#include <vector>


using std::vector;


template <typename T>
concept Comparable = requires(T x, T y) { x < y; x == y; };

template <Comparable T>
vector<T> DeleteRepetitions(const vector<T>& arr) {
    vector<T> result = arr;
    sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}
