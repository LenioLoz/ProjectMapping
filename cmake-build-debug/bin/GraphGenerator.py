import random

n = 80
tab = [[0 for x in range(0, n)] for i in range(0, n)]
p = 3
graph = {x: [] for x in range(n)}

# Macierz sasiedstw
for rows in range(0, n):
    for cols in range(rows, n):
        r = random.randint(0, 100)
        if r < p and rows != cols:
            tab[rows][cols] = 1
            tab[cols][rows] = 1

for i in range(n-1):
    if tab[i][i+1] == 0:
        tab[i][i+1] = 1
        tab[i+1][i] = 1

tab[n-1][0] = 1
tab[0][n-1] = 1

for i in range(n):
    lista = []
    for j in range(n):
        if i == j:
            continue
        if tab[i][j] == 1:
            lista.append(j)
    graph[i] = lista


for i in range(n):
    if len(graph.get(i)) == 0:
        l = random.randint(0, n-1)
        while l == i:
            l = random.randint(0, n-1)
        graph[i].append(l)
        graph[l].append(i)
        graph[i].sort()
        graph[l].sort()

plik = open("GraphStandard.txt", "w")

for i in graph:
    plik.write(str(i) + " ")
    plik.write("0.0" + " " + "0.0" + " ")
    for j in graph.get(i):
        plik.write(str(j) + " ")
    plik.write("\n")

plik.close()


print(graph)

