package com.mycompany.entry.exit_gate;

import java.io.IOException;
import java.util.ArrayList;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;

public class SecondaryController {

    @FXML
    private Button secondaryButton;
    @FXML
    private GridPane CarInfo;
    @FXML
    private Label CarID;
    @FXML
    private Label CarIn;
    @FXML
    private Label CarOut;

    @FXML
    private void switchToPrimary() throws IOException {
        App.setRoot("primary");
    }
    
        public void receiveData(ArrayList<ArrayList<String>> data) {
        ArrayList<String> ids = new ArrayList<>();
        ArrayList<String> INTIMESTAMP = new ArrayList<>();
        ArrayList<String> OUTIMESTAMP = new ArrayList<>();

        for (ArrayList<String> list : data) {
            ids.add(list.get(0));
            INTIMESTAMP.add(list.get(1));
            OUTIMESTAMP.add(list.get(2));
        }
        String IDText = String.join("\n", ids);
        String INTIMEText = String.join("\n", INTIMESTAMP);
        String OUTTIMEText = String.join("\n", OUTIMESTAMP);
        CarID.setText(IDText);
        CarIn.setText(INTIMEText);
        CarOut.setText(OUTTIMEText);
    }
}