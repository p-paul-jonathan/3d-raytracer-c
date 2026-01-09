#ifndef VECTOR_3D_H
#define VECTOR_3D_H

#include <stdbool.h>

/**
 * @file vector_3d.h
 * @brief 3D vector math utilities for ray tracing and geometry.
 *
 * This module provides a small, immutable, heap-free 3D vector
 * implementation suitable for:
 *  - ray tracing
 *  - geometry
 *  - lighting calculations
 *  - physics-style math
 *
 * All functions return new vectors and never modify inputs.
 */

/**
 * @struct Vector3D
 * @brief Represents a 3D vector.
 *
 * Used to represent:
 *  - positions
 *  - directions
 *  - normals
 *  - colors (when interpreted as RGB)
 */
typedef struct {
  float x; /**< X component */
  float y; /**< Y component */
  float z; /**< Z component */
} Vector3D;

/* -------------------------------------------------------------------------
 * Constructors
 * ------------------------------------------------------------------------- */

/**
 * @brief Create a vector from components.
 *
 * @param x X component
 * @param y Y component
 * @param z Z component
 * @return Initialized vector
 */
Vector3D vector_3d_init(float x, float y, float z);

/**
 * @brief Return the zero vector (0,0,0).
 *
 * @return Zero vector
 */
Vector3D vector_3d_zero(void);

/* -------------------------------------------------------------------------
 * Basic arithmetic
 * ------------------------------------------------------------------------- */

/**
 * @brief Add two vectors.
 *
 * @param a First vector
 * @param b Second vector
 * @return a + b
 */
Vector3D vector_3d_add(Vector3D a, Vector3D b);

/**
 * @brief Subtract two vectors.
 *
 * @param a First vector
 * @param b Second vector
 * @return a - b
 */
Vector3D vector_3d_subtract(Vector3D a, Vector3D b);

/**
 * @brief Negate a vector.
 *
 * @param v Vector to negate
 * @return -v
 */
Vector3D vector_3d_negate(Vector3D v);

/* -------------------------------------------------------------------------
 * Dot & cross products
 * ------------------------------------------------------------------------- */

/**
 * @brief Compute the dot product of two vectors.
 *
 * @param a First vector
 * @param b Second vector
 * @return Scalar dot product
 *
 * @note
 * The dot product is defined as:
 * \f[
 * a \cdot b = |a||b|\cos(\theta)
 * \f]
 *
 * Common uses:
 *  - lighting (Lambert / Phong)
 *  - angle between vectors
 *  - projections
 */
float vector_3d_dot_product(Vector3D a, Vector3D b);

/**
 * @brief Compute the cross product of two vectors.
 *
 * @param a First vector
 * @param b Second vector
 * @return Vector perpendicular to a and b
 *
 * @note
 * The magnitude of the cross product is:
 * \f[
 * |a \times b| = |a||b|\sin(\theta)
 * \f]
 *
 * Direction follows the right-hand rule.
 */
Vector3D vector_3d_cross_product(Vector3D a, Vector3D b);

/* -------------------------------------------------------------------------
 * Comparison
 * ------------------------------------------------------------------------- */

/**
 * @brief Compare two vectors for equality.
 *
 * @param a First vector
 * @param b Second vector
 * @param epsilon Tolerance value
 * @return true if vectors are equal within epsilon
 *
 * @note
 * - If epsilon == 0, performs exact floating-point comparison.
 * - If epsilon > 0, performs component-wise comparison within tolerance.
 */
bool vector_3d_equal(Vector3D a, Vector3D b, float epsilon);

/* -------------------------------------------------------------------------
 * Magnitude & scaling
 * ------------------------------------------------------------------------- */

/**
 * @brief Compute the magnitude (length) of a vector.
 *
 * @param v Input vector
 * @return |v|
 *
 * @note
 * \f[
 * |v| = \sqrt{x^2 + y^2 + z^2}
 * \f]
 */
float vector_3d_magnitude(Vector3D v);

/**
 * @brief Multiply a vector by a scalar.
 *
 * @param v Input vector
 * @param k Scalar value
 * @return v * k
 */
Vector3D vector_3d_multiply_scalar(Vector3D v, float k);

/**
 * @brief Normalize a vector.
 *
 * @param v Input vector
 * @return Unit-length vector in the same direction
 *
 * @note
 * If v is the zero vector, returns (0,0,0).
 */
Vector3D vector_3d_normalize(Vector3D v);

/* -------------------------------------------------------------------------
 * Reflection
 * ------------------------------------------------------------------------- */

/**
 * @brief Reflect a vector around a surface normal.
 *
 * @param v Incoming vector
 * @param normal Surface normal (must be normalized)
 * @return Reflected vector
 *
 * @note
 * Reflection formula:
 * \f[
 * r = v - 2(v \cdot n)n
 * \f]
 *
 * Used for:
 *  - mirror reflections
 *  - specular lighting
 */
Vector3D vector_3d_reflect(Vector3D v, Vector3D normal);

#endif /* VECTOR_3D_H */
