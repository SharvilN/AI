package ai;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

/**
 * Main class for driving the process of AStar search
 * 
 * @author sharvil
 */
public class DriverApp {
	
	public static void main(String[] args) throws FileNotFoundException{
		final Graph graph = new Graph(8, getLockedNodesList(), getKeys(), getCostMatrix());
		final AStar aStarAgent = new AStar(graph);
		final int optimalPathCost = aStarAgent.findOptimalPathCost();
		System.out.println("Optimal Path Cost : " + optimalPathCost);
	}

	private static boolean[] getLockedNodesList() {
		return new boolean[]{false, false, true, false, true, false, false, false};
	}
	
	private static int[][] getCostMatrix() throws FileNotFoundException {
		
		return new int[][]{
			{9999	,2	 ,9999	 ,9999 	,9999 	,3 	,9999 	,9999},
			{2 	,9999 	,4 	,9999 	,5 	,7 	,9999 	,9999},
			{9999 	,4 	,9999 	,25 	,2 	,9999 	,9999 	,5},
			{9999 	,9999 	,25 	,9999 	,9999 	,9999 	,9999	 ,9999},
			{9999 	,5 	,2 	,9999 	,9999 	,5 	,4 	,8},
			{3 	,7 	,9999 	,9999 	,5 	,9999 	,10 	,9999},
			{9999 	,9999 	,9999 	,9999 	,4 	,10 	,9999 	,6},
			{9999 	,9999 	,5 	,9999 	,8 	,9999 	,6 	,9999}
		};
	}
	
	private static boolean[][] getKeys() {
		return new boolean[][]{
			{false, false, false, false, false, false, false, false},
			{false, false, false, false, false, false, false, false},
			{false, false, false, false, false, false, false, false},
			{false, false, false, false, false, false, false, false},
			{false, false, false, false, false, false, false, false},
			{false, false, true, false, true, false, false, false},
			{false, false, false, false, false, false, false, false},
			{false, false, false, false, false, false, false, false}
		};
	}
}
