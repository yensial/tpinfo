from math import sqrt
import pandas as pd
import matplotlib.pyplot as plt

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

def moyenne(data):
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
    return n_moyen/count

def variance(data):
    m=moyenne(data)
    indice=0
    var=0
    while indice<len(data):
        nb_positions=nbpos(data,indice)
        var+=(nb_positions-m)**2
        indice+=nb_positions
    var=var/(len(data)/nb_positions)
    return var

print("Moyenne : ", moyenne(data))
print("Ecart-type : ", sqrt(variance(data)))


    