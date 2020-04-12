# Python program for Kruskal's algorithm to find
# Minimum Spanning Tree of a given connected,
# undirected and weighted graph
import sys
from collections import defaultdict

# Class to represent a graph


class Graph:

    def __init__(self, vertices):
        self.V = vertices  # No. of vertices
        self.graph = []  # default dictionary
        # to store graph

    # function to add an edge to graph
    def addEdge(self, u, v, w):
        self.graph.append([u, v, w])

    # A utility function to find set of an element i
    # (uses path compression technique)
    def find(self, parent, i):
        if parent[i] == i:
            return i
        return self.find(parent, parent[i])

    # A function that does union of two sets of x and y
    # (uses union by rank)
    def union(self, parent, rank, x, y):
        xroot = self.find(parent, x)
        yroot = self.find(parent, y)

        # Attach smaller rank tree under root of
        # high rank tree (Union by Rank)
        if rank[xroot] < rank[yroot]:
            parent[xroot] = yroot
        elif rank[xroot] > rank[yroot]:
            parent[yroot] = xroot

        # If ranks are same, then make one as root
        # and increment its rank by one
        else:
            parent[yroot] = xroot
            rank[xroot] += 1

    # The main function to construct MST using Kruskal's
        # algorithm
    def KruskalMST(self):

        result = []  # This will store the resultant MST

        i = 0  # An index variable, used for sorted edges
        e = 0  # An index variable, used for result[]

        # Step 1:  Sort all the edges in non-decreasing
        # order of their
        # weight.  If we are not allowed to change the
        # given graph, we can create a copy of graph
        self.graph = sorted(self.graph, key=lambda item: item[2])

        parent = []
        rank = []

        # Create V subsets with single elements
        for node in range(self.V):
            parent.append(node)
            rank.append(0)

        # Number of edges to be taken is equal to V-1
        while e < self.V - 1:

            # Step 2: Pick the smallest edge and increment
            # the index for next iteration
            u, v, w = self.graph[i]
            i = i + 1
            x = self.find(parent, u)
            y = self.find(parent, v)

            # If including this edge does't cause cycle,
            # include it in result and increment the index
            # of result for next edge
            if x != y:
                e = e + 1
                result.append([u, v, w])
                self.union(parent, rank, x, y)
            # Else discard the edge

        def takeLen(elem):
            return elem[2]
        sorted(result, key=takeLen)

        return result


def read_input(inp_path):
    resMap = {}

    with open(inp_path, 'r') as f:
        data = f.readlines()
        N = int(data[0].strip())
        g = Graph(N)

        for row in data[1:]:
            r = row.strip().split('(')[1].split(')')[0].split(',')
            v1 = int(r[0])
            v2 = int(r[1])
            e = int(r[2])

            g.addEdge(v1, v2, e)
            resMap[e] = (v1, v2, e)
    return g, resMap


def read_result_and_match(out_path, mst,resMap):
    match = True
    with open(out_path, 'r') as f:
        data = f.readlines()

        if len(data) != len(mst):
            return False

        for i, row in enumerate(data):
            r = row.strip().split('(')[1].split(')')[0].split(',')
            v1 = int(r[0])
            v2 = int(r[1])
            e = int(r[2])

            if resMap[e] != (v1,v2,e):
                print(resMap[e], (v1, v2, e))
                return False

            if(mst[i] != [v1, v2, e] and mst[i] != [v2, v1, e]):
                print(mst[i], (v1, v2, e))
                return False

    return match


if __name__ == "__main__":

    inp = sys.argv[1]
    out = sys.argv[2]
    iteration = int(sys.argv[3])

    g, resMap = read_input(inp)
    mst = g.KruskalMST()
    if read_result_and_match(out, mst, resMap):
        print("Iteration {0} Pass".format(iteration))
    else:
        print("Iteration {0} !!!!! FAIL".format(iteration))
