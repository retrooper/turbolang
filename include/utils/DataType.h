#pragma once

namespace turbolang {
    enum DataType {
        DATA_TYPE_VOID = 0,
        DATA_TYPE_BOOL = 1,
        DATA_TYPE_BYTE = 2, DATA_TYPE_UBYTE = 3,
        DATA_TYPE_SHORT = 4, DATA_TYPE_USHORT = 5,
        DATA_TYPE_INT = 6,  DATA_TYPE_UINT = 7,
        DATA_TYPE_LONG = 8, DATA_TYPE_ULONG = 9,
        DATA_TYPE_FLOAT = 10,
        DATA_TYPE_DOUBLE = 11,
        DATA_TYPE_CLASS = 12
    };
}