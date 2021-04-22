import "io"
import "string"
import "vector"
i32 main() {
    #fix angle calculation in vec3 to not be NAN
    Vec3 vecA;
    vecA.x = 1.0;
    vecA.y = 2.0;
    vecA.z = 3.0;

    Vec3 vecB;
    vecB.x = 2.0;
    vecB.y = 3.0;
    vecB.z = 4.0;

    f64 angle = getVecAngle(vecA, vecB);
    printf("Angle: %f\n", angle);
    return 0;
}