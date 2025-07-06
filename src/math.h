#include <cmath>
#include <vector>  // for std::vector
#include <algorithm>  // for std::max

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
double nextPrice(double S, double mu, double sigma, double dt, double Z);

/**
 * Helper method to calculate Call option price from final asset prices
 * Call Price = e^(-r*T) * (1/N) * Σ max(S_T - K, 0)
 * 
 * K = Strike price
 * R = Risk-free interest rate
 * T = Time to expiration
 */
double calculate_call_price(const std::vector<double>& final_prices, double K, double r, double T);

/**
 * Helper method to calculate Put option price from final asset prices
 * Put Price = e^(-r*T) * (1/N) * Σ max(K - S_T, 0)
 */
double calculate_put_price(const std::vector<double>& final_prices, double K, double r, double T);

/**
 * Analytical Black Scholes Formula
 */
// Standard normal cumulative distribution function (CDF)
// Using the error function erf for approximation
double norm_cdf(double x);

// Black-Scholes formula for European Call Option price
double black_scholes_call(double S, double K, double r, double sigma, double T);

// Black-Scholes formula for European Put Option price
double black_scholes_put(double S, double K, double r, double sigma, double T);