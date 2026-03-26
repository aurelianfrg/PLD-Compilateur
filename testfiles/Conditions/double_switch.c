int main() {
    int a = 4, b = 3, c = 0;
    switch (a) {
    case 1 ... 4:
        switch (b) {
        case 1 ... 2:
            c = 7;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
    return c;
}