#include <iostream>
#include <random>
#include <chrono>
#include <fstream> // write to csv
#include "math.h" // function declarations for math formulas

class Simulator {
    private:
        double asset_price;
        double strike_price;
        double time_to_expiration;
        double volatility;
        double interest_rate;
        int num_paths;
        int num_steps;
        double dt = time_to_expiration / num_steps;

        // Random Number Generation
        std::random_device rd;
        std::mt19937 rng; // random number generator
        std::normal_distribution<double> dist; // mean 0, stddev 1

        std::vector<double> final_prices;
        std::vector<std::vector<double>> path_data; // 2d array: [time_step][path_number]

    public:
        // Constructor to initialize random number generator
        Simulator() {
            rng = std::mt19937(rd());         
            dist = std::normal_distribution<double>(0.0, 1.0);
        }

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
        
            // Initialize path_data now that we know the dimensions
            path_data.resize(num_steps, std::vector<double>(num_paths));
        
            // Time step interval
            dt = time_to_expiration / num_steps;
        }
        
        void output_results() {
            double put_price = calculate_put_price(final_prices, strike_price, interest_rate, time_to_expiration);
            double call_price = calculate_call_price(final_prices, strike_price, interest_rate, time_to_expiration);
        
            double analytical_put = black_scholes_put(asset_price, strike_price, interest_rate, volatility, time_to_expiration);
            double analytical_call = black_scholes_call(asset_price, strike_price, interest_rate, volatility, time_to_expiration);
        
            std::cout << "\n====================== Results ======================\n";
        
            std::cout << ">> Monte Carlo Simulation (Single Threaded)\n";
            std::cout << "Estimated Put Price  : " << put_price << "\n";
            std::cout << "Estimated Call Price : " << call_price << "\n";
        
            std::cout << "\n>> Black-Scholes Analytical Solution\n";
            std::cout << "Analytical Put Price  : " << analytical_put << "\n";
            std::cout << "Analytical Call Price : " << analytical_call << "\n";
        
            std::cout << "=====================================================\n";
        }        

        void run_single_threaded_simulation() {
            // generate n number of paths where n = num_paths
            for (int i = 0; i < num_paths; i++) {
                double Z = dist(rng); // generate a random number
            
                double current_price{asset_price};

                // 1 path
                for (int j = 0; j < num_steps; j++) {
                    Z = dist(rng);
                    current_price = nextPrice(current_price, interest_rate, volatility, dt, Z);
                    path_data[j][i] = current_price;
                }
                final_prices.push_back(current_price); // add final price only
            }
        }


        void write_to_csv() {
            std::ofstream data("dist/Data.csv"); // output file stream
            
            // column headers
            data << "time_step,";

            for (int i = 1; i <= num_paths; i++) {
                data << "path_" << i;

                if (i != num_paths) {
                    data << ",";
                }
            }

            data << "\n";
            
            // adding actual data from path_data into .csv
            for (int i = 0; i < num_steps; i++) {
                data << i << ",";
                for (int j = 0; j < num_paths; j++) {
                    data << path_data[i][j];

                    if (j != num_paths - 1) {
                        data << ",";
                    }
                }
                data << "\n";
            }

        }

        void run_multi_threaded_simulation() {
            /**
             * TODO: Implement multi threaded
             */
        }
};

int main() {
    Simulator sim;
    sim.get_user_input();

    /**
     * Time how long it took to compute the price using a single thread
     */
    auto start = std::chrono::high_resolution_clock::now();
    sim.run_single_threaded_simulation();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    sim.output_results();
    std::cout << "\nSingle Threaded Time: " << elapsed.count() <<".\n";

    std::cout << "Generating visual..." << "\n";
    sim.write_to_csv();
    return 0;
}