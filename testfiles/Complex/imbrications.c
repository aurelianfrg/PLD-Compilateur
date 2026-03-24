int main() {
    int a = 0;
    if (a!=1) {
        int b = 2, a = 5;
        while (a!=b) {
            if (a > b) {
                --a;
                continue;
                ++a;
            }
            else {
                break;
            }
        }
        for (int i = 0; i < a; ++i) {
            int c = 2, d = 4;
            a += i;
            if (i == 1) {
                break;
            }
        }

        while (a == a) {
            if (b==b) {
                for (int i = 0; i<5; ++i) {
                    continue;
                }
                for (int i = 0; i<5; ++i) {
                    break;
                }
            }
            break;
        }
    }
    return a;
}
