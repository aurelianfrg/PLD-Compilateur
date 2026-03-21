int main() {
    int a = 4;
    switch (a) {
    case 4:
        a = a - 1;
        break;
        /* a = a - 1; */
    default:
        a = 5;
    }
    return a;
}