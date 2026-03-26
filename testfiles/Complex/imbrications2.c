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
        int i = 0;
        while (i < a) {
            int c = 2, d = 4;
            a += i;
            if (i == 1) {
                break;
            }
            ++i;
        }

        while (a == a) {
            if (b==b) {
                int i = 0;
                while (i < 5) {
                    ++i;
                    continue;
                }
                i=0;
                while (i < 5) {
                    ++i;
                    break;
                }
                break;
            }
        }
    }
    return a;
}

