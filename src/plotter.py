import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('dist/Data.csv')

for column in df.columns:
    if column != "time_step":
        plt.plot(df["time_step"], df[column], label=column) # plot each point, with time_step being the x axis and the y axis being the price path 
                                                            # label each point to differentiate between paths
                                                            
plt.xlabel("Time Step")
plt.ylabel("Asset Price")
plt.title("Simulated Option Price Paths Over Time")

plt.tight_layout()
plt.show()

print(df.head())