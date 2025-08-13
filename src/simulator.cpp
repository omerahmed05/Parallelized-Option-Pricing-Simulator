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
         * Format: time column + averaged path columns for readability
         */
        void write_to_csv() {
            std::ofstream data("dist/Data.csv");
            
            // Calculate target lines dynamically based on number of paths
            int target_lines;
            if (num_paths <= 100) {
                target_lines = num_paths;  // Show all paths for very small datasets
            } else {
                // Scale using square root: more paths = more lines, but not linearly
                target_lines = std::max(15, std::min(50, (int)std::sqrt(num_paths)));
            }
            
            int batch_size = std::max(1, num_paths / target_lines);
            int num_batches = (num_paths + batch_size - 1) / batch_size;
            
            // Write column headers
            data << "time_step,";
            for (int batch = 0; batch < num_batches; batch++) {
                int start_idx = batch * batch_size;
                int end_idx = std::min((batch + 1) * batch_size, num_paths);
                data << "avg_paths_" << (start_idx + 1) << "-" << end_idx;
                if (batch != num_batches - 1) data << ",";
            }
            data << "\n";
            
            // Write price data: each row is a time step, each column is an averaged path
            for (int i = 0; i < num_steps; i++) {
                data << i << ",";
                for (int batch = 0; batch < num_batches; batch++) {
                    int start_idx = batch * batch_size;
                    int end_idx = std::min((batch + 1) * batch_size, num_paths);
                    
                    // Calculate average of this batch at this time step
                    double sum = 0.0;
                    for (int j = start_idx; j < end_idx; j++) {
                        sum += path_data[i][j];
                    }
                    double avg = sum / (end_idx - start_idx);
                    
                    data << avg;
                    if (batch != num_batches - 1) data << ",";
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
 * Main function: gives the user the option to run the simulation with a single thread, multiple threads, or both.
 * It then runs the simulation and outputs the results.
 * It then generates the visualization data and writes it to a CSV file.
 */
int main() {
    Simulator sim;
    sim.get_user_input();

    std::cout << "Would you like to run the simulation with a single thread or multiple threads? (1 for single, 2 for multiple, 3 for both): ";
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        // Single-threaded simulation with timing
        std::cout << "Running single-threaded simulation..." << "\n";
        auto start_single = std::chrono::high_resolution_clock::now();
        sim.run_single_threaded_simulation();
        auto end_single = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_single = end_single - start_single;

        std::cout << "\n=== SINGLE THREADED RESULTS ===\n";
        sim.output_results();
        std::cout << "\nSingle Threaded Time: " << elapsed_single.count() << " seconds.\n";
        
    } else if (choice == 2) {
        // Multi-threaded simulation with timing
        std::cout << "Running multi-threaded simulation..." << "\n";
        auto start_multi = std::chrono::high_resolution_clock::now();
        sim.run_multi_threaded_simulation();
        auto end_multi = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_multi = end_multi - start_multi;

        std::cout << "\n=== MULTI THREADED RESULTS ===\n";
        sim.output_results();
        std::cout << "\nMulti Threaded Time: " << elapsed_multi.count() << " seconds.\n";
        
    } else if (choice == 3) {
        // Run both simulations for comparison
        std::cout << "Running both single and multi-threaded simulations for comparison..." << "\n";
        
        // Single-threaded simulation with timing
        auto start_single = std::chrono::high_resolution_clock::now();
        sim.run_single_threaded_simulation();
        auto end_single = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_single = end_single - start_single;

        std::cout << "\n=== SINGLE THREADED RESULTS ===\n";
        sim.output_results();
        std::cout << "\nSingle Threaded Time: " << elapsed_single.count() << " seconds.\n";

        // Clear data for next run
        sim.clear();

        // Multi-threaded simulation with timing
        auto start_multi = std::chrono::high_resolution_clock::now();
        sim.run_multi_threaded_simulation();
        auto end_multi = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_multi = end_multi - start_multi;

        std::cout << "\n=== MULTI THREADED RESULTS ===\n";
        sim.output_results();
        std::cout << "\nMulti Threaded Time: " << elapsed_multi.count() << " seconds.\n";

        // Performance comparison
        std::cout << "\n=== PERFORMANCE COMPARISON ===\n";
        std::cout << "Speedup: " << elapsed_single.count() / elapsed_multi.count() << "x\n";
        
    } else {
        std::cout << "Invalid choice. Please enter 1, 2, or 3." << "\n";
        return 1;
    }

    // Generate visualization data
    std::cout << "Generating visualization data..." << "\n";
    sim.write_to_csv();
    std::cout << "Simulation complete! Check 'dist/Data.csv' for visualization data.\n";
    
    return 0;
}