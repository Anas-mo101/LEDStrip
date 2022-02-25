import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.layout.Pane;
import com.fazecast.jSerialComm.SerialPort;


public class MainController implements Initializable {
    private final String LEDSTRIP = "USB-SERIAL CH340";  
    ObservableList<String> portList;
    static SerialPort chosenPort;
    String COM = "";
    Boolean COMFlag = false;

    @FXML
    private Pane funcPane;

    @FXML
    private Button btnConnect;

    @FXML
    private Button btnKey;

    @FXML
    private Button btnRemote;

    @FXML
    private Button btnSetting;

    @FXML
    private Button btnCustom;

    @FXML
    private Button btnAudio;

    @FXML
    private ComboBox<String> serialBox;

    @Override
    public void initialize(URL arg0, ResourceBundle arg1) {
        btnKey.setDisable(true);
        btnRemote.setDisable(true);
        btnSetting.setDisable(true);
        btnCustom.setDisable(true);
        btnAudio.setDisable(true);
        findPorts();           
    }
    
    public void findPorts(){         
        COM = "";                                    
        portList = FXCollections.observableArrayList();             
        SerialPort[] portNames = SerialPort.getCommPorts();           
        for(int i = 0; i < portNames.length; i++){
            if(portNames[i].getDescriptivePortName().contains(LEDSTRIP)){
                COM = "LED STRIP " + portNames[i].getSystemPortName();
                portList.add(COM); 
            }else{
                portList.add(portNames[i].getSystemPortName()); 
            }
        }

        if(COM.equals("")){
            COMFlag = false;
        }else{
            COMFlag = true;
        }
        serialBox.setItems(portList); 
    }

    public void autoConnect(){
        if(COMFlag){
            String portSelection = COM.substring(COM.lastIndexOf(" ")+1);
            serialBox.getSelectionModel().select(portSelection);
            chosenPort = SerialPort.getCommPort(portSelection);
            chosenPort.setComPortTimeouts(SerialPort.TIMEOUT_SCANNER, 0, 0);
            setConnectBtn();
        }else{
            System.out.println("LEDStrip not found");
        }
    }

    public void setConnectBtn(){
        if (chosenPort.openPort()){
            btnConnect.setText("Disconnect");
            serialBox.setDisable(true);
            btnKey.setDisable(false);
            btnRemote.setDisable(false);
            btnSetting.setDisable(false);
            btnCustom.setDisable(false);
            btnAudio.setDisable(false);
        }else{
            btnConnect.setText("Connect");
            serialBox.setDisable(false);
            btnKey.setDisable(true);
            btnRemote.setDisable(true);
            btnSetting.setDisable(true);
            btnCustom.setDisable(true);
            btnAudio.setDisable(true);
            funcPane.getChildren().clear();
            serialBox.getSelectionModel().clearSelection();
        }
    }
    
    @FXML
    void btnConnect(ActionEvent event){
        if(btnConnect.getText().equals("Connect")){
            findPorts();
            if(!serialBox.getSelectionModel().isEmpty()){
                COM = serialBox.getValue().toString();
                if(serialBox.getValue().toString().contains("LED STRIP")){
                    String portSelection = COM.substring(COM.lastIndexOf(" ")+1);
                    chosenPort = SerialPort.getCommPort(portSelection);
                    chosenPort.setComPortTimeouts(SerialPort.TIMEOUT_SCANNER, 0, 0);
                    setConnectBtn();
                }else{
                    System.out.println("NOT FOUND");
                }
            }else{
                autoConnect();
            }
        }else{
            chosenPort.closePort();
            btnConnect.setText("Connect");
            serialBox.setDisable(false);
            btnKey.setDisable(true);
            btnRemote.setDisable(true);
            btnSetting.setDisable(true);
            btnCustom.setDisable(true);
            btnAudio.setDisable(true);
            funcPane.getChildren().clear();
            serialBox.getSelectionModel().clearSelection();
        }
    }

    @FXML
    void btnReload(ActionEvent event) {
        serialBox.getSelectionModel().clearSelection();
        findPorts();
    }

    @FXML
    void btnRemote(ActionEvent event) throws IOException{
        funcPane.getChildren().clear();
        FXMLLoader loader = new FXMLLoader(getClass().getResource("FXML/RemoteUI.fxml"));
        Pane newLoadedPane = (Pane) loader.load();
        funcPane.getChildren().add(newLoadedPane);
        RemoteControl remote = loader.getController();
        remote.setPort(chosenPort);
    }

    @FXML
    void btnKey(ActionEvent event) throws IOException{ 
        funcPane.getChildren().clear();
        FXMLLoader loader = new FXMLLoader(getClass().getResource("FXML/KeyUI.fxml"));
        Pane newLoadedPane = (Pane) loader.load();
        funcPane.getChildren().add(newLoadedPane);
        KeypadController key = loader.getController();
        key.setPort(chosenPort);
    }

    @FXML
    void btnAudio(ActionEvent event) throws IOException{
        funcPane.getChildren().clear();
        FXMLLoader loader = new FXMLLoader(getClass().getResource("FXML/AudioUI.fxml"));
        Pane newLoadedPane = (Pane) loader.load();
        funcPane.getChildren().add(newLoadedPane);
    }

    @FXML
    void btnCustom(ActionEvent event) throws IOException{
        funcPane.getChildren().clear();
        FXMLLoader loader = new FXMLLoader(getClass().getResource("FXML/XX.fxml"));
        Pane newLoadedPane = (Pane) loader.load();
        funcPane.getChildren().add(newLoadedPane);
    }

    @FXML
    void btnSetting(ActionEvent event) throws IOException{
        funcPane.getChildren().clear();
        FXMLLoader loader = new FXMLLoader(getClass().getResource("FXML/XX.fxml"));
        Pane newLoadedPane = (Pane) loader.load();
        funcPane.getChildren().add(newLoadedPane);
    }
}