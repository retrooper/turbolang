#include "utils/objectwrapper.h"


turbolang::objectwrapper::objectwrapper(bool b) {
    type = OBJECT_TYPE_BOOL;
    this->b = b;
}

turbolang::objectwrapper::objectwrapper(char c) {
    type = OBJECT_TYPE_CHAR;
    this->c = c;
    numericBitCount = CHAR_BIT;
}

turbolang::objectwrapper::objectwrapper(short s) {
    type = OBJECT_TYPE_SHORT;
    this->s = s;
    numericBitCount = 16;
}

turbolang::objectwrapper::objectwrapper(int i) {
    type = OBJECT_TYPE_INT;
    this->i = i;
    numericBitCount = 32;
}

turbolang::objectwrapper::objectwrapper(long l) {
    type = OBJECT_TYPE_LONG;
    this->l = l;
    numericBitCount = 64;
}

turbolang::objectwrapper::objectwrapper(float f) {
    type = OBJECT_TYPE_FLOAT;
    this->f = f;
    numericBitCount = 4;
}


turbolang::objectwrapper::objectwrapper(double d) {
    type = OBJECT_TYPE_DOUBLE;
    this->d = d;
    numericBitCount = 8;
}

turbolang::objectwrapper::objectwrapper(const char* str) {
    type = OBJECT_TYPE_STRING;
    this->str = str;
}

bool turbolang::objectwrapper::is_numeric_integer() const {
    return type == OBJECT_TYPE_CHAR ||
    type == OBJECT_TYPE_SHORT ||
    type == OBJECT_TYPE_INT ||
    type == OBJECT_TYPE_LONG;
}

bool turbolang::objectwrapper::is_numeric_floating_point() const {
    return type == OBJECT_TYPE_FLOAT || type == OBJECT_TYPE_DOUBLE;
}

long turbolang::objectwrapper::get_numeric_integer_value() const {
    switch (type) {
        case OBJECT_TYPE_CHAR:
            return c;
        case OBJECT_TYPE_SHORT:
            return s;
        case OBJECT_TYPE_INT:
            return i;
        case OBJECT_TYPE_LONG:
            return l;
    }
    return -1L;
}

double turbolang::objectwrapper::get_numeric_floating_value() const {
    if (type == OBJECT_TYPE_FLOAT) {
        return f;
    }
    else {
        return d;
    }
}