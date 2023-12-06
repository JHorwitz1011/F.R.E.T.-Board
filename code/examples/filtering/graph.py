import csv
from matplotlib import pyplot as plt 

file = open("output.csv")

csvreader = csv.reader(file)
header = []
header = next(csvreader)

def validRow(row):
    counter = 0
    for element in row:
        if float(element) > 0:
            counter += 1

    if counter > 1:
        # print('true!')
        return True
    else:
        return False

freq = []

fixedResponses = {
    "low": [],
    "mid1": [],
    "mid2": [],
    "mid3": [],
    "high": [],
    "full": []
}

floatResponses = {
    "low": [],
    "mid1": [],
    "mid2": [],
    "mid3": [],
    "high": [],
    "full": []
}

AMPLITUDE = 9500*2

for row in csvreader:
    if validRow(row):
        freq.append(row[0])
        fixedResponses["low"].append(float(row[1])/(AMPLITUDE))
        fixedResponses["mid1"].append(float(row[3])/(AMPLITUDE))
        fixedResponses["mid2"].append(float(row[5])/(AMPLITUDE))
        fixedResponses["mid3"].append(float(row[7])/(AMPLITUDE))
        fixedResponses["high"].append(float(row[9])/(AMPLITUDE))
        fixedResponses["full"].append(float(row[11])/(AMPLITUDE))

        floatResponses["low"].append(float(row[2])/(AMPLITUDE))
        floatResponses["mid1"].append(float(row[4])/(AMPLITUDE))
        floatResponses["mid2"].append(float(row[6])/(AMPLITUDE))
        floatResponses["mid3"].append(float(row[8])/(AMPLITUDE))
        floatResponses["high"].append(float(row[10])/(AMPLITUDE))
        floatResponses["full"].append(float(row[12])/(AMPLITUDE))
        
# print(fixedResponses)
# print(floatResponses)

mid1 = max(fixedResponses['mid1'])
mid2 = max(fixedResponses['mid2'])
mid3 = max(fixedResponses['mid3'])

highFlag = False
lowFlag = False

for i in range(10,len(fixedResponses['low'])):
    if mid1 == fixedResponses['mid1'][i]:
        print(f"MID1 @ {freq[i]}")
    if mid2 == fixedResponses['mid2'][i]:
        print(f"MID2 @ {freq[i]}")
    if mid3 == fixedResponses['mid3'][i]:
        print(f"MID3 @ {freq[i]}")

    if fixedResponses['high'][i] > 0.707 and not highFlag:
        print(f"high PASS 3DB AT {freq[i]}")
        highFlag = True
    if fixedResponses['low'][i] < 0.707 and not lowFlag:
        print(f"low PASS 3DB AT {freq[i]}")
        lowFlag = True



# print full response
plt.figure(figsize=(18, 10))
plt.title('full filter curve, fixed vs float')
plt.plot(freq, fixedResponses['full'], label='fixed')
plt.plot(freq, floatResponses['full'], label='float')
plt.legend()
plt.xscale('log')
# plt.yscale('log')
plt.ylabel('amplitude (log)')
plt.xlabel('freq (log)')
plt.savefig('fullFilterBoth.png')
plt.clf()

# print full response fixed only
plt.figure(figsize=(18, 10))
plt.title('full filter curve, fixed')
plt.plot(freq, fixedResponses['full'], label='fixed')
plt.legend()
plt.xscale('log')
plt.yscale('log')
plt.ylabel('amplitude (log)')
plt.xlabel('freq (log)')
plt.savefig('fullFilterFixed.png')
plt.clf()

# print full response float only
plt.figure(figsize=(18, 10))
plt.title('full filter curve float')
plt.plot(freq, floatResponses['full'], label='float')
plt.legend()
plt.xscale('log')
plt.yscale('log')
plt.ylabel('amplitude (log)')
plt.xlabel('freq (log)')
plt.savefig('fullFilterFloat.png')
plt.clf()


# print individual repsonses
plt.figure(figsize=(18, 10))
plt.title('individual responses filter curve, fixed and float')
plt.plot(freq, fixedResponses['low'], label='fixedL')
plt.plot(freq, floatResponses['low'], label='floatL')
plt.plot(freq, fixedResponses['high'], label='fixedH')
plt.plot(freq, floatResponses['high'], label='floatH')
plt.plot(freq, fixedResponses['mid1'], label='fixedM1')
plt.plot(freq, floatResponses['mid1'], label='floatM1')
plt.plot(freq, fixedResponses['mid2'], label='fixedM2')
plt.plot(freq, floatResponses['mid2'], label='floatM2')
plt.plot(freq, fixedResponses['mid3'], label='fixedM3')
plt.plot(freq, floatResponses['mid3'], label='floatM3')
plt.legend()
plt.xscale('log')
# plt.yscale('log')
plt.ylabel('amplitude (log)')
plt.xlabel('freq (log)')
plt.savefig('individualResponsesBoth.png')
plt.clf()

# print individual repsonses, fixed only
plt.figure(figsize=(18, 10))
plt.title('individual responses filter curve, fixed only')
plt.plot(freq, fixedResponses['low'], label='fixedL')
plt.plot(freq, fixedResponses['high'], label='fixedH')
plt.plot(freq, fixedResponses['mid1'], label='fixedM1')
plt.plot(freq, fixedResponses['mid2'], label='fixedM2')
plt.plot(freq, fixedResponses['mid3'], label='fixedM3')
plt.plot(freq, fixedResponses['full'], label='fixedFull')
plt.legend()
plt.xscale('log')
plt.yscale('log')
plt.ylabel('amplitude (log)')
plt.xlabel('freq (log)')
plt.savefig('individualResponsesFixed.png')
plt.clf()

# print individual repsonses, float only
plt.figure(figsize=(18, 10))
plt.title('individual responses filter curve, float only')
plt.plot(freq, floatResponses['low'], label='floatL')
plt.plot(freq, floatResponses['high'], label='floatH')
plt.plot(freq, floatResponses['mid1'], label='floatM1')
plt.plot(freq, floatResponses['mid2'], label='floatM2')
plt.plot(freq, floatResponses['mid3'], label='floatM3')
plt.legend()
plt.xscale('log')
plt.yscale('log')
plt.ylabel('amplitude (log)')
plt.xlabel('freq (log)')
plt.savefig('individualResponsesFloat.png')
plt.clf()

