declare f64 sqrt(f64);

class Vec3 {
    f64 x;
    f64 y;
    f64 z;
}

f64 getVecLength(Vec3 a) {
    f64 x = a.x;
    f64 y = a.y;
    f64 z = a.z;
    f64 value = x * x + y * y + z * z;
    f64 result = sqrt(value);
    return result;
}

f64 getVecDot(Vec3 a, Vec3 b) {
    f64 aX = a.x;
    f64 aY = a.y;
    f64 aZ = a.z;

    f64 bX = b.x;
    f64 bY = b.y;
    f64 bZ = b.z;
    return (aX * bX) + (aY * bY) + (aZ * bZ);

}

f64 getVecX(Vec3 a) {
    return a.x;
}

f64 getVecY(Vec3 a) {
    return a.y;
}

f64 getVecZ(Vec3 a) {
    return a.z;
}