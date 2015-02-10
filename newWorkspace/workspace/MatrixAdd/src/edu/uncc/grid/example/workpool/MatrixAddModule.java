package edu.uncc.grid.example.workpool;

import java.util.Random;
import java.util.logging.Level;

import edu.uncc.grid.pgaf.datamodules.Data;
import edu.uncc.grid.pgaf.datamodules.DataMap;
import edu.uncc.grid.pgaf.interfaces.basic.Workpool;
import edu.uncc.grid.pgaf.p2p.Node;

public class MatrixAddModule extends Workpool {

	private static final long serialVersionUID = 1L;

	int[][] matrixA;
	int[][] matrixB;
	int[][] matrixC;

	public MatrixAddModule() {		
		matrixC = new int[3][3];
	}
			
	public void initMatrices(){	
		matrixA = new int[][]{{1,2,3},
						{4,5,6},
						{7,8,9}};
		
		matrixB = new int[][]{{9,8,7},
						{6,5,4},
						{3,2,1}};		
	}

	public int getDataCount() {
		return 3;
	}

	public void initializeModule(String[] args) {
	
		Node.getLog().setLevel(Level.WARNING);		
	}
	
	public Data DiffuseData(int segment) {

		int[] rowA = new int[3];
		int[] rowB = new int[3];

		DataMap<String, int[]> d =new DataMap<String, int[]>();

		int k = segment;
		for (int i=0;i<3;i++){		//Copy one row of A and one row of B into d
			rowA[i] = matrixA[k][i];
			rowB[i] = matrixB[k][i];
		}

		d.put("rowA",rowA);
		d.put("rowB",rowB);
		
		return d;
	}

	public Data Compute(Data data) {
			
		int[] rowC = new int[3];

		DataMap<String, int[]> input = (DataMap<String,int[]>)data;
		DataMap<String, int[]> output = new DataMap<String, int[]>();

		int[] rowA = (int[]) input.get("rowA");
		int[] rowB = (int[]) input.get("rowB");

		//computation	
	
		for (int i=0;i<3;i++){
			rowC[i] = rowA[i] + rowB[i];
		}
 							
		output.put("rowC",rowC);	
		
		return output;
	}
	
	public void GatherData(int segment, Data dat) {

		DataMap<String,int[]> out = (DataMap<String,int[]>) dat;

		int[] rowC = (int[]) out.get("rowC");
					
		for (int i=0;i<3;i++) {
			matrixC[segment][i]= rowC[i];
		}

	}
	
	public void printResult(){

		for (int i=0;i<3;i++){
			System.out.println();	
			for(int j=0;j<3;j++){			
				System.out.print(matrixC[i][j] + "  ");			
			}	
		}
		System.out.println();
	}

} // end of MatrixAddModule
