package ai;

import java.util.Comparator;

/**
 * Class representing the structure of element in FringeList
 * 
 * @author sharvil
 */
public class Node implements Comparable{
	final int vertex;
	int fCost;
	
	public Node(final int vertex, final int fCost){
		this.vertex = vertex;
		this.fCost = fCost;
	}

	@Override
	public int compareTo(Object o) {
		// TODO Auto-generated method stub
		return 0;
	}
}

  
