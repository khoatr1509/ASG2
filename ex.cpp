#include <iostream>
using namespace std;
int findF(int n, int k, int i, int count) {
    if (i > n) return -1;
    if (n % i == 0) {
        count++;
    }
    if (count == k) return i;
    i++;
    return findF(n, k, i, count);
}
int kthFactor(int n, int k) {
    return findF(n, k, 1, 0);
}
int main() {
    cout << kthFactor(1000, 3);
}
