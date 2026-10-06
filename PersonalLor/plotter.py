import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('FreeFall.dat')
print(df)
df.plot()
plt.show()