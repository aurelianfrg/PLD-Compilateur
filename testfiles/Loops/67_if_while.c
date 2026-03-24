int main() {
    int a = 3;
    if (a == 3) {
        while (a) {
            a--;
            if (a == 2) {
                continue;
                a = 1;
            }
        }
    }
    return a;
}