#include <iostream>
#include <cmath>
#include <random>

/**
 * Calculate the next asset price using geometric Brownian motion
 * 
 * @param S Current asset price
 * @param mu Interest rate
 * @param sigma Volatility (standard deviation)
 * @param dt Time step size
 * @param Z Random normal variable ~ N(0,1)
 * @return Next asset price
 */
double nextPrice(double S, double mu, double sigma, double dt, double Z) {
    // S_next = S * exp((mu - 0.5*sigma^2)*dt + sigma*sqrt(dt)*Z)
    double drift = (mu - 0.5 * sigma * sigma) * dt;
    double diffusion = sigma * std::sqrt(dt) * Z;
    
    return S * std::exp(drift + diffusion);
}

/**
 * Analytical Black Scholes Formula
 */

// Standard normal cumulative distribution function (CDF)
    // Using the error function erf for approximation
double norm_cdf(double x) {
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

// Black-Scholes formula for European Call Option price
double black_scholes_call(double S, double K, double r, double sigma, double T) {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    
    return S * norm_cdf(d1) - K * std::exp(-r * T) * norm_cdf(d2);
}

// Black-Scholes formula for European Put Option price
double black_scholes_put(double S, double K, double r, double sigma, double T) {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    
    return K * std::exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
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

    double dt = time_to_expiration / num_steps;

    std::random_device rd;
    std::mt19937 rng(rd()); // random number generator

    /**
     * Specify the distribution of the randomly generated values for Z. We want it to follow a realistic bell curve since thats how price shocks behave in real life (where most price changes are very minor - near 0), so we specify a standard deviation of 1.
     * 
     * Mean is 0 because we want to keep the model unbiased, meaning the random shocks modeled by Z are equally likely to push the price up or down
     */
    std::normal_distribution<double> dist(0.0, 1.0); // mean 0, stddev 1

    double Z = dist(rng); // generate a random number


    double new_price = nextPrice(asset_price, interest_rate, volatility, dt, Z);

    return 0;
}