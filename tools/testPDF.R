require(MASS)
library(fExtremes)
library(EnvStats)
library(evd)

rm(list=ls())

x <- c(173, 18, 20, 60, 99, 81, 40, 33, 27, 0)
fit1 <- fitdistr(x, "exponential")

fit_fExtremes<-gevFit(x)
#fit_fExtremes$Estimates
fit_evd<-fgev(x) # Use mle method
fit_EnvStats<-egevd(x, method = "mle")

# fit_evd
# Estimates
# loc    scale    shape  
# 32.0959  26.3801  -0.1171  

# fit_fExtremes
# Estimated Parameters:
#   xi         mu       beta 
# -0.1209737 32.2849620 26.4276733 

# fit_EnvStats This 
# location = 32.2879006
# scale    = 26.4330082
# shape    =  0.1211745

c<-rbind(c(0.012669,-0.013632,0.196362), c(-0.024849, 0.012669, -0.275052), c(-106.187921,0.196362,-6.213073))  
solve(c)

Estimated Parameters:
  xi         mu       beta 
-0.7744723 -7.0365311 85.3687702 
Estimates
loc    scale    shape  
13.9899  84.6882  -0.9961  


