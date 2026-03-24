int main() {
    int a = 4, b;
    switch (a) {
    case 1:
        b = 1;
    case 2:
        b = 2;
    case 3:
        b = 3;
    case 4:
        b = 4;
    default:
        b = 5;
    }
    return b;
}