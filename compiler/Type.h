#pragma once

#include <unordered_map>
#include <string>

typedef enum {
	VOID,
	INT
} Type;

const unordered_map<Type, int> typeSizes = {
	{Type::VOID, 0},
	{Type::INT, 4}
};

const unordered_map<string, Type> typeFromString = {
	{string("void"), VOID},
	{string("int"), INT}
};

const unordered_map<Type, string> typeToString = {
	{VOID, string("void")},
	{INT, string("int")}
};