#pragma once
#include <bits/stdc++.h>
#include <string>
 enum objectype {
    OBJECT_TYPE_BOOL, OBJECT_TYPE_CHAR, OBJECT_TYPE_SHORT, OBJECT_TYPE_INT,
    OBJECT_TYPE_LONG, OBJECT_TYPE_FLOAT, OBJECT_TYPE_DOUBLE, OBJECT_TYPE_STRING
};
namespace turbolang {
    struct objectwrapper {
        objectype type;
        int numericBitCount;
        bool b;
        char c;
        short s;
        int i;
        long l;
        float f;
        double d;
        std::string str;

        objectwrapper(const char* str);

        objectwrapper(bool b);

        objectwrapper(char c);

        objectwrapper(short s);

        objectwrapper(int i);

        objectwrapper(long l);

        objectwrapper(float f);

        objectwrapper(double d);

        bool is_numeric_integer() const;

        bool is_numeric_floating_point() const;

        long get_numeric_integer_value() const;

        double get_numeric_floating_value() const;
    };


}
