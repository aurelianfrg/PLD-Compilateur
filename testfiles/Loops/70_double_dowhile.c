int main() {
    int b, a = 0;
    do {
        b++;
        do {
            a += b;
        } while (a < 6 * b);
    } while (b < 4);
    return a;
}