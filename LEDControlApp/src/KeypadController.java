import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.URL;
import java.util.ResourceBundle;
import com.fazecast.jSerialComm.SerialPort;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.input.KeyEvent;

public class KeypadController implements Initializable{  
    static SerialPort chosenPort;
    BufferedWriter out;
    
    @Override
    public void initialize(URL arg0, ResourceBundle arg1) {
        
    }

    @FXML
    void keyPressed(KeyEvent event) {
        System.out.println("Released key code: " + event.getCode().toString());
        switch(event.getCode().toString()){
            case "Q": 
                keyQ();
                break;

            case "W":
                keyW();
                break;

            case "E":
                keyE();
                break;

            case "A":
                keyA();
                break;
            
            case "S":
                keyS();
                break;

            case "Z":
                keyZ();
                break;

            case "X":
                keyX();
                break;
        }
    }

    void keyQ() {
        String send = "K";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }     
    }

    void keyW() {
        String send = "L";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    void keyE() {
        String send = "M";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    void keyA() {
        String send = "O";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    void keyS() {
        String send = "P";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    void keyZ() {
        String send = "Q";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    void keyX() {
        String send = "T";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    public void setPort(SerialPort chosenPort){
        KeypadController.chosenPort = chosenPort;
        out = new BufferedWriter(new OutputStreamWriter( KeypadController.chosenPort.getOutputStream() ));
    }

}
