
import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.TargetDataLine;
import javax.sound.sampled.LineUnavailableException;
// import static java.awt.GraphicsDevice.WindowTranslucency.TRANSLUCENT;
import java.awt.*;


public class Audio {
    
    protected TargetDataLine line = null;
    private int amp;

    // Audio(){
    //     startListening();    
    // }

    public void startListening() {
        // Open a TargetDataLine for getting mic input level
        AudioFormat format = new AudioFormat(42000.0f, 16, 1, true, true); // Get default line
        DataLine.Info info = new DataLine.Info(TargetDataLine.class, format);
        if (!AudioSystem.isLineSupported(info)) { // If no default line
            System.out.println("The TargetDataLine is unavailable");
        }
        
        // Obtain and open the line.
        try {
            line = (TargetDataLine) AudioSystem.getLine(info);
            line.open(format);
            line.start();
        } catch (LineUnavailableException ex) {
            System.out.println("The TargetDataLine is Unavailable.");
        }
        
        amp = 0; // Hold calculated RMS volume level
        byte tempBuffer[] = new byte[6000]; // Data buffer for raw audio
        try {
            // Continually read in mic data into buffer and calculate RMS
            while (true) {
                // If read in enough, calculate RMS
                if (line.read(tempBuffer, 0, tempBuffer.length) > 0) {
                    amp = calculateRMSLevel(tempBuffer);
                    System.out.println(amp);
                }
            }
        } catch (Exception e) {
            System.err.println(e);
            System.exit(0);
        }
    }
    
    /** VolumeMeter::calculateRMSLevel
     * Calculate the RMS of the raw audio in buffer
     * @param byte[] audioData  The buffer containing snippet of raw audio data
     * @return int  The RMS value of the buffer
     */
    protected static int calculateRMSLevel(byte[] audioData) {
        long lSum = 0;
        for(int i = 0; i < audioData.length; i++)
            lSum = lSum + audioData[i];

        double dAvg = lSum / audioData.length;

        double sumMeanSquare = 0d;
        for(int j = 0; j < audioData.length; j++)
            sumMeanSquare = sumMeanSquare + Math.pow(audioData[j] - dAvg, 2d);

        double averageMeanSquare = sumMeanSquare / audioData.length;
        return (int)(Math.pow(averageMeanSquare, 0.5d) + 0.5) - 50;
    }

    /** VolumeMeter::close
     * Close TargetDataLine on exit
     */
    public void close() {
        line.close();
    }

    public int getAmp(){
        return amp;
    }

    public void launch() {
        // Create GUI on event-dispatching thread
        EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
                final Runnable r = new Runnable() {
                    @Override
                    public void run() {
                        startListening(); // Start main process

                        // Add shutdown hook to close TargetDataLine on exit
                        Runtime.getRuntime().addShutdownHook(new Thread() {
                            @Override
                            public void run() {
                                close();
                            }
                        });
                    }
                };
                new Thread(r).start();
            }
        });
    }


}
