#import "vector"
import "io"
i32 main() {
    #Vec3 vec;
    #vec.x = 10.0;
    #vec.y = 5.0;
    #vec.z = 10.0;
    #f64 x = getVecX(vec);
    #f64 y = getVecY(vec);
    #f64 z = getVecZ(vec);
    i32 length = 1 * 10 + 1 * 10;
    printf("Length: %d\n", length);
    #printf("X: %f, Y: %f, Z: %f\n", x, y, z);
    return 0;
}