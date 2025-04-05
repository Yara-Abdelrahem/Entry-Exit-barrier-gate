package com.mycompany.entry.exit_gate;

import java.io.IOException;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ToggleGroup;
import java.util.ArrayList;
import javafx.scene.image.ImageView;

public class PrimaryController implements Runnable {

    @FXML
    RadioButton EntryON, EntryOFF, ExitON, ExitOFF;

    Thread thread;
    private Client client;
    @FXML
    private Label SpotCount;
    @FXML
    ImageView logoImage, entry_icon, exit_icon, spot1_image, spot2_image, spot3_image;
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
    private int cnt = 0;

    public void initialize() {
        client = new Client();
        thread = new Thread(this);
        thread.setDaemon(true);
        thread.start();
        logoImage.setImage(new javafx.scene.image.Image(getClass().getResourceAsStream("images/logo.png")));
        spot1_image.setImage(new javafx.scene.image.Image(getClass().getResourceAsStream("images/park.png")));
        spot2_image.setImage(new javafx.scene.image.Image(getClass().getResourceAsStream("images/park.png")));
        spot3_image.setImage(new javafx.scene.image.Image(getClass().getResourceAsStream("images/park.png")));
    }

    @Override
    public void run() {
        while (true) {
            // Request the current spot status from the server
            client.printMessage("GetSpotStatus");
            // Read the response from the server
            final String statusString = client.readMessage();
            cnt = 0;
            // Create local copies of the UI controls for use in the lambda
            final Label[] labels = {Spot1Status, Spot2Status, Spot3Status};
            final ImageView[] imageViews = {spot1_image, spot2_image, spot3_image};

            // Update the GUI on the JavaFX Application Thread
            Platform.runLater(() -> {
                if (statusString != null && statusString.length() >= 3) {
                    for (int i = 0; i < 3; i++) {
                        if (statusString.charAt(i) == '1') {
                            labels[i].setText("Available");
                            cnt++;
                        } else {
                            labels[i].setText("Not Available");
                        }
                        updateSpotImage(labels[i], imageViews[i]);
                        SpotCount.setText(Integer.toString(cnt));
                    }
                }
            });

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
            changeIconImage("ON", entry_icon); // Change image to ON
            client.printMessage("OpenEntry");

        } else if (EntryOFF.isSelected()) {
            changeIconImage("OFF", entry_icon); // Change image to OFF
            client.printMessage("CloseEntry");

        }
    }

    //exit_dexitDooeoor
    @FXML
    public void exitDoorControl(ActionEvent event) {
        if (ExitON.isSelected()) {
            changeIconImage("ON", exit_icon); // Change image to ON
            client.printMessage("OpenExit");

        } else if (ExitOFF.isSelected()) {
            changeIconImage("OFF", exit_icon); // Change image to OFF
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

    // This method will check the status of each spot and update the image accordingly
    @FXML
    public void updateSpotImages() {
        updateSpotImage(Spot1Status, spot1_image);
        updateSpotImage(Spot2Status, spot2_image);
        updateSpotImage(Spot3Status, spot3_image);
    }

    // Helper method to check the spot's status and update the image
    private void updateSpotImage(Label spotStatus, ImageView spotImage) {
        String imageName = "nopark.png"; // Default image for "not available"
        if (spotStatus.getText().equals("Available")) {
            imageName = "park.png"; // Image for "available"
        }
        spotImage.setImage(new javafx.scene.image.Image(getClass().getResourceAsStream("images/" + imageName)));
    }

    private void changeIconImage(String status, ImageView icon) {
        String imagePath = status.equals("ON") ? "led_on.png" : "led_off.png";
        icon.setImage(new javafx.scene.image.Image(getClass().getResourceAsStream("images/" + imagePath)));
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
