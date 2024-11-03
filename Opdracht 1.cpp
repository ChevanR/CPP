#include <iostream>
#include <vector>

// Function to check if a point (x, y) is inside a circle
bool isInsideCircle(int x, int y, int radius) {
  // Adjust for the aspect ratio (each y unit is taller than each x unit)
  float adjusted_y = y * 2.0f; // Compensating for the aspect ratio (10 vs 20)
  return (x * x + adjusted_y * adjusted_y) <= (radius * radius);
}

int main() {
  // Chessboard dimensions
  const int vertical_squares = 10;
  const int horizontal_squares = 16;
  const int square_size_vertical = 10;
  const int square_size_horizontal = 20;

  // Larger circle dimensions
  const int radius = 50;      // Increased circle radius
  const int thickness = 15;   // Increased thickness of the circle (ring)

  // Chessboard center coordinates
  const int centre_x = (horizontal_squares * square_size_horizontal) / 2;
  const int centre_y = (vertical_squares * square_size_vertical) / 2;

  // 2D vector representing the chessboard, initialized to 0.0f
  std::vector<std::vector<float>> chessboard(
      vertical_squares * square_size_vertical,
      std::vector<float>(horizontal_squares * square_size_horizontal, 0.0f));

  // Draw the chessboard
  bool change_color = false;
  for (int i = 0; i < vertical_squares * square_size_vertical; ++i) {
    if (i % square_size_vertical == 0)
      change_color = !change_color; // Alternate color for every new row

    for (int j = 0; j < horizontal_squares * square_size_horizontal; ++j) {
      if (j % square_size_horizontal == 0)
        change_color = !change_color; // Alternate color for every new square

      if (change_color)
        chessboard[i][j] = 0.5f; // Light square
    }
  }

  // Draw the ring (between inner and outer circles)
  for (int i = 0; i < vertical_squares * square_size_vertical; ++i) {
    for (int j = 0; j < horizontal_squares * square_size_horizontal; ++j) {
      // Coordinates relative to the center of the board
      int x = j - centre_x;
      int y = i - centre_y;

      // Check if the point is within the inner and outer circles
      bool insideInnerCircle = isInsideCircle(x, y, radius);
      bool insideOuterCircle = isInsideCircle(x, y, radius + thickness);

      // Color the ring region
      if (!insideInnerCircle && insideOuterCircle) {
        chessboard[i][j] = (chessboard[i][j] == 0.5f) ? 1.0f : 0.5f;
      }
    }
  }

  // Print the chessboard
  for (int i = 0; i < vertical_squares * square_size_vertical; ++i) {
    for (int j = 0; j < horizontal_squares * square_size_horizontal; ++j) {
      if (chessboard[i][j] == 1.0f)
        std::cout << "N"; // Dark square (ring)
      else if (chessboard[i][j] == 0.5f)
        std::cout << "L"; // Light square
      else
        std::cout << " "; // Empty space
    }
    std::cout << std::endl;
  }

  return 0;
}
