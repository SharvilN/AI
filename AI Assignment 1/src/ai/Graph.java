package ai;

/**
 * Class for input weighted undirected graph
 * 
 * @author sharvil
 */
public class Graph {
	final int vertices;
	final boolean[] isLocked;
	final boolean[][] keys;
	final int[][] costMatrix;
	
	public Graph(final int vertices, final boolean[] isLocked, final boolean[][] keys, final int[][] costMatrix){
		this.vertices = vertices;
		this.isLocked = isLocked;
		this.keys = keys;
		this.costMatrix = costMatrix;
	}	
}
