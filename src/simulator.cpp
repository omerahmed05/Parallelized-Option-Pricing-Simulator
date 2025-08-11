#include <iostream>
#include <random>
#include <chrono>
#include <fstream> // write to csv
#include "math.h" // function declarations for math formulas
#include <omp.h>

/**
 * Monte Carlo Option Pricing Simulator
 * Simulates asset price paths using geometric Brownian motion
 * and calculates option prices using both Monte Carlo and Black-Scholes methods
 */
class Simulator {
    private:
        // Market parameters
        double asset_price;
        double strike_price;
        double time_to_expiration;
        double volatility;
        double interest_rate;
        
        // Simulation parameters
        int num_paths;
        int num_steps;
        double dt = time_to_expiration / num_steps;

        // Random number generation
        std::random_device rd;

        // Storage for simulation results
        std::vector<double> final_prices;  // Final price of each path
        std::vector<std::vector<double>> path_data; // 2D array: [time_step][path_number]
    
    public:
        Simulator() { }

        /**
         * Collects user input for market and simulation parameters
         */
        void get_user_input() {
            std::cout << "\n=== Market Parameters ===\n";
            std::cout << "Current asset price: ";
            std::cin >> asset_price;
        
            std::cout << "Strike price: ";
            std::cin >> strike_price;
        
            std::cout << "Time to expiration (in years, e.g., 0.5 for 6 months): ";
            std::cin >> time_to_expiration;
        
            std::cout << "Volatility (as a decimal, e.g., 0.2 for 20%): ";
            std::cin >> volatility;
        
            std::cout << "Risk-free interest rate (as a decimal, e.g., 0.05 for 5%): ";
            std::cin >> interest_rate;
        
            std::cout << "\n=== Simulation Parameters ===\n";
            std::cout << "Number of simulation paths (e.g., 100000): ";
            std::cin >> num_paths;
        
            std::cout << "Number of time steps per path (max allowed: 1000): ";
            std::cin >> num_steps;
        
            if (num_steps > 1000) {
                std::cout << "Capping time steps to 1000 due to performance constraints.\n";
                num_steps = 1000;
            }
        
            // Initialize data structures
            path_data.resize(num_steps, std::vector<double>(num_paths));
            final_prices.resize(num_paths);
            dt = time_to_expiration / num_steps;
        }
        
        /**
         * Displays simulation results comparing Monte Carlo vs Black-Scholes
         */
        void output_results() {
            double put_price = calculate_put_price(final_prices, strike_price, interest_rate, time_to_expiration);
            double call_price = calculate_call_price(final_prices, strike_price, interest_rate, time_to_expiration);
        
            double analytical_put = black_scholes_put(asset_price, strike_price, interest_rate, volatility, time_to_expiration);
            double analytical_call = black_scholes_call(asset_price, strike_price, interest_rate, volatility, time_to_expiration);
        
            std::cout << "\n====================== Results ======================\n";
        
            std::cout << ">> Monte Carlo Simulation\n";
            std::cout << "Estimated Put Price  : " << put_price << "\n";
            std::cout << "Estimated Call Price : " << call_price << "\n";
        
            std::cout << "\n>> Black-Scholes Analytical Solution\n";
            std::cout << "Analytical Put Price  : " << analytical_put << "\n";
            std::cout << "Analytical Call Price : " << analytical_call << "\n";
        
            std::cout << "=====================================================\n";
        } 

        /**
         * Runs Monte Carlo simulation using single-threaded approach
         * Generates asset price paths using geometric Brownian motion
         */
        void run_single_threaded_simulation() {
            std::mt19937 rng(rd());
            std::normal_distribution<double> dist(0.0, 1.0);

            // Generate num_paths price trajectories
            for (int i = 0; i < num_paths; i++) {
                double current_price{asset_price};

                // Simulate one complete price path
                for (int j = 0; j < num_steps; j++) {
                    double Z = dist(rng);  // Random normal variable
                    current_price = nextPrice(current_price, interest_rate, volatility, dt, Z);
                    path_data[j][i] = current_price;
                }
                final_prices[i] = current_price;  // Store final price for option pricing
            }
        }

        /**
         * Runs Monte Carlo simulation using OpenMP parallelization
         * Each thread generates its own random number generator for thread safety
         */
        void run_multi_threaded_simulation() {              
            #pragma omp parallel for
            for (int i = 0; i < num_paths; i++) {
                // Thread-local random number generators for safety
                std::mt19937 local_rng(rd() + omp_get_thread_num()); 
                std::normal_distribution<double> local_dist(0.0, 1.0);

                double current_price{asset_price};

                // Simulate one complete price path
                for (int j = 0; j < num_steps; j++) {
                    double Z = local_dist(local_rng);
                    current_price = nextPrice(current_price, interest_rate, volatility, dt, Z);
                    path_data[j][i] = current_price;
                }
                final_prices[i] = current_price;
            }
        }

        /**
         * Exports simulation data to CSV file for visualization
         * Format: time column + one column per price path
         */
        void write_to_csv() {
            std::ofstream data("dist/Data.csv");
            
            // Write column headers
            data << "time,";
            for (int i = 1; i <= num_paths; i++) {
                data << "path_" << i;
                if (i != num_paths) data << ",";
            }
            data << "\n";
            
            // Write price data: each row is a time step, each column is a path
            for (int i = 0; i < num_steps; i++) {
                data << i << ",";
                for (int j = 0; j < num_paths; j++) {
                    data << path_data[i][j];
                    if (j != num_paths - 1) data << ",";
                }
                data << "\n";
            }
        }

        /**
         * Resets simulation data for multiple runs
         */
        void clear() {
            for (int i = 0; i < final_prices.size(); i++) {
                final_prices[i] = 0.0;
            }

            for (int i = 0; i < path_data.size(); i++) {
                for (int j = 0; j < path_data[i].size(); j++) {
                    path_data[i][j] = 0.0;
                }
            }
        }
};

/**
 * Main function: runs both single and multi-threaded simulations
 * and compares performance and results
 */
int main() {
    Simulator sim;
    sim.get_user_input();

    // Single-threaded simulation with timing
    auto start_single = std::chrono::high_resolution_clock::now();
    sim.run_single_threaded_simulation();
    auto end_single = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_single = end_single - start_single;

    std::cout << "\n=== SINGLE THREADED RESULTS ===\n";
    sim.output_results();
    std::cout << "\nSingle Threaded Time: " << elapsed_single.count() <<".\n";

    // Clear data for next run
    sim.clear();

    // Multi-threaded simulation with timing
    auto start_multi = std::chrono::high_resolution_clock::now();
    sim.run_multi_threaded_simulation();
    auto end_multi = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_multi = end_multi - start_multi;

    std::cout << "\n=== MULTI THREADED RESULTS ===\n";
    sim.output_results();
    std::cout << "\nMulti Threaded Time: " << elapsed_multi.count() <<".\n";

    // Performance comparison
    std::cout << "\n=== PERFORMANCE COMPARISON ===\n";
    std::cout << "Speedup: " << elapsed_single.count() / elapsed_multi.count() << "x\n";
    
    std::cout << "Generating visual..." << "\n";
    sim.write_to_csv();
    return 0;
}