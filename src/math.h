#include <cmath>
#include <vector>  // for std::vector
#include <algorithm>  // for std::max

/**
 * Mathematical functions for Monte Carlo option pricing
 * 
 * This header provides functions for:
 * - Geometric Brownian motion simulation
 * - Monte Carlo option price calculation
 * - Black-Scholes analytical pricing
 */

/**
 * Calculates next asset price using geometric Brownian motion
 * S_next = S * exp((mu - 0.5*sigma^2)*dt + sigma*sqrt(dt)*Z)
 * 
 * @param S Current asset price
 * @param mu Interest rate (drift)
 * @param sigma Volatility
 * @param dt Time step size
 * @param Z Random normal variable ~ N(0,1)
 * @return Next asset price
 */
double nextPrice(double S, double mu, double sigma, double dt, double Z);

/**
 * Calculates European Call option price using Monte Carlo method
 * Call Price = e^(-r*T) * (1/N) * Σ max(S_T - K, 0)
 * 
 * @param final_prices Vector of final asset prices from simulation
 * @param K Strike price
 * @param r Risk-free interest rate
 * @param T Time to expiration
 * @return Estimated call option price
 */
double calculate_call_price(const std::vector<double>& final_prices, double K, double r, double T);

/**
 * Calculates European Put option price using Monte Carlo method
 * Put Price = e^(-r*T) * (1/N) * Σ max(K - S_T, 0)
 * 
 * @param final_prices Vector of final asset prices from simulation
 * @param K Strike price
 * @param r Risk-free interest rate
 * @param T Time to expiration
 * @return Estimated put option price
 */
double calculate_put_price(const std::vector<double>& final_prices, double K, double r, double T);

/**
 * Standard normal cumulative distribution function
 * Uses error function approximation: Φ(x) = 0.5 * (1 + erf(x/√2))
 * 
 * @param x Input value
 * @return Cumulative probability P(Z ≤ x)
 */
double norm_cdf(double x);

/**
 * Black-Scholes analytical formula for European Call option
 * C = S*Φ(d1) - K*e^(-rT)*Φ(d2)
 * where d1 = (ln(S/K) + (r + σ²/2)*T) / (σ√T)
 *       d2 = d1 - σ√T
 * 
 * @param S Current asset price
 * @param K Strike price
 * @param r Risk-free interest rate
 * @param sigma Volatility
 * @param T Time to expiration
 * @return Analytical call option price
 */
double black_scholes_call(double S, double K, double r, double sigma, double T);

/**
 * Black-Scholes analytical formula for European Put option
 * P = K*e^(-rT)*Φ(-d2) - S*Φ(-d1)
 * where d1 = (ln(S/K) + (r + σ²/2)*T) / (σ√T)
 *       d2 = d1 - σ√T
 * 
 * @param S Current asset price
 * @param K Strike price
 * @param r Risk-free interest rate
 * @param sigma Volatility
 * @param T Time to expiration
 * @return Analytical put option price
 */
double black_scholes_put(double S, double K, double r, double sigma, double T);