int main() {
    int i = 2, a = 0;
    while (i <= 2) {
        if (a == 1)
            continue;
        a = a + 1;
    }
    return a;
}