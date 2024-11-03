#include <iostream>
#include <vector>
#include <cmath>

// Vec3D class
class Vec3D {
public:
    float x, y, z;

    // Constructor
    Vec3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    // Method to compute the dot product of two Vec3D
    float dot(const Vec3D& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // Method to compute the vector subtraction
    Vec3D operator-(const Vec3D& v) const {
        return Vec3D(x - v.x, y - v.y, z - v.z);
    }

    // Method to compute the vector addition
    Vec3D operator+(const Vec3D& v) const {
        return Vec3D(x + v.x, y + v.y, z + v.z);
    }

    // Method to compute the scalar multiplication
    Vec3D operator*(float scalar) const {
        return Vec3D(x * scalar, y * scalar, z * scalar);
    }

    // Method to compute the length of the vector
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};

// Ray class
class Ray {
public:
    Vec3D support;
    Vec3D direction;

    // Constructor
    Ray(float xSup, float ySup, float zSup, float xDir, float yDir, float zDir)
        : support(xSup, ySup, zSup), direction(xDir, yDir, zDir) {}

    // Method to get a point on the ray at parameter t
    Vec3D pointAt(float t) const {
        return support + direction * t;
    }
};

// Sphere class
class Sphere {
public:
    Vec3D center;
    float radius;

    // Constructor
    Sphere(float x, float y, float z, float radius)
        : center(x, y, z), radius(radius) {}

    // Method to calculate the distance from the sphere's center to a ray
    float distFromRay(const Ray& ray) const {
        Vec3D diff = ray.support - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2 * diff.dot(ray.direction);
        float c = diff.dot(diff) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        return discriminant >= 0 ? std::sqrt(discriminant) : -1;  // Return -1 if no intersection
    }

    // Method to determine if a ray hits the sphere
    bool hit(const Ray& ray) const {
        return distFromRay(ray) >= 0;
    }

    // Method to calculate the hit point where the ray intersects the sphere
    Vec3D hitPoint(const Ray& ray) const {
        Vec3D diff = ray.support - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2 * diff.dot(ray.direction);
        float c = diff.dot(diff) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) return Vec3D();  // No intersection

        float sqrtDisc = std::sqrt(discriminant);
        float t1 = (-b - sqrtDisc) / (2 * a);
        float t2 = (-b + sqrtDisc) / (2 * a);

        // Choose the smallest positive t
        float t = (t1 > 0) ? t1 : t2;

        return ray.pointAt(t);
    }
};

int main() {
    // Define spheres
    std::vector<Sphere> spheres = {
        Sphere(-0.4, 0.23, -1, 0.4),
        Sphere(0.4, 0.4, -1.2, 0.3),
        Sphere(0.7, -0.15, -1.5, 0.2)
    };

    // Define rays
    float far = 1000;  // Or any sufficiently large value
    std::vector<Ray> rays = {
        Ray(-far, 0.23, -1, far, 0, 0),
        Ray(0.4, -far, -1.2, 0, far, 0),
        Ray(0.7, -0.15, -far, 0, 0, far)
    };

    // Check intersections and print results
    for (const auto& ray : rays) {
        for (const auto& sphere : spheres) {
            if (sphere.hit(ray)) {
                Vec3D hit_point = sphere.hitPoint(ray);
                std::cout << "Ray hits sphere at (" << hit_point.x << ", " << hit_point.y << ", " << hit_point.z << ")\n";
            }
        }
    }

    return 0;
}
