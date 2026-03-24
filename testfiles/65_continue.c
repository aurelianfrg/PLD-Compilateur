int main() {
    int i = 0, a = 0;
    while (i <= 2) {
        if (a == 1) {
            a = 15;
            continue;
            a = a + 1;
        }
        i++;
        a++;
    }
    return a;
}