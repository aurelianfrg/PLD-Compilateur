int main() {
    int a = 4, b;
    switch (a) {
    case 1 ... 4:
        b = 1;
    default:
        b = 0;
    }
    return b;
}