int main() {
    int a = 1;
    for (int i = 0, j = 0; i < 5; i++) {
        for (int k = 2; k < 2; k++) {
            a += k;
        }
        a *= i + j;
    }
    return a;
}