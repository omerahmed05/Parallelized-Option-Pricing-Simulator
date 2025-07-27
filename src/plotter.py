import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('dist/Data.csv')

# Create a pandas series for each column 
# A Pandas Series is a mapping where each value in a row / column gets an index assigned to it
for column in df.columns:
    if column != "time":
        plt.plot(df["time"], df[column], label=column) # each value in time is the x-value, each value in current column is the y-value 
                                                            # label each point to differentiate between paths

plt.xlim(0, df["time"].iloc[-1]) # scale our axis from 0 to the last value in the time series column 
                                                            
plt.xlabel("Time (in years)")
plt.ylabel("Asset Price") 
plt.title("Simulated Option Price Paths Over Time")

plt.tight_layout()
plt.show()

print(df.head())