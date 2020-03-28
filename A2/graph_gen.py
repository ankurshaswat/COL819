import numpy as np
import random

#PRE: V for the number of vertices
#POST: creates a random connected graph with a V-1 edges

def generateRandomConnectedGraph(V):

    initialSet = set()

    visitedSet = set()

    vertices = set()

    edges = set()

    #generate the set of names for the vertices

    for i in range(V):

        initialSet.add(str(i))

        vertices.add(str(i))

    #set the intial vertex to be connected

    curVertex = random.sample(initialSet, 1).pop()

    initialSet.remove(curVertex)

    visitedSet.add(curVertex)

    #loop through all the vertices, connecting them randomly

    while initialSet:

        adjVertex = random.sample(initialSet, 1).pop()

        edge = (0, curVertex, adjVertex)

        edges.add(edge)

        initialSet.remove(adjVertex)

        visitedSet.add(adjVertex)

        curVertex = adjVertex

    return vertices, edges

#PRE: the number of elements in a list
#POST: generate a 2D array of all combinations
def generateCombination(n):

    com = []

    for i in range(n):

        for j in range(i+1, n):

            com.append([i,j])

            com.append([j,i])

    return com

if __name__ == "__main__":
    
    num_nodes = 100
    print(num_nodes)

    # print(generateRandomConnectedGraph(100))
    g = generateRandomConnectedGraph(num_nodes)
    

    num_edges = len(g[1])

    # assert((num_nodes*(num_nodes-1)/2) >= num_edges)

    lens = list(range(1,num_edges+1))
    random.shuffle(lens)

    for i,item in enumerate(g[1]):
        if(item[1] < item[2]):
            print("(%s, %s, %d)"%(item[1],item[2],lens[i]))

    # print(num_nodes)
    # # print(lens)

    # a = np.zeros((num_nodes,num_nodes))

    # num_changed = 0

    # while num_changed<num_edges:
    #     i = random.randint(0,num_nodes-1)
    #     j = random.randint(0,num_nodes-1)

    #     if i==j:
    #         continue
    #     if a[i][j] == 1:
    #         continue

    #     a[i][j] = 1
    #     print("(%d, %d, %d)"%(i,j,lens[num_changed]))
    #     num_changed += 1