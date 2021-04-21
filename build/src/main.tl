import "vector"
import "io"
i32 main() {
    Vec3 vec;
    vec.x = 2.0;
    vec.y = 3.0;
    vec.z = 2.0;
    Vec3 vec2;
    vec2.x = 4.0;
    vec2.y = 5.0;
    vec2.z = 6.0;
    f64 length = getVecLength(vec);
    printf("Length: %f\n", length);
    return 0;
}