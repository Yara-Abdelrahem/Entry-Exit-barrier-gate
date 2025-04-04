package com.mycompany.entry.exit_gate;

import java.io.IOException;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ToggleGroup;
import java.util.ArrayList;

public class PrimaryController implements Runnable {

    @FXML
    RadioButton EntryON, EntryOFF, ExitON, ExitOFF;
    @FXML
    Label label1;
    Thread thread;
    private Client client;
    @FXML
    private Label SpotCount;
    @FXML
    private Label Spot1Status;
    @FXML
    private Label Spot2Status;
    @FXML
    private Label Spot3Status;
    @FXML
    private ToggleGroup ExitDoorControl;
    @FXML
    private ToggleGroup EntryDoorControl;

    public void initialize() {
        client = new Client();
        thread = new Thread(this);
        thread.setDaemon(true);
        thread.start();
    }

    @Override
    public void run() {
        while (true) {
            // client.printMessage("GetSpotStatus");
            try {
                Thread.sleep(500);
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }
        }
    }

    @FXML
    public void entryDoorControl(ActionEvent event) {
        if (EntryON.isSelected()) {
            label1.setText("Entry Door is Open");
            client.printMessage("OpenEntry");
        } else if (EntryOFF.isSelected()) {
            label1.setText("Entry Door is Closed");
            client.printMessage("CloseEntry");
        }
    }

    //exit_dexitDooeoor
    @FXML
    public void exitDoorControl(ActionEvent event) {
        if (ExitON.isSelected()) {
            label1.setText("Exit Door is Open");
            client.printMessage("OpenExit");
        } else if (ExitOFF.isSelected()) {
            label1.setText("Exit Door is Closed");
            client.printMessage("CloseExit");
        }
    }

    @FXML
    private void ShowHistory(ActionEvent event) throws IOException {
        client.printMessage("History");

        try {
            Thread.sleep(500);
        } catch (InterruptedException ex) {
            ex.printStackTrace();
        }

        String itr = client.readMessage();
        ArrayList<ArrayList<String>> twoDList = new ArrayList<>();

        System.out.println(itr);

        if (itr != null) {
            for (int i = 0; i < Integer.parseInt(itr); i++) {
                ArrayList<String> arrString = new ArrayList<>();

                for (int j = 0; j < 3; j++) {
                    String msg = client.readMessage();
                    if (msg != null) {
                        arrString.add(msg);
                    } else {
                        break;
                    }
                }
                twoDList.add(arrString);
            }
        }

        App.switchToSecondary(twoDList);
    }

}

class Client {

    private java.net.Socket server;
    private java.io.BufferedReader reader;
    private java.io.PrintStream output;

    public Client() {
        try {
            server = new java.net.Socket("127.0.0.1", 5005);
            reader = new java.io.BufferedReader(
                    new java.io.InputStreamReader(server.getInputStream()));
            output = new java.io.PrintStream(server.getOutputStream());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void printMessage(String msg) {
        output.println(msg);
    }

    public String readMessage() {
        String msg = null;
        try {
            msg = reader.readLine();
        } catch (java.io.IOException e) {
            e.printStackTrace();
        }
        return msg;
    }
}
