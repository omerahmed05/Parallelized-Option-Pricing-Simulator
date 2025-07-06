#include "math.h"

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
 * Helper method to calculate Call option price from final asset prices
 * Call Price = e^(-r*T) * (1/N) * Σ max(S_T - K, 0)
 * 
 * K = Strike price
 * R = Risk-free interest rate
 * T = Time to expiration
 */
double calculate_call_price(const std::vector<double>& final_prices, double K, double r, double T) {
    double payoff_sum = 0.0;
    int N = final_prices.size();
    
    // Calculate Σ max(S_T - K, 0)
    for (double S_T : final_prices) {
        payoff_sum += std::max(S_T - K, 0.0);
    }
    
    // Apply the formula: e^(-r*T) * (1/N) * Σ max(S_T - K, 0)
    return std::exp(-r * T) * (payoff_sum / N);
}

/**
 * Helper method to calculate Put option price from final asset prices
 * Put Price = e^(-r*T) * (1/N) * Σ max(K - S_T, 0)
 */
double calculate_put_price(const std::vector<double>& final_prices, double K, double r, double T) {
    double payoff_sum = 0.0;
    int N = final_prices.size();
    
    // Calculate Σ max(K - S_T, 0)
    for (double S_T : final_prices) {
        payoff_sum += std::max(K - S_T, 0.0);
    }
    
    // Apply the formula: e^(-r*T) * (1/N) * Σ max(K - S_T, 0)
    return std::exp(-r * T) * (payoff_sum / N);
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