#include <iostream>
#include <queue>
using namespace std;
int main() {
    queue<int> v;
    for (int i = 0; i < 12; i++) {
        (v).push(i + 1);
    }
    queue<int> v1 = v;
    //delete v;
    while (!(v1).empty()) {
        cout << " " << (v1).front();
        (v1).pop();
    }
    return 0;
}