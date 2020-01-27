#if !defined(GILESC_MATH_H)
#define GILESC_MATH_H

#if USE_GILESC_MATH
#define Pi32         3.14159265359f
#define Tau2         Pi32 * 2
#define EulersNumber 2.718282f
#define Log10E       0.4342945f
#define Log2E        1.442695f
#define PiOver2      1.570796f
#define PiOver4      0.7853982f
#define TwoPi        6.283185f
#define Epsilon      0.000001f

#define Clamp(value, min, max) (((value) < (min)) ? (min) : (((value) > (max)) ? (max) : (value)))
#define SignOf(Value)    ((Value == 0) ? (Value = 1) : ((Value > 0) ? (Value / Value) : (-1 * (Value / Value))))

#define Min(X, Y)    ((X) < (Y) ? (X) : (Y))
#define Max(X, Y)    ((X) > (Y) ? (X) : (Y))

inline f32
Lerp(f32 a, f32 b, f32 f)
{
    f32 result = (a * (1.0 - f)) + (b * f);
    return result;
}

struct Degree {
    f32 value;
};
struct Radian {
    f32 value;
};

#define Radian(value) Radian{value}
#define Degree(value) Degree{value}
#define degrees_to_radians(angleInDegrees) (Radian((f32)((angleInDegrees) * M_PI / 180.0)))
#define radians_to_degrees(angleInRadians) (Degree((angleInRadians) * 180.0 / M_PI))

internal size_t
next_power_of_two(size_t allocation_size)
{
    u32 result = allocation_size;
    result--;
    result |= result >> 1;
    result |= result >> 2;
    result |= result >> 4;
    result |= result >> 8;
    result |= result >> 16;
    result++;

    return result;
}
#endif

#endif
