#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Vooruit-declaraties van de klassen die later worden gebruikt
class Ray;
class Object;

// Klasse Vec3D definieert een 3D-vector met bewerkingen zoals optellen,
// vermenigvuldigen, en andere vectoroperaties.
class Vec3D {
public:
  Vec3D(float x = 0, float y = 0, float z = 0)
      : x(x), y(y), z(z) {} // Constructor met standaardwaarden
  float x, y, z;            // Coördinaten van de vector

  void show(const std::string &label) const {
    std::cout << label << " : " << x << " " << y << " " << z << '\n';
  }

  Vec3D minus() const { return Vec3D(-x, -y, -z); }

  Vec3D add(const Vec3D &other) const {
    return Vec3D(x + other.x, y + other.y, z + other.z);
  }

  Vec3D sub(const Vec3D &other) const {
    return Vec3D(x - other.x, y - other.y, z - other.z);
  }

  Vec3D mul(float scalar) const {
    return Vec3D(x * scalar, y * scalar, z * scalar);
  }

  Vec3D div(float scalar) const {
    return Vec3D(x / scalar, y / scalar, z / scalar);
  }

  float norm() const { return std::sqrt(x * x + y * y + z * z); }

  Vec3D unit() const {
    float length = norm();
    return (length != 0) ? div(length) : *this;
  }

  float dot(const Vec3D &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  Vec3D cross(const Vec3D &other) const {
    return Vec3D(y * other.z - z * other.y, z * other.x - x * other.z,
                 x * other.y - y * other.x);
  }

  // Copy constructor
  Vec3D(const Vec3D &other) : x(other.x), y(other.y), z(other.z) {}

  // Copy assignment operator
  Vec3D &operator=(const Vec3D &other) {
    if (this != &other) {
      x = other.x;
      y = other.y;
      z = other.z;
    }
    return *this;
  }

  // Move constructor and move assignment operator for efficiency
  Vec3D(Vec3D &&other) noexcept : x(other.x), y(other.y), z(other.z) {
    other.x = 0;
    other.y = 0;
    other.z = 0;
  }

  Vec3D &operator=(Vec3D &&other) noexcept {
    if (this != &other) {
      std::swap(x, other.x);
      std::swap(y, other.y);
      std::swap(z, other.z);
    }
    return *this;
  }
};

// Object is een abstracte klasse die de basis vormt voor objecten die kunnen
// worden geraakt door een straal (Ray).
class Object {
public:
  Object(float x, float y, float z) : center(x, y, z) {}
  virtual ~Object() = default; // Virtuele destructor, default implementatie

  virtual bool hit(Ray &ray) = 0; // Pure virtuele functie
  Vec3D center;
};

using VPO = std::vector<std::unique_ptr<Object>>; // Gebruik van unique_ptr voor
                                                  // automatisch geheugenbeheer

// De Ray-klasse vertegenwoordigt een straal in de ruimte die objecten kan
// raken.
class Ray {
public:
  Ray(float xStart, float yStart, VPO &objects)
      : support(0, 0, 3.0),
        direction(Vec3D(xStart, yStart, 0).sub(support).unit()),
        objects(objects) {}

  bool scan() const;

  Vec3D support, direction;
  VPO &objects;
};

// De Sphere-klasse is een specifieke vorm van Object die een bol representeert.
class Sphere : public Object {
public:
  Sphere(float x, float y, float z, float radius)
      : Object(x, y, z), radius(radius) {}

  bool hit(Ray &ray) override;
  float distFromRay(const Ray &ray) const;
  Vec3D hitPoint(const Ray &ray) const;

private:
  float radius;
};

// De Floor-klasse is een specifiek Object dat een vloer representeert.
class Floor : public Object {
public:
  Floor(float x, float y, float z, float side, float side2)
      : Object(x, y, z), side(side), side2(side2), normalVector(0, 1, 0) {}

  bool hit(Ray &ray) override;
  Vec3D hitPoint(const Ray &ray) const;

private:
  float side, side2;
  Vec3D normalVector;
};

// De RayScanner-klasse scant het scherm en controleert of de stralen objecten
// raken.
class RayScanner {
public:
  RayScanner();
  ~RayScanner() = default; // Default destructor aangezien unique_ptr
                           // automatisch geheugen beheert

  void scan();

private:
  VPO objects;
  void initializeObjects(); // Separated object initialization
};

// Implementatie van de Ray-methoden
bool Ray::scan() const {
  for (const auto &obj : objects) {
    if (obj->hit(const_cast<Ray &>(*this))) {
      return true;
    }
  }
  return false;
}

// Implementatie van de Sphere-methoden
float Sphere::distFromRay(const Ray &ray) const {
  Vec3D centerToRay = ray.support.sub(center); // Cached value
  return centerToRay.cross(ray.direction).norm();
}

bool Sphere::hit(Ray &ray) {
  float distanceToCenter = distFromRay(ray);
  if (distanceToCenter < radius) {
    ray.support = hitPoint(ray);
    Vec3D normalVector = ray.support.sub(center).unit();
    float dotProduct = ray.direction.dot(normalVector);

    // Improved reflection calculation
    Vec3D reflectionVector =
        ray.direction.sub(normalVector.mul(2 * dotProduct)).unit();

    // Allow for multiple reflections (e.g., bouncing rays)
    if (reflectionVector.dot(Vec3D(0, 1, 0)) < 0) {
      ray.direction = reflectionVector;
    } else {
      ray.direction =
          reflectionVector.minus(); // Reflect in the opposite direction
    }

    return true;
  }
  return false;
}

Vec3D Sphere::hitPoint(const Ray &ray) const {
  Vec3D centerToRayVector = ray.support.sub(center);
  float discriminant = std::pow(ray.direction.dot(centerToRayVector), 2) -
                       std::pow(centerToRayVector.norm(), 2) +
                       std::pow(radius, 2);

  // Controleer of discriminant negatief is
  if (discriminant < 0) {
    return ray.support; // Geen hit, retourneer huidige support
  }

  float distanceToHitPoint =
      -ray.direction.dot(centerToRayVector) - std::sqrt(discriminant);
  return ray.support.add(ray.direction.mul(distanceToHitPoint));
}

// Implementatie van de Floor-methoden
bool Floor::hit(Ray &ray) {
  const float epsilon = 1e-4f; // Drempelwaarde voor floating-point precisie
  if (ray.direction.y > -epsilon) {
    return false;
  }

  // Bereken en cache de hitpoint direct
  ray.support = hitPoint(ray);

  // Controleer of de hitpoint binnen de vloergrenzen valt
  float halfSide = side / 2;
  if (std::abs(ray.support.x - center.x) <= halfSide &&
      std::abs(ray.support.z - center.z) <= halfSide) {
    Vec3D radialComponent = normalVector.mul(ray.direction.dot(normalVector));
    Vec3D tangentialComponent = ray.direction.sub(radialComponent);
    ray.direction = tangentialComponent.sub(radialComponent).unit();

    int xCoord = 1000 + ray.support.x / side2;
    int zCoord = 1000 + ray.support.z / side2;

    return (xCoord % 2) != (zCoord % 2); // Checkerboard patroon
  }
  return false;
}

Vec3D Floor::hitPoint(const Ray &ray) const {
  float distanceToHitPoint = center.sub(ray.support).dot(normalVector) /
                             ray.direction.dot(normalVector);
  return ray.support.add(ray.direction.mul(distanceToHitPoint));
}

// Implementatie van de RayScanner-methoden
RayScanner::RayScanner() {
  initializeObjects(); // Separated object initialization
  scan();
}

void RayScanner::initializeObjects() {
  objects.reserve(4); // Reserveer capaciteit voor 4 objecten
  objects.emplace_back(std::make_unique<Floor>(0, -1.2, -2, 25, 0.8));
  objects.emplace_back(std::make_unique<Sphere>(-0.49, 0.26, -1, 0.4));
  objects.emplace_back(std::make_unique<Sphere>(0.35, 0.42, -1.2, 0.3));
  objects.emplace_back(std::make_unique<Sphere>(0.72, -0.12, -1.5, 0.2));
}

void RayScanner::scan() {
  std::cout << std::endl;
  float step = 0.5 / 60; // Increased resolution by decreasing step size
  for (auto y = 0.5; y >= -0.5; y -= step) {
    for (auto x = -0.5 / 0.4; x <= 0.5 / 0.4; x += step * 0.4) {
      Ray newRay(x, y, objects);
      std::cout << (newRay.scan() ? "N" : " ");
    }
    std::cout << '\n';
  }
}

// De main-functie start het programma door een RayScanner-object te maken
int main() {
  RayScanner scanner;
  return 0;
}
