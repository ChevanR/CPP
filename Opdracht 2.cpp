#include <iostream>
#include <string>
#include <cmath> 

struct Vec3D {
  float x, y, z;
};

// Function to create a Vec3D
Vec3D vec3D(float x, float y, float z) {
  Vec3D result;
  result.x = x;
  result.y = y;
  result.z = z;
  return result;
}

// Function to print a Vec3D
void show(std::string label, Vec3D const &self) {
  std::cout << label << ": (" << self.x << ", " << self.y << ", " << self.z << ")\n";
}

// Function to print a scalar
void show(std::string label, float scalar) {
  std::cout << label << ": " << scalar << "\n";
}

// Function to print a newline
void show() {
  std::cout << "\n";
}

// Function to negate a Vec3D
Vec3D minus(Vec3D const &self) {
  return vec3D(-self.x, -self.y, -self.z);
}

// Function to add two Vec3Ds
Vec3D add(Vec3D const &self, Vec3D const &other) {
  return vec3D(self.x + other.x, self.y + other.y, self.z + other.z);
}

// Function to subtract two Vec3Ds
Vec3D sub(Vec3D const &self, Vec3D const &other) {
  return vec3D(self.x - other.x, self.y - other.y, self.z - other.z);
}

// Function to multiply a Vec3D by a scalar
Vec3D mul(Vec3D const &self, float scalar) {
  return vec3D(self.x * scalar, self.y * scalar, self.z * scalar);
}

// Function to divide a Vec3D by a scalar
Vec3D div(Vec3D const &self, float scalar) {
  return vec3D(self.x / scalar, self.y / scalar, self.z / scalar);
}

// Function to calculate the norm (length) of Vec3D
float norm(Vec3D const &self) {
  return sqrt(self.x * self.x + self.y * self.y + self.z * self.z);
}

// Function to get the unit vector of a Vec3D
Vec3D unit(Vec3D const &self) {
  float length = norm(self);
  if (length == 0.0f) {
    return vec3D(0.0f, 0.0f, 0.0f); 
  }
  return div(self, length);
}

// Function to calculate the dot product of two Vec3Ds
float dot(Vec3D const &self, Vec3D const &other) {
  return self.x * other.x + self.y * other.y + self.z * other.z;
}

// Function to calculate the cross product of two Vec3Ds
Vec3D cross(Vec3D const &self, Vec3D const &other) {
  return vec3D(
    self.y * other.z - self.z * other.y,
    self.z * other.x - self.x * other.z,
    self.x * other.y - self.y * other.x
  );
}

int main() {
  Vec3D v1 = vec3D(1.0f, 2.0f, 3.0f);
  Vec3D v2 = vec3D(4.0f, 5.0f, 6.0f);

  // Test functions
  show("v1", v1);
  show("v2", v2);

  show("minus(v1)", minus(v1));
  show("add(v1, v2)", add(v1, v2));
  show("sub(v1, v2)", sub(v1, v2));
  show("mul(v1, 2.0f)", mul(v1, 2.0f));
  show("div(v1, 2.0f)", div(v1, 2.0f));
  show("norm(v1)", norm(v1));
  show("unit(v1)", unit(v1));
  show("dot(v1, v2)", dot(v1, v2));
  show("cross(v1, v2)", cross(v1, v2));

  show(); 
  return 0;
}