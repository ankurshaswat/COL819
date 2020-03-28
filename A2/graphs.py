# """ A Python Class
# A simple Python graph class, demonstrating the essential 
# facts and functionalities of graphs.
# """


# class Graph(object):

#     def __init__(self, graph_dict=None):
#         """ initializes a graph object 
#             If no dictionary or None is given, 
#             an empty dictionary will be used
#         """
#         if graph_dict == None:
#             graph_dict = {}
#         self.__graph_dict = graph_dict

#     def vertices(self):
#         """ returns the vertices of a graph """
#         return list(self.__graph_dict.keys())

#     def edges(self):
#         """ returns the edges of a graph """
#         return self.__generate_edges()

#     def add_vertex(self, vertex):
#         """ If the vertex "vertex" is not in 
#             self.__graph_dict, a key "vertex" with an empty
#             list as a value is added to the dictionary. 
#             Otherwise nothing has to be done. 
#         """
#         if vertex not in self.__graph_dict:
#             self.__graph_dict[vertex] = []

#     def add_edge(self, edge):
#         """ assumes that edge is of type set, tuple or list; 
#             between two vertices can be multiple edges! 
#         """
#         edge = set(edge)
#         (vertex1, vertex2) = tuple(edge)
#         if vertex1 in self.__graph_dict:
#             self.__graph_dict[vertex1].append(vertex2)
#         else:
#             self.__graph_dict[vertex1] = [vertex2]

#     def __generate_edges(self):
#         """ A static method generating the edges of the 
#             graph "graph". Edges are represented as sets 
#             with one (a loop back to the vertex) or two 
#             vertices 
#         """
#         edges = []
#         for vertex in self.__graph_dict:
#             for neighbour in self.__graph_dict[vertex]:
#                 if {neighbour, vertex} not in edges:
#                     edges.append({vertex, neighbour})
#         return edges

#     def __str__(self):
#         res = "vertices: "
#         for k in self.__graph_dict:
#             res += str(k) + " "
#         res += "\nedges: "
#         for edge in self.__generate_edges():
#             res += str(edge) + " "
#         return res

#     def is_connected(self, 
#                      vertices_encountered = None, 
#                      start_vertex=None):
#         """ determines if the graph is connected """
#         if vertices_encountered is None:
#             vertices_encountered = set()
#         gdict = self.__graph_dict        
#         vertices = list(gdict.keys()) # "list" necessary in Python 3 
#         if not start_vertex:
#             # chosse a vertex from graph as a starting point
#             start_vertex = vertices[0]
#         vertices_encountered.add(start_vertex)
#         if len(vertices_encountered) != len(vertices):
#             for vertex in gdict[start_vertex]:
#                 if vertex not in vertices_encountered:
#                     if self.is_connected(vertices_encountered, vertex):
#                         return True
#         else:
#             return True
#         return False


# if __name__ == "__main__":

#     g = {}

#     path = 'inps/sample_inp.txt'
#     with open(path,'r') as f:
#         data = f.readlines()
#         numNodes = int(data[0].strip())

#         for row in data[1:]:
#             r= row.strip().split('(')[1].split(')')[0].split(',')
#             v1 = int(r[0])
#             v2 = int(r[1])
#             e = int(r[2])
#             # print(int(r[0]),int(r[1]),int(r[2]))
#             # print(r)
#             if v1 not in g:
#                 g[v1] = []
#             g[v1].append(v2)
#             if v2 not in g:
#                 g[v2] = []
#             g[v2].append(v1)


#     # g = { "a" : ["d"],
#     #       "b" : ["c"],
#     #       "c" : ["b", "c", "d", "e"],
#     #       "d" : ["a", "c"],
#     #       "e" : ["c"],
#     #       "f" : []
#     #     }


#     graph = Graph(g)

#     print("Vertices of graph:")
#     print(graph.vertices())

#     print("Edges of graph:")
#     print(graph.edges())

#     print(graph.is_connected())

#     # print("Add vertex:")
#     # graph.add_vertex("z")

#     # print("Vertices of graph:")
#     # print(graph.vertices())
 
#     # print("Add an edge:")
#     # graph.add_edge({"a","z"})
    
#     # print("Vertices of graph:")
#     # print(graph.vertices())

#     # print("Edges of graph:")
#     # print(graph.edges())

#     # print('Adding an edge {"x","y"} with new vertices:')
#     # graph.add_edge({"x","y"})
#     # print("Vertices of graph:")
#     # print(graph.vertices())
#     # print("Edges of graph:")
#     # print(graph.edges())

import sys
# Python program to print connected 
# components in an undirected graph 
class Graph: 
	
	# init function to declare class variables 
	def __init__(self,V): 
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

# Driver Code 
if __name__=="__main__": 
	
	# Create a graph given in the above diagram 
	# 5 vertices numbered from 0 to 4 
	g = {}
	gr = None
	weights = []
	edges_list = []
	# path = 'inps/sample_inp.txt'
	path = sys.argv[1]

	with open(path,'r') as f:
		data = f.readlines()
		numNodes = int(data[0].strip())
		gr = Graph(numNodes)

		for row in data[1:]:
			r= row.strip().split('(')[1].split(')')[0].split(',')
			v1 = int(r[0])
			v2 = int(r[1])
			e = int(r[2])
			# print(int(r[0]),int(r[1]),int(r[2]))
			# print(r)
			if v1 not in g:
				g[v1] = []
			g[v1].append(v2)
			if v2 not in g:
				g[v2] = []
			g[v2].append(v1)

			gr.addEdge(v1,v2)
			weights.append(e)

			if v1<v2 :
				edges_list.append((v1,v2))
			else:
				edges_list.append((v2,v1))

			if v1==v2:
				print('Self Edges')
	# g = { "a" : ["d"],
	#       "b" : ["c"],
	#       "c" : ["b", "c", "d", "e"],
	#       "d" : ["a", "c"],
	#       "e" : ["c"],
	#       "f" : []
	#     }


	# graph = Graph(g)

	# g = Graph(5); 
	# g.addEdge(1, 0) 
	# g.addEdge(2, 3) 
	# g.addEdge(3, 4) 
	cc = gr.connectedComponents() 
	# print("Following are connected components") 
	# print(cc)
	# print(len(cc)) 

	flag1 = len(cc) == 1
	flag2 = len(set(weights)) == len(weights) 
	flag3 = len(set(edges_list)) == len(edges_list)

	if flag1 and flag2 and flag3:
		print("Correct input")

	if flag1!=True:
		print('Disconnected graph')

	if flag2!=True:
		print('Non unique edges')

	if flag3!=True:
		print('Multiple edges between node pairs')
# This code is contributed by Abhishek Valsan 
