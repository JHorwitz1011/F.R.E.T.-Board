# float lowFloat(float* x, float* y, Coeff* coeffs){
#     return (.0001)*(x[0]) + (.0002)*(x[-1]) + (.0001)*(x[-2]) - (-1.9752)*(y[-1]) - (0.9755)*(y[-2]);
# } 

import math
import numpy as np
from matplotlib import pyplot as plt

freq = 100
amplitude = 500
FS = 44100
LEN = 1
SAMPLE_SIZE = math.floor(FS*LEN)


time = np.linspace(0, LEN, SAMPLE_SIZE)
unfiltered = amplitude* np.sin(2*np.pi*freq*time) 

filtered = [0.0]*SAMPLE_SIZE
for i in range(2, len(unfiltered)):
    filtered[i] = .0001*unfiltered[i] + .0002*unfiltered[i-1] + .0001*unfiltered[i-2] - (-1.9752)*filtered[i-1] - (0.9755)*filtered[i-2]



plt.plot(time, unfiltered, label='unfiltered')
plt.plot(time, filtered, label='filtered')
plt.legend()
plt.savefig('output.png')
print(unfiltered)

# unfiltered = math.sin(input)
