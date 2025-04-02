package com.mycompany.entry.exit_gate;

import java.io.IOException;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;

public class PrimaryController {
    
    @FXML
    RadioButton EntryON , EntryOFF , ExitON , ExitOFF;
    @FXML
    Label label1;

    @FXML    
    public void entryDoorControl(ActionEvent event){
        if (EntryON.isSelected()) {
            label1.setText("Entry Door is Open");
        }else if(EntryOFF.isSelected()){
            label1.setText("Entry Door is Closed");
        }
    }
    
    //exit_dexitDooeoor
    @FXML    
    public void exitDoorControl(ActionEvent event){
        if (ExitON.isSelected()) {
            label1.setText("Exit Door is Open");
        }else if(ExitOFF.isSelected()){
            label1.setText("Exit Door is Closed");
        }
    }
    
    @FXML
    private void switchToSecondary() throws IOException {
        App.setRoot("secondary");
    }
}
