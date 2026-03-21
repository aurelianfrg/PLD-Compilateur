int main() {
    int i = 0, a = 0;
    while (i <= 2) {
        if (a == 1) {
            a = 15;
            continue;
        }
        i++;
        a++;

        /* a = a + 1; */
    }
    return a;
}