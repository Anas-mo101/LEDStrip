import java.net.URL;
import java.util.ResourceBundle;
import java.awt.*;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.ProgressBar;
import javafx.scene.control.Label;

public class AudioController implements Initializable{
    Audio audio = new Audio();
    int lvl;

    @FXML
    private ProgressBar AmpBar;

    @FXML
    private Label ambLbl;

    @FXML
    void ListenBtn(ActionEvent event) {
        audio.launch();                          
    }

    @FXML
    void stopListeningBtn(ActionEvent event) {
        audio.close();
    }

    @Override
    public void initialize(URL arg0, ResourceBundle arg1) {
        // TODO Auto-generated method stub
        EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
                final Runnable r = new Runnable() {
                    @Override
                    public void run() {
                        while(true){
                            AmpBar.setProgress( audio.getAmp() / 20 );
                            ambLbl.setText( Double.toString( audio.getAmp() / 20) );
                        }   
                    }
                };
                new Thread(r).start();
            }
        });
    }

}
