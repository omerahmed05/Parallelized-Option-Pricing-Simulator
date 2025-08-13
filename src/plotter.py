import pandas as pd
import plotly.graph_objects as go

df = pd.read_csv('dist/Data.csv')

fig = go.Figure()

for column in df.columns:
    if column != "time_step":
        fig.add_trace(go.Scatter(x=df["time_step"], y=df[column], mode='lines', name=column))

fig.update_layout(
    title="Simulated Option Price Paths Over Time (Averaged for readability)",
    xaxis_title="Time Steps",
    yaxis_title="Asset Price"
)

fig.write_html("dist/plot.html")
print("Interactive plot saved to dist/plot.html")
