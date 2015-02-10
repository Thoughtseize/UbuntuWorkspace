package edu.uncc.grid.example.workpool;

import java.util.Random;
import java.util.logging.Level;

import edu.uncc.grid.pgaf.datamodules.Data;
import edu.uncc.grid.pgaf.datamodules.DataMap;
import edu.uncc.grid.pgaf.interfaces.basic.Workpool;
import edu.uncc.grid.pgaf.p2p.Node;

public class MonteCarloPiModule extends Workpool {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private static final int DoubleDataSize = 1000;
	
	double total;
	int random_samples;
	//Random R;
	public MonteCarloPiModule() {
		//R = new Random();
	}

	@Override
	public void initializeModule(String[] args) {
		total = 0;
		Node.getLog().setLevel(Level.WARNING);
		random_samples = 3000;
	}
	public Data Compute(Data data) {
		DataMap<String, Object> input = (DataMap<String,Object>)data;
		DataMap<String, Object> output = new DataMap<String, Object>();
		Long seed = (Long) input.get("seed");
		// compute
		Random r = new Random();
		r.setSeed(seed);
		Long inside = 0L;
		
		for (int i = 0; i < DoubleDataSize ; i++) {
			double x = r.nextDouble();
			double y = r.nextDouble();
			double dist = x * x + y * y;
			if (dist <= 1.0) {
				++inside;// = 1L;
			}
		}
		output.put("inside", inside);
		return output;
	}

	public Data DiffuseData(int segment) {
		Random R = new Random();
		DataMap<String, Object> d =new DataMap<String, Object>();
		d.put("seed", R.nextLong());
		return d;
	}
	public void GatherData(int segment, Data dat) {
		DataMap<String,Object> out = (DataMap<String,Object>) dat;
		Long inside = (Long) out.get("inside");
		total += inside;
	}

	public double getPi() {
		double pi = (total / (random_samples * DoubleDataSize)) * 4;
		return pi;
	}

	public int getDataCount() {
		return random_samples;
	}

}
