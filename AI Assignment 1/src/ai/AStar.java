package ai;

import java.util.Comparator;
import java.util.PriorityQueue;

/**
 * Utility class for AStar Algorithm
 * 
 * @author sharvil
 */
public class AStar {
	
	private final static int INF = 9999;
	
	private final Graph graph;
	// Ith element is set to 1 if the node was expanded before
	private boolean[] alreadyExpandedNodes;
	// Stores unexpanded nodes
	private PriorityQueue<Node> fringeList;
	// Stores heuristic costs estimated
	private int[] hCost;
	
	public AStar(final Graph graph){
		this.graph = graph;
		// Initializing alreadyExpandedNodes
		final int vertices = graph.vertices;
		alreadyExpandedNodes = new boolean[vertices];
		for(int v = 0; v < vertices; v++){
			alreadyExpandedNodes[v] = false;
		}
		// Initializing fringeList
		fringeList = new PriorityQueue<Node>(vertices, new NodeComparator());
		computeHeuristicValues(graph, vertices - 1);
		for(int i=0; i<vertices; i++){
			System.out.print(hCost[i] + " ");
		}
	}
	
	public int findOptimalPathCost(){
		final int vertices = graph.vertices;
		// Initializing fCost with default values as INF
		int[] fCost = new int[vertices];
		for(int v = 0; v < vertices; v++){
			fCost[v] = INF;
		}
		// Initializing node wise fCost required for handling key and lock issue
		int[][] nodeWiseFCost = new int[vertices][vertices];
		for(int v = 0; v < vertices; v++){
			for(int w = 0; w < vertices; w++){
				nodeWiseFCost[v][w] = INF;
			}
		}
		
		final Node startNode = new Node(0, hCost[0]);
		final Node goalNode = new Node(vertices - 1, hCost[vertices - 1]);
		fringeList.add(startNode);
		Node currentNode = startNode;
		fCost[currentNode.vertex] = hCost[currentNode.vertex];
		while(currentNode.vertex != goalNode.vertex){
			currentNode = fringeList.poll();
			System.out.println("Current Node : " + currentNode.vertex + " fCost : " + fCost[currentNode.vertex]);
			alreadyExpandedNodes[currentNode.vertex] = true;
			for(int neighbour = 0; neighbour < vertices; neighbour++){
				if(graph.costMatrix[currentNode.vertex][neighbour] != INF){
					if(!alreadyExpandedNodes[neighbour]){
						// Passing keys to its neighbors
						for(int v = 0; v < vertices; v++){
							if(graph.keys[currentNode.vertex][v]){
								nodeWiseFCost[currentNode.vertex][v] = fCost[currentNode.vertex];
								System.out.println("Passing keys to neighbours...");
								nodeWiseFCost[neighbour][v] = fCost[currentNode.vertex] - hCost[currentNode.vertex] 
															  + graph.costMatrix[currentNode.vertex][neighbour] + hCost[neighbour];
							}
						}
					}
					// We went, saw the node locked and came back. So, additional cost incurred for current node
					if(graph.isLocked[neighbour] && nodeWiseFCost[currentNode.vertex][neighbour] == INF && !graph.keys[currentNode.vertex][neighbour]){
					//	fCost[currentNode.vertex] += graph.costMatrix[currentNode.vertex][neighbour];
					//	System.out.println("Additional cost incurred... : " + fCost[currentNode.vertex]);
					}
					else if(graph.isLocked[neighbour] && (nodeWiseFCost[currentNode.vertex][neighbour] != INF || graph.keys[currentNode.vertex][neighbour])){
						final int newFCost = nodeWiseFCost[currentNode.vertex][neighbour] + graph.costMatrix[currentNode.vertex][neighbour]
											 + hCost[neighbour] - hCost[currentNode.vertex];
						System.out.println("Updating fCost of unlocked neighbour node " + neighbour + " to " + newFCost);

						if(fCost[neighbour] > newFCost){
							fringeList.remove(new Node(neighbour, fCost[neighbour]));
							System.out.println("prev fcost of " + neighbour + " : " + fCost[neighbour]);							
							fCost[neighbour] = newFCost;
							fringeList.add(new Node(neighbour, newFCost));
							System.out.println("new fcost  of " + neighbour + " : " + newFCost);
						}
					}
					else{
						if(!alreadyExpandedNodes[neighbour]){
							final int newFCost = fCost[currentNode.vertex] + graph.costMatrix[currentNode.vertex][neighbour]
												 + hCost[neighbour] - hCost[currentNode.vertex];
							System.out.println("prev fcost of " + neighbour + " : " + fCost[neighbour]);
							System.out.println("new fcost  of " + neighbour + " : " + newFCost);

							if(fCost[neighbour] > newFCost){
								fringeList.remove(new Node(neighbour, fCost[neighbour]));
								fCost[neighbour] = newFCost;
								fringeList.add(new Node(neighbour, newFCost));
							}
						}
						else{
							System.out.println("Already expanded nodes...");
							int flag = 0;
							for(int v = 0; v < vertices  && flag == 0; v++){
								if(nodeWiseFCost[currentNode.vertex][v] <= nodeWiseFCost[neighbour][v]){
									// Passing keys to its neighbors
									for(int w = 0; w < vertices; w++){
										System.out.println("Passing keys to neighbours in already exp nodes...");
										if(graph.keys[currentNode.vertex][w]){
											flag = 1;
											nodeWiseFCost[neighbour][w] = fCost[currentNode.vertex] - hCost[currentNode.vertex] 
																		  + graph.costMatrix[currentNode.vertex][neighbour] + hCost[neighbour];
										}
									}
								}
							}
							System.out.println("Flag : " + flag);
							if(flag == 1){
								for(int w = 0; w < vertices; w++){
									if(graph.keys[currentNode.vertex][w]){
										fringeList.add(new Node(neighbour, nodeWiseFCost[neighbour][w]));
										break;
									}
								}
							}
						}
					}
				}
			}
		}
		final int optimalCost = fCost[goalNode.vertex];
		return optimalCost;
	}
	
	private void computeHeuristicValues(final Graph graph, final int src) {
		// Dijsktra's Algorithm
		dijkstra(graph, src);
		
	}
	
	private int minDistance(final int dist[], final boolean sptSet[], final int V){
	   // Initialize min value
	   int min = INF, min_index = 0;
	  
	   for (int v = 0; v < V; v++)
	     if (sptSet[v] == false && dist[v] <= min){
	         min = dist[v];
	         min_index = v;
	     }	  
	   return min_index;
	}

	// Function that implements Dijkstra's single source shortest path algorithm
	// for a graph represented using adjacency matrix representation
	void dijkstra(final Graph graph,final int src){
		
		int V = graph.vertices;
		hCost = new int[V]; // The output array. hCost[i] will hold
        // the shortest distance from src to i

			// sptSet[i] will true if vertex i is included in shortest
			// path tree or shortest distance from src to i is finalized
			boolean sptSet[] = new boolean[V];
			
			// Initialize all distances as INFINITE and stpSet[] as false
			for (int i = 0; i < V; i++)
			{
			hCost[i] = Integer.MAX_VALUE;
			sptSet[i] = false;
			}
			
			// hCostance of source vertex from itself is always 0
			hCost[src] = 0;
			
			// Find shortest path for all vertices
			for (int count = 0; count < V-1; count++){
			// Pick the minimum hCostance vertex from the set of vertices
			// not yet processed. u is always equal to src in first
			// iteration.
			int u = minDistance(hCost, sptSet, V);
			
			// Mark the picked vertex as processed
			sptSet[u] = true;
			
			// Update hCost value of the adjacent vertices of the
			// picked vertex.
			for (int v = 0; v < V; v++)
			
			// Update hCost[v] only if is not in sptSet, there is an
			// edge from u to v, and total weight of path from src to
			// v through u is smaller than current value of hCost[v]
			if (!sptSet[v] && graph.costMatrix[u][v]!=0 &&
			hCost[u] != Integer.MAX_VALUE &&
			hCost[u]+graph.costMatrix[u][v] < hCost[v])
			hCost[v] = hCost[u] + graph.costMatrix[u][v];
	     }
	}
	
	class NodeComparator implements Comparator<Node>{
	    // Overriding compare() method of Comparator 
	                // for descending order of fCost
	    public int compare(Node n1, Node n2) {
	        if (n1.fCost > n2.fCost)
	            return 1;
	        else if (n1.fCost < n2.fCost)
	            return -1;
	        return 0;
	    }
	}
}
