#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jun  3 21:10:38 2020

@author: luis
"""

from scipy.stats import uniform, norm, lognorm, expon
import numpy as np

x = [73, 18, 20, 60, 99, 81, 40, 33, 27, 0]                                                     

a, b = uniform.fit(x)
print(a,b)
print(uniform.pdf(x, a,b))
print(uniform.cdf(x,a,b))

a, b = norm.fit(x)
print(a,b)
print(norm.pdf(x, a,b))
print(norm.cdf(x,a,b))

#a, b, s = lognorm.fit(x, floc=0)
#print(a,b,s)
#print(lognorm.pdf(x, a,b,s))
#print(lognorm.cdf(x,a,b,s))
#np.log(a), b  # mu, sigma

a = expon.fit(x)
print(a)
print(expon.pdf(x,a))
print(expon.cdf(x,a))