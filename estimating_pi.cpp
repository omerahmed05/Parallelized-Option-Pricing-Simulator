#include <iostream>
#include <random> // Provides the RNG engine (implemented with Mersenne Twister)

/**
 * Returns true if the point (x, y) lies inside the quarter circle of radius 1.
 */
bool isInsideCircle(double x, double y) {
    return (x* x + y*y) <= 1;
}

int main() {
    std::cout << "How many points would you like to generate? The more points you generate, the more accurate the approximation will be: ";
    int numPoints{};
    std::cin >> numPoints;

    std::cout << "Enter a random number: ";
    int randomUserInput{};
    std::cin >> randomUserInput;

    std::uniform_real_distribution<double> dist(0.0,1.0); // instantiate an object which maps the number it is passed (will be a RNG) to a random double in the range of 0.0 to 1.0
                                                          // uniform distribution, meaning any number in that range is equally likely to get mapped

    int inside_circle{}; // total number of points inside the circle

    /**
     * Randomly generated a value for xi and yi of the point
     * 
     * We use a single std::mt19937 RNG instance seeded once by the user.
     * Each call to dist(rng) advances the RNG and returns a new random number,
     * so calling it twice in a row (for x and y) gives two different values.
     * 
     * This is different from creating two separate RNGs with the same seed:
     * doing that would produce identical sequences and result in repeated values.
     */
    std::mt19937 rng(randomUserInput); 

    // generate the points 
    for (int i = 0; i < numPoints; i++) {
        double x {dist(rng)};
        double y {dist(rng)};

        if (isInsideCircle(x, y)) {
            inside_circle += 1;
        }
    }

    double ratio = static_cast<double>(inside_circle) / numPoints;
    double pi = ratio * 4;
    std::cout << "The appromixate value of pi is: " << pi << ".\n";

    return 0;
}