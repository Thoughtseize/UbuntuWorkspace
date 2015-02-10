package edu.uncc.grid.example.workpool;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Scanner;
import java.util.logging.Level;

import edu.uncc.grid.pgaf.datamodules.Data;
import edu.uncc.grid.pgaf.datamodules.DataMap;
import edu.uncc.grid.pgaf.interfaces.basic.Workpool;
import edu.uncc.grid.pgaf.p2p.Node;

public class MatrixMultModule extends Workpool {

	private static final long serialVersionUID = 1L;

	float[][] matrixA;
	float[][] matrixB;
	float[][] matrixC;

	public MatrixMultModule() {
		matrixC = new float[512][512];
	}

	public void initMatrices() throws FileNotFoundException {
		Scanner file = new Scanner(new File("input2x512x512Doubles"));
		
		matrixA = new float[512][512];
		matrixB = new float[512][512];
		
		for(int i = 0; i < 512; i++){
			for(int j = 0; j < 512; j++){
				matrixA[i][j] = file.nextFloat();
			}
		}
		
		for(int i = 0; i < 512; i++){
			for(int j = 0; j < 512; j++){
				matrixB[i][j] = file.nextFloat();
			}
		}
	}

	public int getDataCount() {
		return 16;
	}

	public void initializeModule(String[] args) {

		Node.getLog().setLevel(Level.WARNING);
	}

	public Data DiffuseData(int segment) {

		float[][] chunkA = new float[128][512];
		float[][] chunkB = new float[512][128];

		DataMap<String, float[][]> d = new DataMap<String, float[][]>();

		int a = (segment / 4);
		a = a * 128;
		
		int b = (segment % 4);
		b = b * 128;
		
		for (int i = 0; i < 128; i++) {
			for (int j = 0; j < 512; j++) {

				chunkA[i][j] = matrixA[i+a][j];
				chunkB[j][i] = matrixB[j][i+b];
			}
		}

		d.put("chunkA", chunkA);
		d.put("chunkB", chunkB);

		return d;
	}

	public Data Compute(Data data) {

		DataMap<String, float[][]> input = (DataMap<String,float[][]>)data;
		DataMap<String, float[][]> output = new DataMap<String, float[][]>();

		float[][] chunkA = (float[][]) input.get("chunkA");
		float[][] chunkB = (float[][]) input.get("chunkB");

		float[][] out = new float[128][128];
		//computation
	
		for (int i=0; i < 128; i++) {
			for(int j=0; j < 128; j++){
				out[i][j] = 0;
				for(int k=0; k < 512; k++){
					out[i][j] = out[i][j] + chunkA[i][k] * chunkB[k][j];
				}
			}
		}
 							
		output.put("out",out);	
		
		return output;
	}

	public void GatherData(int segment, Data dat) {

		DataMap<String, float[][]> out = (DataMap<String, float[][]>) dat;

		float[][] answer = out.get("out");

		int a = (segment / 4);
		a = a * 128;
		
		int b = (segment % 4);
		b = b * 128;
		
		for(int i = 0; i < 128; i++){
			for(int j = 0; j < 128; j++){
				matrixC[i+a][j+b] = answer[i][j];
			}
		}

	}

	public void printResult() {

		for (int i = 0; i < 512; i++) {
			System.out.println();
			for (int j = 0; j < 512; j++) {
				System.out.print(matrixC[i][j] + "  ");
			}
		}
		System.out.println();
	}

} // end of MatrixMultiplyModule

