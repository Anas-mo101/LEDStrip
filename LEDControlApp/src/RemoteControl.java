import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.URL;
import java.util.ResourceBundle;
import com.fazecast.jSerialComm.SerialPort;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.ColorPicker;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.paint.Color;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;

public class RemoteControl implements Initializable{  
    static SerialPort chosenPort;
    PrintWriter output;
    BufferedWriter out;
    Color color;

    @FXML
    private Slider sliderInterval;
    
    @FXML
    private Label Intervallbl;

    @FXML
    private ColorPicker colorPick;
    
    @Override
    public void initialize(URL arg0, ResourceBundle arg1) {
        colorPick.setDisable(true);

        sliderInterval.setOnMouseReleased(event -> {
            System.out.println("S" + Integer.toString( (int) sliderInterval.getValue()));
            Intervallbl.setText( Integer.toString( (int) sliderInterval.getValue() ));
            output.print("S" + Integer.toString( (int) sliderInterval.getValue()) );
            output.flush();
        });

        colorPick.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent arg0) {
                color = colorPick.getValue();
                String sentColor = "R" + color.toString().substring(0,8);
                System.out.println(sentColor);
                output.print(sentColor);
                output.flush();
            }
        });
    }

    public void setPort(SerialPort chosenPort){
        KeypadController.chosenPort = chosenPort;
        out = new BufferedWriter(new OutputStreamWriter( KeypadController.chosenPort.getOutputStream() ));
        output = new PrintWriter(KeypadController.chosenPort.getOutputStream());
    }

    @FXML
    void btnOne(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "1";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    @FXML
    void btnSeven(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "7";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    @FXML
    void btnD(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "D";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    @FXML
    void btnE(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "E";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    @FXML
    void btnEight(ActionEvent event) {
        colorPick.setDisable(false);
        String send = "8";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    @FXML
    void btnTwo(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "2";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    @FXML
    void btnThree(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "3";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    @FXML
    void btnNine(ActionEvent event) {
        colorPick.setDisable(false);
        String send = "9";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    @FXML
    void btnF(ActionEvent event) {
        colorPick.setDisable(false);
        String send = "F";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    @FXML
    void btnG(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "G";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }

    @FXML
    void btnA(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "A";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    void btnFour(ActionEvent event) {
        colorPick.setDisable(false);
        String send = "4";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    void btnFive(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "5";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }   

    @FXML
    void btnB(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "B";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    void btnH(ActionEvent event) {
        colorPick.setDisable(false);
        String send = "H";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    void btnI(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "0";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    void btnC(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "C";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    void btnSix(ActionEvent event) {
        colorPick.setDisable(true);
        String send = "6";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    void btnJ(ActionEvent event) {
        String send = "J";
        try {
            out.write(send, 0, send.length());
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    void colorPick(ActionEvent event) {

    }

    @FXML
    void sliderInterval(ActionEvent event) {

    }

}
