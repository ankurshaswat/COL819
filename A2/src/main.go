package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
	"sort"
	"strconv"
	"strings"
	"sync"
)

// State
const (
	sleep = iota
	find  = iota
	found = iota
)

// Branch Status
const (
	basic        = iota
	branch       = iota
	branchReject = iota
)

// Msg Type
const (
	connect       = iota //0
	initiate      = iota //1
	test          = iota //2
	accept        = iota //3
	msgTypeReject = iota //4
	report        = iota //5
	changerootMsg = iota //6
)

var msgTypeReverse = make(map[int]string)
var stateReverse = make(map[int]string)

func init() {
	msgTypeReverse[connect] = "connect"
	msgTypeReverse[initiate] = "initiate"
	msgTypeReverse[test] = "test"
	msgTypeReverse[accept] = "accept"
	msgTypeReverse[msgTypeReject] = "msgTypeReject"
	msgTypeReverse[report] = "report"
	msgTypeReverse[changerootMsg] = "changerootMsg"
	stateReverse[sleep] = "sleep"
	stateReverse[find] = "find"
	stateReverse[found] = "found"
}

// Messages
// <connect,L>
// <initiate,level,name,state>
// <test,level,name>
// <accept>
// <reject>
// <report,bestWt>
// <changeroot>

type msgStruct struct {
	msgType, state, level, sender, weight, fragmentName int
}

type graph struct {
	numNodes  int
	edges     map[int][]int
	distances map[int]map[int]int
}
type node struct {
	selfID, state, level, rec, parentInd, bestWt, bestNodeInd, testNodeInd, name int
	selfChannel                                                                  chan msgStruct
	mainChannel                                                                  chan edge
	neighbours, distances, status                                                []int
	channels                                                                     []chan msgStruct
}

type edge struct {
	v1, v2, e int
}

type byLength []edge

func (s byLength) Len() int {
	return len(s)
}
func (s byLength) Swap(i, j int) {
	s[i], s[j] = s[j], s[i]
}
func (s byLength) Less(i, j int) bool {
	return s[i].e < s[j].e
}

func (n *node) runNode() {
	n.parentInd = -1

	fmt.Println("Process:", n.selfID, " in runNode()")

	// * Initialize status array
	for i := 0; i < len(n.neighbours); i++ {
		n.status = append(n.status, basic)
	}

	// * Alg1: Initialization

	var minWt int = n.distances[0]
	var qInd int = 0
	// * Find min weight edge pq
	for i := 1; i < len(n.neighbours); i++ {
		if n.distances[i] < minWt {
			qInd = i
			minWt = n.distances[i]
		}
	}

	if n.selfID < n.neighbours[qInd] {
		n.mainChannel <- edge{v1: n.selfID, v2: n.neighbours[qInd], e: n.distances[qInd]}
	}
	n.status[qInd] = branch
	n.level = 0
	n.state = found
	n.rec = 0

	// * Send <connect,0> to q
	fmt.Println("Process:", n.selfID, " sending first connect")
	n.channels[qInd] <- msgStruct{msgType: connect, level: 0, sender: n.selfID}

	// fmt.Println("Process:", n.selfID, " starting to read self channel")
	for msg := range n.selfChannel {
		q := msg.sender
		var qInd int

		for i := 0; i < len(n.neighbours); i++ {
			if n.neighbours[i] == q {
				qInd = i
			}
		}

		// fmt.Println("Process:", n.selfID, " parent:", n.neighbours[n.parentInd])

		switch msg.msgType {
		case connect:
			fmt.Println("Process:", n.selfID, " 'Connect' Level:", msg.level, " from Process:", msg.sender)

			if msg.level < n.level {
				if n.selfID < n.neighbours[qInd] {
					n.mainChannel <- edge{v1: n.selfID, v2: n.neighbours[qInd], e: n.distances[qInd]}
				}
				n.status[qInd] = branch
				fmt.Println("Process:", n.selfID, " sending initiate same level")

				n.channels[qInd] <- msgStruct{msgType: initiate, level: n.level, fragmentName: n.name, state: n.state, sender: n.selfID}
				if n.state == find {
					n.rec = n.rec + 1
				}
			} else if n.status[qInd] == basic {
				// * Wait
				fmt.Println("Process:", n.selfID, " deferring msg")
				n.selfChannel <- msg
			} else {
				fmt.Println("Process:", n.selfID, " sending initiate level+1")
				n.channels[qInd] <- msgStruct{msgType: initiate, level: n.level + 1, fragmentName: n.distances[qInd], state: find, sender: n.selfID}
			}
		case initiate:
			fmt.Println("Process:", n.selfID, " 'Initiate' Level:", msg.level, " FragmentName:", msg.fragmentName, " State:", stateReverse[msg.state], " from Process:", msg.sender)

			n.level = msg.level
			n.name = msg.fragmentName
			n.state = msg.state
			n.parentInd = qInd

			n.bestNodeInd = -1
			n.bestWt = math.MaxInt32
			// n.testNodeInd = -1

			for i := range n.neighbours {
				if n.status[i] == branch && i != qInd {
					n.channels[i] <- msgStruct{msgType: initiate, level: n.level, fragmentName: n.name, state: n.state, sender: n.selfID}
					if n.state == find {
						n.rec = n.rec + 1
					}
				}
			}

			// * Find Least edge weight
			if n.state == find {
				// n.rec = 0
				fmt.Println("Process:", n.selfID, " finding min")
				n.findMin()
				// fmt.Println("afterFindMin ", stateReverse[n.state])
			}
		case test:
			fmt.Println("Process:", n.selfID, " 'Test' Level:", msg.level, " FragmentName:", msg.fragmentName, " from Process:", msg.sender)

			if msg.level > n.level {
				// * Wait
				fmt.Println("Process:", n.selfID, " deferring msg")
				n.selfChannel <- msg

			} else if msg.fragmentName == n.name {
				// * Internal Edge
				if n.status[qInd] == basic {
					n.status[qInd] = branchReject
					fmt.Println("Process:", n.selfID, " rejecting branch")
				}
				if qInd != n.testNodeInd {
					fmt.Println("Process:", n.selfID, " sending reject msg")
					n.channels[qInd] <- msgStruct{msgType: msgTypeReject, sender: n.selfID}
				} else {
					fmt.Println("Process:", n.selfID, " finding min")
					n.findMin()
				}
			} else {
				fmt.Println("Process:", n.selfID, " sending accept msg")
				n.channels[qInd] <- msgStruct{msgType: accept, sender: n.selfID}
			}
		case accept:
			fmt.Println("Process:", n.selfID, " 'Accept'", " from Process:", msg.sender)

			n.testNodeInd = -1
			if n.distances[qInd] < n.bestWt {
				n.bestWt = n.distances[qInd]
				n.bestNodeInd = qInd
			}
			n.report()

		case msgTypeReject:
			fmt.Println("Process:", n.selfID, " 'Reject'", " from Process:", msg.sender)

			if n.status[qInd] == basic {
				n.status[qInd] = branchReject
			}
			n.findMin()

		case report:
			fmt.Println("Process:", n.selfID, " 'Report' Weight:", msg.weight, " from Process:", msg.sender)
			// fmt.Println(stateReverse[n.state])
			if qInd != n.parentInd {
				n.rec = n.rec - 1
				if msg.weight < n.bestWt {
					n.bestWt = msg.weight
					n.bestNodeInd = qInd
				}
				// n.rec = n.rec + 1
				fmt.Println("Process:", n.selfID, " reporting")
				n.report()
			} else if n.state == find {
				// * Wait
				fmt.Println("Process:", n.selfID, " deferring msg")
				n.selfChannel <- msg
			} else if msg.weight > n.bestWt {
				fmt.Println("Process:", n.selfID, " changing root")
				n.changeRoot()

			} else if msg.weight == n.bestWt && n.bestWt == math.MaxInt32 {
				// * stop
				fmt.Println("Process:", n.selfID, " stopping")
				// close(n.selfChannel)
				break
			}
		case changerootMsg:
			fmt.Println("Process:", n.selfID, " 'ChangeRoot'", " from Process:", msg.sender)
			n.changeRoot()
			// fmt.Println("Process:", n.selfID, " changing root")

		default:
			log.Fatal("Unknown msg type ", msg.msgType)
		}
	}

	fmt.Println("Process:", n.selfID, " exiting")

}

func (n *node) findMin() {
	fmt.Println("Process:", n.selfID, " in findMin()")

	minDist := math.MaxInt32
	minDistNodeInd := -1

	for i := range n.neighbours {
		if n.status[i] == basic && n.distances[i] < minDist {
			minDist = n.distances[i]
			minDistNodeInd = i
		}
	}

	if minDistNodeInd != -1 {
		n.testNodeInd = minDistNodeInd
		n.channels[minDistNodeInd] <- msgStruct{msgType: test, level: n.level, fragmentName: n.name, sender: n.selfID}
	} else {
		n.testNodeInd = -1
		n.report()
	}
}

func (n *node) report() {
	fmt.Println("Process:", n.selfID, " in report()")

	// count := 0
	// for i := range n.neighbours {
	// 	if n.status[i] == branch && i != n.parentInd {
	// 		count++
	// 	}
	// }

	if n.rec == 0 && n.testNodeInd == -1 {
		n.state = found
		// fmt.Println("Process:", n.selfID, " n.rec:", n.rec, " n.testNodeInd:", n.testNodeInd, " state:", stateReverse[n.state])
		n.channels[n.parentInd] <- msgStruct{msgType: report, weight: n.bestWt, sender: n.selfID}
		fmt.Println("Process:", n.selfID, " sent report to parent")
	}
}

func (n *node) changeRoot() {
	fmt.Println("Process:", n.selfID, " in changeRoot()")

	if n.status[n.bestNodeInd] == branch {
		n.channels[n.bestNodeInd] <- msgStruct{msgType: changerootMsg, sender: n.selfID}
	} else {
		n.channels[n.bestNodeInd] <- msgStruct{msgType: connect, level: n.level, sender: n.selfID}
		if n.selfID < n.neighbours[n.bestNodeInd] {
			n.mainChannel <- edge{v1: n.selfID, v2: n.neighbours[n.bestNodeInd], e: n.distances[n.bestNodeInd]}
		}
		n.status[n.bestNodeInd] = branch
	}
}

func main() {

	// * Get input
	inputPath := os.Args[1]
	fmt.Println("Input:", inputPath)

	// * Pass input to graph parsing function
	g := createGraph(inputPath)
	fmt.Println(g)

	// * Loop through each vertice and create input channels
	numNodes := g.numNodes
	var channelsList []chan msgStruct
	mainChannel := make(chan edge, 1000)
	var expectedMsgs int
	var nodeList []node
	var wg sync.WaitGroup

	for i := int(0); i < numNodes; i++ {
		channelsList = append(channelsList, make(chan msgStruct, 1000))
	}

	// create other data structures required

	// loop through each vertice and run a handler function
	for i := 0; i < numNodes; i++ {
		neighbours := g.edges[i]
		var distances []int
		var neighbourChannels []chan msgStruct
		for _, neighbour := range neighbours {
			distances = append(distances, g.distances[i][neighbour])
			neighbourChannels = append(neighbourChannels, channelsList[neighbour])
		}

		n := node{neighbours: neighbours, distances: distances, channels: neighbourChannels, selfID: i, mainChannel: mainChannel, selfChannel: channelsList[i]}
		nodeList = append(nodeList, n)

		wg.Add(1)

		// go func() {
		go n.runNode()
		// wg.Done()
		// }()
	}

	expectedMsgs = numNodes - 1

	fmt.Println(expectedMsgs)

	var finalEdges []edge

	for i := 0; i < expectedMsgs; i++ {
		e := <-mainChannel
		finalEdges = append(finalEdges, e)
		// fmt.Println(e.v1, e.v2, e.e)
	}

	// for i := 0; i < numNodes; i++ {
	// 	close(channelsList[i])
	// }
	// close(mainChannel)

	sort.Sort(byLength(finalEdges))

	for i := 0; i < expectedMsgs; i++ {
		e := finalEdges[i]
		// finalEdges = append(finalEdges, e)
		fmt.Printf("(%d, %d, %d)\n", e.v1, e.v2, e.e)
	}

	// recieve results from all threads into result thread and create save final mst
	// for i := 0; i < expectedMsgs; i++ {
	// 	res := <-mainChannel
	// 	fmt.Println(res)
	// }
	// wg.Wait()

}

func createGraph(path string) graph {
	// Parse input file and create graph

	readFile, err := os.Open(path)

	if err != nil {
		log.Fatalf("failed to open file: %s", err)
	}

	fileScanner := bufio.NewScanner(readFile)
	fileScanner.Split(bufio.ScanLines)
	var fileTextLines []string

	for fileScanner.Scan() {
		fileTextLines = append(fileTextLines, fileScanner.Text())
	}

	readFile.Close()

	numNodes, err := strconv.Atoi(fileTextLines[0])

	if err != nil {
		log.Fatalf("failed to read numNodes: %s", err)
	}

	// Possibly use adjacency list

	g := graph{numNodes: int(numNodes), edges: make(map[int][]int), distances: make(map[int]map[int]int)}

	for _, eachline := range fileTextLines[1:] {
		res := strings.Split(eachline, "(")
		res = strings.Split(res[1], ")")
		res = strings.Split(res[0], ",")
		// fmt.Println(res[0], res[1], res[2])

		tV1, _ := strconv.Atoi(strings.TrimSpace(res[0]))
		v1 := int(tV1)
		// fmt.Println(v1, res[0], err)
		tV2, _ := strconv.Atoi(strings.TrimSpace(res[1]))
		v2 := int(tV2)
		// fmt.Println(v2, res[1], err)
		tE, _ := strconv.Atoi(strings.TrimSpace(res[2]))
		e := int(tE)
		// fmt.Println(e, res[2], err)
		// fmt.Println(v1, v2, e)
		g.edges[v1] = append(g.edges[v1], v2)
		g.edges[v2] = append(g.edges[v2], v1)

		if g.distances[v1] == nil {
			g.distances[v1] = make(map[int]int)
		}
		g.distances[v1][v2] = e
		if g.distances[v2] == nil {
			g.distances[v2] = make(map[int]int)
		}
		g.distances[v2][v1] = e
	}

	return g
}
