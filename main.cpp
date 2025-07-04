#include <iostream>

/**
 * Returns the estimated value of the option based on the Black Scholes Model
 * 
 * Volatility measures how much the price of an asset tends to fluctuate over time.
 */
double black_scholes_output(double asset_price, double strike_price, double time_to_expiration, double volatility, double interest_rate, int num_paths, int num_steps) {

}

int main() {
    double asset_price{};
    double strike_price{};
    double time_to_expiration{};
    double volatility{};
    double interest_rate{};
    int num_paths{};
    int num_steps{};

    std::cout << "Enter the current asset price: " << "\n";
    std::cin >> asset_price;
    std::cout << "Enter the strike price: " << "\n";
    std::cin >> strike_price;
    std::cout << "Enter the time to expiration (in years, e.g., 0.5 for 6 months): " << "\n";
    std::cin >> time_to_expiration;
    std::cout << "Enter the volatility (as a decimal, e.g., 0.2 for 20%): " << "\n";
    std::cin >> volatility;
    std::cout << "Enter the risk-free interest rate (as a decimal, e.g., 0.05 for 5%): " << "\n";
    std::cin >> interest_rate;

    std::cout << "Algorithm Tuning Parameters:" << "\n";

    std::cout << "Enter the number of simulation paths (e.g., 100000): " << "\n";
    std::cin >> num_paths;
    std::cout << "Enter the number of time steps per path (e.g., 252): " << "\n"; 
    std::cin >> num_steps;  
    return 0;
}