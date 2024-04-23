#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 23 16:21:57 2024

@author: samarth
"""

import pandas as pd

df = pd.read_csv('./lb_benchmarks_for_mk/0.csv', skiprows=19)

D=3
N= len(df)
num_bonds = (df['attachments'].sum())/2

filename = 'test_config.csv'
f = open(filename, "w")

f.write(str(D)+"\n")
f.write("0 67 0 67 0 67\n")
f.write(str(N)+"\n")

for i in range(len(df)):
    
    row = df.iloc[i]
    f.write('{} {} {} {}\n'.format(int(row['id']), row['x0'], row['x1'], row['x2']))
    
f.write(str(int(num_bonds))+'\n')

for i in range(len(df)):
    
    row = df.iloc[i]
    col = 'att_'
    
    atts = int(row['attachments'])
    
    for j in range(atts):
        
        neigh = row[col+str(1+j)]
        
        if i < neigh:
            f.write('{} {}\n'.format(int(i), int(neigh)))


f.close()

