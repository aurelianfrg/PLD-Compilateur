int c() {
	int c1 = 4;
	int c2 = 6;
	int c3 = 8;
	int c4 = 10;
	return c1 + 5;
}

int b() {
	int b1 = 3;
	int b2 = 5;
	int b3 = 7;
	return b1 + c();
}

int a() {
	int a1 = 2;
	int a2 = 4;
	return a1 + a2 + b();
}

int main() {
	return a();
}
