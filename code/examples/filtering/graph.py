import csv
from matplotlib import pyplot as plt 

file = open("output.csv")

csvreader = csv.reader(file)
header = []
header = next(csvreader)

x = []
y = []

for row in csvreader:
    x.append(row[0])
    y.append(row[1])

plt.title('mid3 pass amplitude response')
plt.plot(x,y)
plt.xscale('log')
plt.yscale('log')
plt.ylabel('amplitude (log)')
plt.xlabel('freq (log)')
print('showing!')
plt.savefig('mid3pass.png')


