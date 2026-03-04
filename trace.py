import matplotlib.pyplot as plt
import pandas as pd

FILEPATH="dat\SlowingDownNeutron_trajectory.dat"

data = pd.read_csv(FILEPATH)

def nbpos(data,indice):
    nb_positions=0
    for i in data['#n'][indice:]:
        if i>nb_positions:
            nb_positions=i
        if i<nb_positions:
            break
    return nb_positions

nb_positions=nbpos(data,0)
x=data['x'].values[:nb_positions]
y=data['y'].values[:nb_positions]

plt.figure(figsize=(10,10))
plt.title("Neutron Trajectories in a Non-Absorbing Moderator", fontsize=16)
plt.xlabel("x (cm)", fontsize=14)
plt.ylabel("y (cm)", fontsize=14)
plt.plot(x, y, marker='o', markersize=2, linewidth=1)
plt.grid()
plt.axis('equal')
plt.show()


def plot_n_repartition(data):
    n_moyen=0
    i=0
    count=0
    n_rep=dict()
    while i<len(data):
        nb_positions=nbpos(data,i)
        n_rep[nb_positions]=n_rep.get(nb_positions,0)+1
        n_moyen+=nb_positions
        i+=nb_positions
        count+=1

    print(f"Nombre moyen de positions: {n_moyen/count:.2f}")
    plt.figure(figsize=(10,6))
    plt.title(f"Répartition du nombre de chocs avec {count:.2d}", fontsize=16)
    plt.xlabel("Nombre de chocs", fontsize=14)
    plt.ylabel("Fréquence", fontsize=14)
    plt.bar(n_rep.keys(), n_rep.values())
    plt.grid()
    plt.show()

plot_n_repartition(data)