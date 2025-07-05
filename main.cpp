#include <iostream>
#include <cmath>

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
    double Z = /* generate a standard normal random number here */;

    double new_price = nextPrice(asset_price, interest_rate, volatility, dt, Z);

    return 0;
}