from datetime import datetime
from pandas import read_csv, read_fwf, DataFrame
from swinging_door import swinging_door


df = DataFrame([
    {
        "Date": datetime.now(),
        "Value": value
    } 
    for value in read_fwf("calgarycorpus/bib").values.tolist()
])

print(df)

print(list(swinging_door(df, deviation=1., mode=True)))
[68.27, 99.05, 96.20, 72.73, 71.10, 63.12, 69.59,47.64, 70, 74.68, 63.72, 99, 80.26, 65.47]
[68.87, 68, 99.7, 98, 73, 70.7, 62.7, 69.3, 47.95, 69.85, 64.27, 74.58, 64.14, 63.3, 67.2, 100, 80, 65]