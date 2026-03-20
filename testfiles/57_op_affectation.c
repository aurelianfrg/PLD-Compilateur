int main() {
    int a = 1, b = 3, c = 1;
    a = b += c;
    b /= 2;
    a %= 3;
    return b;
}