#include <iostream>
#include <random>
#include "math.h" // function declarations for math formulas

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

    double dt = time_to_expiration / num_steps;

    std::random_device rd;
    std::mt19937 rng(rd()); // random number generator

    /**
     * Specify the distribution of the randomly generated values for Z. We want it to follow a realistic bell curve since thats how price shocks behave in real life (where most price changes are very minor - near 0), so we specify a standard deviation of 1.
     * 
     * Mean is 0 because we want to keep the model unbiased, meaning the random shocks modeled by Z are equally likely to push the price up or down
     */
    std::normal_distribution<double> dist(0.0, 1.0); // mean 0, stddev 1
    
    std::vector<double> final_prices;

    // generate n number of paths where n = num_paths
    for (int i = 0; i < num_paths; i++) {
        double Z = dist(rng); // generate a random number
    
        double current_price{asset_price};

        // 1 path
        for (int j = 0; j < num_steps; j++) {
            Z = dist(rng);
            current_price = nextPrice(current_price, interest_rate, volatility, dt, Z);
        }
        final_prices.push_back(current_price); // add final price only
    }

    double put_price = calculate_put_price(final_prices, strike_price, interest_rate, time_to_expiration);
    double call_price = calculate_call_price(final_prices, strike_price, interest_rate, time_to_expiration);
    double analytical_put{black_scholes_put(asset_price, strike_price, interest_rate, volatility, time_to_expiration)};
    double analytical_call{black_scholes_call(asset_price, strike_price, interest_rate, volatility, time_to_expiration)};

    std::cout << "Results: \n";

    std::cout << "===Single Threaded Monte Carlo Simulation Engine===\n";
    std::cout << "Put Price: " << put_price << ".\n";
    std::cout << "Call Price: " << call_price << ".\n";

    std::cout << "===Black Scholes Analytical Formula===\n";
    std::cout << "Put Price: " << analytical_put << ".\n";
    std::cout << "Call Price: " << analytical_call << ".\n";
    return 0;
}