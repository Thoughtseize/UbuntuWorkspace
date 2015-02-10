package edu.uncc.grid.example.workpool;

import java.io.IOException;

import net.jxta.pipe.PipeID;
import edu.uncc.grid.pgaf.Anchor;
import edu.uncc.grid.pgaf.Operand;
import edu.uncc.grid.pgaf.Seeds;
import edu.uncc.grid.pgaf.p2p.Types;

public class RunMonteCarloPiModule {

	/**
	 * @param args
	 */
	public static void main(String[] args) { // 1st argument host name
		try {
			long start = System.currentTimeMillis();
			
			MonteCarloPiModule pi = new MonteCarloPiModule();
			Thread id = Seeds.startPatternMulticore( new Operand( (String[])null, new Anchor( args[0]  , Types.DataFlowRole.SINK_SOURCE), pi ),4 );
			
			id.join();
			
			System.out.println( "The result is: " + pi.getPi() ) ;
			
			long stop = System.currentTimeMillis();
			System.out.println("Execution time = " + (double) (stop - start) / 1000.0 + " ms");

		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

}
