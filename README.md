# OpenMP Parallel Financial Monte Carlo Simulator

## Summary
This project implements a Monte Carlo simulation engine in C++ using OpenMP to price options. In this project, we use the Black-Scholes model as a framework to describe how asset prices behave over time. It provides the mathematical assumptions we rely on, such as constant volatility, no arbitrage, and asset prices following a geometric Brownian motion.

We then apply a Monte Carlo simulation engine to estimate the future price of the asset based on these assumptions. 
   - The Black-Scholes framework gives us a formula that models how the asset price evolves step-by-step, including a random variable Z that represents unpredictable market movements.
   - The Monte Carlo engine simulates many possible price paths by generating different values for ZZ at each step.
   - Each simulation gives us one possible future path for the asset price.
   - We repeat this simulation many times (thousands or more), producing a wide range of potential futures.

By averaging the option payoff across all of these paths, we estimate the expected value of the option today.

## Technologies
- C++
- OpenMP
- Python (for visualization)
- Makefile

## Background
### What are financial deratives?
Financial derivatives is a contract between two parties whose value is derived from an underlying asset (such as a stock, bond, or commodity). In some types like futures and forwards, one party agrees to buy an asset at a predetermined price and future date. Put simply, it can be like a bet, but often serves other purposes like risk management. The parties may have different motivations. One might bet that the asset will increase in value, while the other bets that the asset will decrease in value. If the asset depreciates below the agreed price, then the seller benefits because they sold it at a higher price than current market value. If the asset increases in value above the agreed price, then the buyer benefits because they secured it at a lower price than current market value. The special thing about derivatives is that it can be used as a trading tool, which is a key factor. 

#### How can it be used to manage risks?
Imagine a boxing match is announced 6 months from today. You can:
Option 1 (No Derivative): Wait until the day of the fight and buy tickets at whatever price they are selling for then - could be cheap if nobody wants to go or extremely expensive if sold out.
Option 2 (Futures Contract): By the ticket today at a fixed price of $100, locking in the price.

By going with option 2, you might be worried that the price of the ticket will skyrocket in the future, so you lock in your price by paying now at today's price. However, if the ticket's value depreciates, you end up paying more.

### What is the Monte Carlo Simulation?

Monte Carlo Simulation is a way to estimate the answer to a complex problem by using random numbers and repeating an experiment many times. For example, let’s say we want to calculate the area of a circle, but we don’t know the formula A=πr^2.

Instead, we draw a rectangle that completely contains the circle. Since we know how to calculate the area of a rectangle (length × width), we generate random points inside the rectangle. Some of those points will fall inside the circle, and some will fall outside. We use the ratio of points that fall inside the circle vs points that fall outside the circle and multiply it by the area of the rectangle. We know that a point has landed inside the circle using the formula: x² + y² ≤ 1. The more points we generate, the better our estimate becomes.

\[
\frac{\text{Points inside circle}}{\text{Total points}} \times \text{Area of rectangle} \approx \text{Area of the circle}
\]

#### Estimating Pi with Monte Carlo

One of the simplest and most elegant uses of Monte Carlo simulation is estimating the value of π using the unit square.

![alt text](images/step_formula.png)

##### Unit Square  
A square with side length 1, giving an area of 1.
![alt text](images/unit_square)

##### Quarter Circle  
A quarter of a unit circle (radius = 1) is inscribed within the square.  
- Full circle area: \( \pi \times r^2 = \pi \times 1^2 = \pi \)  
- Quarter circle area: \( \frac{\pi}{4} \)

##### Random Sampling  
Random points are generated within the unit square. Each point has an \( x \) and \( y \) coordinate between 0 and 1.

A point \( (x, y) \) falls inside the quarter circle if:
![alt text](images/circle.png)
This condition comes from the equation of a circle centered at the origin with radius 1.

Because the quarter circle is fully contained within the square, the ratio of points that fall inside the circle to the total number of points approximates the ratio of their areas:
![alt text](images/ratio.png)

Solving for π gives the estimation formula:
![alt text](images/pi.png)

The accuracy of the estimate improves as the number of sampled points increases.

The code for this is provided in the root directory and can be ran with `g++ estimating_pi.cpp`; `./a.out`.

### What is the Black-Scholes Model?
The Black-Scholes Model is a mathematical formula used to calculate option values. In reality, stock prices are chaotic—influenced by news, emotions, earnings, wars, etc. The model simplifies this by assuming prices follow **geometric Brownian motion**: random evolution with constant drift and volatility.

#### How does this project use Monte Carlo?
While Black-Scholes provides closed-form solutions for simple European options, it has significant limitations, such as unrealistic assumptions (constant volatility, no dividends, no transaction costs).

Our Monte Carlo approach uses the **same mathematical foundation** (geometric Brownian motion) but **simulates** the process instead of using analytical formulas (simply plugging in values). This allows us to price complex derivatives that cannot be accurately estimated by the closed-form Black Scholes formula.

The discrete step formula we use:
![alt text](images/step_formula.png)

This simulates the random walk step-by-step, then averages thousands of paths to estimate option values. 

### Putting it all together
Our Monte Carlo simulation engine helps us estimate the value of options by simulating many possible future price scenarios for the underlying asset.

Here’s how it works:

1. Start with today’s asset price.

2. Use the Black-Scholes assumptions to model how the price might change over small time steps into the future.
   This includes factors like volatility (how much the price can fluctuate) and expected return.

3. At each small time step, we simulate how the price might change using this formula:

   ![alt text](images/step_formula.png)

   ![alt text](images/parameters.png)

   \( Z \) represents the random shock to the asset price at that step. Most of the time, it's near 0 (small price changes), but sometimes it’s larger, causing bigger fluctuations.

4. Repeat this step-by-step price update many times to create one possible "path" of how the price might move over time (this is where the multithreading optimization comes in).

5. Simulate thousands or millions of these paths. Each one represents a different possible future for the asset price, capturing the uncertainty and randomness of the market.

6. For each simulated path, calculate the payoff of the option (how much it would be worth at expiration).

7. Average all those payoffs together to estimate the expected value of the option today.

## Evaluating the model
We will evaluate the Monte Carlo simulation engine in two key ways:
   1. Performance Comparison: Measure the speedup achieved by parallelizing the simulation with OpenMP by comparing the run time of the single-threaded and multi-threaded versions.
   2. Accuracy Check: Compare the option price estimated by our Monte Carlo simulation against the price calculated using the Black-Scholes closed-form analytical formula (when applicable). This helps validate the correctness of our simulation under ideal conditions.

## User Input
Current asset price (S₀) -> the price of the underlying asset today
Strike price (K) -> the price at which the option can be exercised
Time to expiration (T) -> usually in years (e.g., 0.5 for 6 months)
Volatility (σ) -> annualized standard deviation of the asset’s returns (e.g., 0.2 for 20%)
Risk-free interest rate (r) -> annualized risk-free rate (e.g., 0.05 for 5%)
Number of simulation paths -> how many random price paths to generate (more paths → more accurate results but slower)
Number of time steps per path -> how many small intervals to split the total time T into (e.g., 252 for daily steps in a year). When simulating a price path from today until option expiration, you break the total time period into small intervals, called time steps. Instead of jumping directly from the start price to the end price in one go, you simulate the price step-by-step, moving forward a little bit at a time.