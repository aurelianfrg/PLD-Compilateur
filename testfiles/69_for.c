int main() {
    int a = 1;
    int i, j;
    for (i = 0, j = 0; i < 5; i++) {
        a *= i + j;
        a = 0;
    }
    return a;
}