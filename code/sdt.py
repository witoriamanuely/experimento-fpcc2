from datetime import datetime
from pandas import read_csv, read_fwf, DataFrame
from swinging_door import swinging_door
import numpy as np

df = DataFrame([
    {
        "Date": datetime.now(),
        "Value": value
    } 
    for value in read_fwf("calgarycorpus/bib").values.tolist()
])
CD = 0;
c = d = df['Date'][0], df['Value'][0]

u = df + (0,CD)

sUMAX = -np.inf
sUMIN = np.inf
broadcast(c,c)

def new_window():
    u = c + (0,CD)
    l = c + (0,-CD)
    su = (dy - uy)/(dx - ux)
    si = (dy - ly)/(dx - lx)
    sUMAX = su
    sUMIN = sl


def sdt(time, value):
    p = d 
    d = (time,value)
    su = (dy - uy)/(dx - ux)
    si = (dy - ly)/(dx - lx)
    if su > sUMAX:
        sUMAX = su
        if sUMAX > sUMIN:
            so = (dy - py)/(dx - px)
            cu = (uy - py + so*px - min(si)*ux)/(so-min(si))
            c = (cu, uy + min(si)*(cu-ux) - (CD/2))
            broadcast(c, c)
            new_window()
    if si < siMIN:
        siMIN = si
        if sUMax > siMIN:
            so = (dy - py)/(dx - px)
            cl = (ly - py + so*px - sUMAX*lx)/(so-sUMAX)
            c = (ci, ly + suMAX*(ci-lx) + (CD/2))
            broadcast(c, c)
            new_window()



def broadcast(array1, array2):
    shape1 = np.array(array1.shape)
    shape2 = np.array(array2.shape)
    shape_result = np.maximum(shape1, shape2)
    
    # Expande as dimensões dos arrays para ter o mesmo número de dimensões
    expanded_shape1 = np.ones(len(shape_result), dtype=int)
    expanded_shape1[:len(shape1)] = shape1
    expanded_shape2 = np.ones(len(shape_result), dtype=int)
    expanded_shape2[:len(shape2)] = shape2
    
    # Realiza o broadcast entre os arrays
    broadcasted_array1 = np.broadcast_to(array1, expanded_shape1)
    broadcasted_array2 = np.broadcast_to(array2, expanded_shape2)
    
    # Realiza a operação desejada entre os arrays broadcasted
    resultado = broadcasted_array1 + broadcasted_array2
    
    return resultado





print(list(swinging_door(df, deviation=1., mode=True)))