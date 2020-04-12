import numpy as np
import random
import sys
# PRE: V for the number of vertices
# POST: creates a random connected graph with a V-1 edges


def generateRandomConnectedGraph(V):

    initialSet = set()
    visitedSet = set()
    vertices = set()
    edges = set()

    # generate the set of names for the vertices
    for i in range(V):
        initialSet.add(i)
        vertices.add(i)

    # set the intial vertex to be connected
    curVertex = random.sample(initialSet, 1).pop()
    initialSet.remove(curVertex)
    visitedSet.add(curVertex)

    # loop through all the vertices, connecting them randomly
    while initialSet:
        adjVertex = random.sample(initialSet, 1).pop()
        edge = (0, curVertex, adjVertex)
        edges.add(edge)
        initialSet.remove(adjVertex)
        visitedSet.add(adjVertex)
        curVertex = adjVertex

    return vertices, edges


if __name__ == "__main__":

    N = random.randrange(50, 100+1)
    if len(sys.argv) > 1:
        N = int(sys.argv[1])

    max_edges = int((N*(N-1))/2)
    M = random.randrange(N-1, max_edges)
    if len(sys.argv) > 2:
        M = int(sys.argv[2])
        if M > max_edges:
            M = max_edges

    print(N)
    g = generateRandomConnectedGraph(N)

    while len(g[1]) < M:
        v1 = random.randint(0, N-1)
        v2 = random.randint(0, N-1)

        if v1 == v2 or (0, v1, v2) in g[1] or (0, v2, v1) in g[1]:
            continue

        g[1].add((0, v1, v2))

    lens = list(range(1, M+1))
    random.shuffle(lens)

    for i, item in enumerate(g[1]):
        print("(%s, %s, %d)" % (item[1], item[2], lens[i]))
