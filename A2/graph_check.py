import sys


class Graph:

    # init function to declare class variables
    def __init__(self, V):
        self.V = V
        self.adj = [[] for i in range(V)]

    def DFSUtil(self, temp, v, visited):

        # Mark the current vertex as visited
        visited[v] = True

        # Store the vertex to list
        temp.append(v)

        # Repeat for all vertices adjacent
        # to this vertex v
        for i in self.adj[v]:
            if visited[i] == False:

                # Update the list
                temp = self.DFSUtil(temp, i, visited)
        return temp

    # method to add an undirected edge
    def addEdge(self, v, w):
        self.adj[v].append(w)
        self.adj[w].append(v)

    # Method to retrieve connected components
    # in an undirected graph
    def connectedComponents(self):
        visited = []
        cc = []
        for i in range(self.V):
            visited.append(False)
        for v in range(self.V):
            if visited[v] == False:
                temp = []
                cc.append(self.DFSUtil(temp, v, visited))
        return cc


if __name__ == "__main__":

    g = {}
    gr = None
    weights = []
    edges_list = []
    path = sys.argv[1]

    with open(path, 'r') as f:
        data = f.readlines()
        numNodes = int(data[0].strip())
        gr = Graph(numNodes)

        for row in data[1:]:
            r = row.strip().split('(')[1].split(')')[0].split(',')
            v1 = int(r[0])
            v2 = int(r[1])
            e = int(r[2])

            if v1 not in g:
                g[v1] = []
            g[v1].append(v2)

            if v2 not in g:
                g[v2] = []
            g[v2].append(v1)

            gr.addEdge(v1, v2)
            weights.append(e)

            if v1 < v2:
                edges_list.append((v1, v2))
            else:
                edges_list.append((v2, v1))

            if v1 == v2:
                print('Self Edges')

    cc = gr.connectedComponents()

    flag1 = len(cc) == 1
    flag2 = len(set(weights)) == len(weights)
    flag3 = len(set(edges_list)) == len(edges_list)

    if flag1 and flag2 and flag3:
        print("Correct input")

    if flag1 != True:
        print('!!!!!!! Disconnected graph')

    if flag2 != True:
        print('!!!!!!! Non unique edges')

    if flag3 != True:
        print('!!!!!!! Multiple edges between node pairs')
