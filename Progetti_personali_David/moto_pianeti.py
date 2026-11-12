import matplotlib.pyplot as plt
import numpy as np
t = []
x = []
y = []
vx = []
vy = []
periodi = []
def T(x,t):
    zeros = []
    periodi = []
    for i in range(len(x)-1):
        if x[i] * x[i+1] < 0:
            m_inverso = (t[i+1] - t[i]) / (x[i+1]- x[i]) 
            zeros.append(-(x[i]* m_inverso)+t[i])
    for t1,t2 in zip(zeros[:-2],zeros[2:]) :
        periodi.append((t2-t1))
    return periodi

def Ampiezza(x, y):
    r = np.sqrt(x**2 + y**2)
    massimi = []
    minimi = []
    
    # Dal secondo al penultimo elemento per non uscire dai bordi
    for i in range(1, len(r) - 1):
        if r[i] > r[i-1] and r[i] > r[i+1]:
            massimi.append(r[i])
        if r[i] < r[i-1] and r[i] < r[i+1]:
            minimi.append(r[i])
            
    # Caso cerchio perfetto (nessun picco)
    if len(massimi) == 0:
        return np.mean(r)
        
    # Semiasse maggiore = media tra afelio e perielio
    return (np.mean(massimi) + np.mean(minimi)) / 2

for i in range(10):
    ti,xi,yi,vxi,vyi = np.loadtxt(f"pianeti{i}.dat", unpack = True)
    t.append(ti)
    x.append(xi)
    y.append(yi)
    vx.append(vxi)
    vy.append(vyi)
    periodi.append(T(xi,ti))
media_periodi = []
for j in range(10):
    media_periodi.append(np.mean(periodi[j]))
    print(f"periodo_{j} = {media_periodi[j]}")
fig,axes = plt.subplots(2,1)
#traiettoria
for i in range(10):
    axes[0].plot(x[i],y[i],label = f"Traiettoria pianeta_{i}")
    #axes[0].plot(t[i],x[i],label = "Traiettoria x")
    axes[0].set(title = "Traiettoria pianeti", xlabel = "Asse x (m)", ylabel = "Asse y (m)")
    #axes[0].set_xlim(-3, 3)
    #axes[0].set_ylim(-3, 3)
    #axes[0].set_aspect('equal') 
    axes[0].plot(0, 0, 'yo', markersize=10)
    axes[0].legend()
    #Spazioe delle fasi
    axes[1].plot(x[i],vx[i], label = f"Fasi x_{i}")
    #axes[1].plot(y[i],vy[i], label = f"Fasi y_{i}")
    axes[1].set(title = "Spazio delle fasi", xlabel = "Asse x (m)", ylabel = "Asse y (m)")
    #axes[1].set_xlim(-3, 3)
    #axes[1].set_ylim(-3, 3)
   # axes[1].set_aspect('equal') # Forza 1 unità su X = 1 unità su Y
    #axes[1].plot(0, 0, 'yo', markersize=10, label="Sole")
    axes[1].legend()
plt.savefig("pianeti.png")
plt.tight_layout()
plt.show()
