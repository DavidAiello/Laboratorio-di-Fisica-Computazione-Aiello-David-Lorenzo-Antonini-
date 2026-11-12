import matplotlib.pyplot as plt
import numpy as np
from math import atan, cos, sin, sqrt

t, x, v, E = np.loadtxt("moto.dat",unpack = True)

fig, axes = plt.subplots(3, 1)#, figsize=(6, 4))


#Traiettorie
axes[0].plot(t, x, 'o', label="Traiettoria")
axes[0].set_ylabel(r"x / $\mathrm{m}$")
axes[0].set_xlabel(r"t / $\mathrm{s}$")
axes[0].legend()
axes[0].grid()

# Spazio delle fasi
axes[1].plot(x,v, 'o', label = 'Spazio fasi')
axes[1].set_ylabel(r"v(t) / $\mathrm{m/s}$")
axes[1].set_xlabel(r"x(t) / $\mathrm{m}$")
axes[1].legend()
axes[1].grid()


#Energia
axes[2].plot(t,E, 'o',label = 'Energia')
axes[2].set_ylabel(r"E(t) / $\mathrm{N \cdot m}$")
axes[2].set_xlabel(r"t / $\mathrm{s}$")
axes[2].legend()
axes[2].grid()

fig.tight_layout()
plt.title("Moto di qualcosa",fontsize = 15)
plt.savefig("moto.png")
plt.show()
