# k nearest neighbors

import math
import matplotlib

# CHECKED
def normalize(X):
    AttributeCount = len(X[0])
    Size = len(X)
    for i in range(AttributeCount):
        Max = X[0][i]
        for j in range(1, Size):
            if X[j][i] > Max: Max = X[j][i]
        Min = X[0][i]
        for j in range(1, Size):
            if X[j][i] < Min: Min = X[j][i]
        for j in range(Size):
            X[j][i] = float((X[j][i] - Min + 0.0) * 1.0/(Max - Min))

# CHECKED
def distance(x, y):
    global w
    res = 0.0
    for i in range(len(x)):
        res += w[i] * math.pow(x[i] - y[i], 2)
    res = math.sqrt(res)
    return res

# CHECKED
def kMinDistance(X, x, k = 1):
    global w
    AttributeCount = len(X[0])
    Size = len(X)
    Distance = []
    for y in X:
        Distance.append(distance(x, y))

    print("Distance")
    print(Distance)

    Index = []
    q = 0
    while q < k:
        ind = -1
        for i in range(len(Distance)):
            if (not (i in Index)) and (ind == -1 or Distance[i] < Distance[ind]): ind = i
        Index.append(ind)
        q += 1
    return Index


# CHECKED
def voting(Index, y, nClasses):
    Count = [0 for x in range(nClasses)]
    for ind in Index:
        Count[y[ind]] += 1
    print("Count")
    print(Count)
    MaxInd = 0
    for x in range(1, nClasses):
        if Count[x] > Count[MaxInd]: MaxInd = x
    return MaxInd


def Answer(X, y, TestData, k = 1):
    #length = len(TestData)
    global nClasses
    answer = []
    Index = []
    for x in TestData:
        Index = kMinDistance(X, x, k)
        answer.append(voting(Index, y, nClasses))
    return answer

def score(X, y, TestData, yTestData):
    answer = Answer(X, y, TestData)
    right = 0.0
    for i in range(len(answer)):
        if answer[i] == yTestData[i]: right += 1
    return right * 1.0/len(answer)


def fit(X):
    normalize(X)

def Print(X):
    for x in X:
        print(x)

w = [1, 1]

X = [
    [1, 1],
    [2, 2],
    [2, 3],
    [3, 1],
    [3, 2],
    [3, 3],
    [4, 2],
    [4, 3],

    [10, 7],
    [11, 8],
    [11, 9],
    [11, 10],
    [12, 8],
    [12, 9],
    [13, 9],

    [1, 9],
    [2, 8],
    [2, 9],
    [2, 10],
    [3, 7]
]

nClasses = 3

y = []
for x in range(8):
    y.append(0)

for x in range(8, 15):
    y.append(1)

for x in range(15, 20):
    y.append(2)


w = [1 for x in range(len(X[0]))] # default


print(Answer(X, y, [[12, 25]], 3))