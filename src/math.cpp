#include "math.h"

/**
 * Implementation of mathematical functions for Monte Carlo option pricing
 * 
 * This file contains the core mathematical operations:
 * - Geometric Brownian motion simulation
 * - Monte Carlo option pricing calculations
 * - Black-Scholes analytical formulas
 */

/**
 * Calculates next asset price using geometric Brownian motion
 * Implements the stochastic differential equation:
 * S_next = S * exp((mu - 0.5*sigma^2)*dt + sigma*sqrt(dt)*Z)
 */
double nextPrice(double S, double mu, double sigma, double dt, double Z) {
    double drift = (mu - 0.5 * sigma * sigma) * dt;
    double diffusion = sigma * std::sqrt(dt) * Z;
    
    return S * std::exp(drift + diffusion);
}

/**
 * Calculates European Call option price using Monte Carlo method
 * Averages the discounted payoffs across all simulation paths
 */
double calculate_call_price(const std::vector<double>& final_prices, double K, double r, double T) {
    double payoff_sum = 0.0;
    int N = final_prices.size();
    
    // Sum up call option payoffs: max(S_T - K, 0)
    for (double S_T : final_prices) {
        payoff_sum += std::max(S_T - K, 0.0);
    }
    
    // Apply discounting and averaging
    return std::exp(-r * T) * (payoff_sum / N);
}

/**
 * Calculates European Put option price using Monte Carlo method
 * Averages the discounted payoffs across all simulation paths
 */
double calculate_put_price(const std::vector<double>& final_prices, double K, double r, double T) {
    double payoff_sum = 0.0;
    int N = final_prices.size();
    
    // Sum up put option payoffs: max(K - S_T, 0)
    for (double S_T : final_prices) {
        payoff_sum += std::max(K - S_T, 0.0);
    }
    
    // Apply discounting and averaging
    return std::exp(-r * T) * (payoff_sum / N);
}

/**
 * Standard normal cumulative distribution function
 * Uses error function approximation for efficiency
 */
double norm_cdf(double x) {
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

/**
 * Black-Scholes analytical formula for European Call option
 * Provides exact theoretical price for comparison with Monte Carlo
 */
double black_scholes_call(double S, double K, double r, double sigma, double T) {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    
    return S * norm_cdf(d1) - K * std::exp(-r * T) * norm_cdf(d2);
}

/**
 * Black-Scholes analytical formula for European Put option
 * Provides exact theoretical price for comparison with Monte Carlo
 */
double black_scholes_put(double S, double K, double r, double sigma, double T) {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    
    return K * std::exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
}