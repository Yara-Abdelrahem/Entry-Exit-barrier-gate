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
    @FXML 
    private Label SpotCount;
    @FXML 
    ImageView logoImage, entry_icon, exit_icon, spot1_image, spot2_image, spot3_image;
    @FXML 
    private Label Spot1Status, Spot2Status, Spot3Status;
    @FXML 
    private ToggleGroup ExitDoorControl, EntryDoorControl;

    private Thread thread;
    private Client client;
    private int cnt = 0;
    private volatile boolean running = true;

    public void initialize() {
        client = new Client();
        initializeImages();
        startStatusThread();
    }

    private void initializeImages() {
        try {
            logoImage.setImage(loadImage("images/logo.png"));
            spot1_image.setImage(loadImage("images/park.png"));
            spot2_image.setImage(loadImage("images/park.png"));
            spot3_image.setImage(loadImage("images/park.png"));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private javafx.scene.image.Image loadImage(String path) {
        return new javafx.scene.image.Image(getClass().getResourceAsStream(path));
    }

    private void startStatusThread() {
        thread = new Thread(this);
        thread.setDaemon(true);
        thread.start();
    }

    @Override
    public void run() {
        while (running) {
            updateParkingStatus();
            safeSleep(500);
        }
    }

    private void updateParkingStatus() {
        client.printMessage("GetSpotStatus");
        final String statusString = client.readMessage();
        
        Platform.runLater(() -> {
            if (statusString != null && statusString.length() >= 3) {
                cnt = 0;
                updateSpot(0, Spot1Status, spot1_image, statusString);
                updateSpot(1, Spot2Status, spot2_image, statusString);
                updateSpot(2, Spot3Status, spot3_image, statusString);
                SpotCount.setText(Integer.toString(cnt));
            }
        });
    }

    private void updateSpot(int index, Label label, ImageView image, String status) {
        if (status.charAt(index) == '1') {
            label.setText("Available");
            cnt++;
        } else {
            label.setText("Not Available");
        }
        updateSpotImage(label, image);
    }

    @FXML
    private void handleDoorControl(ActionEvent event, boolean isEntry) {
        RadioButton selected = (RadioButton) event.getSource();
        String command = selected.getText().contains("ON") ? "Open" : "Close";
        String type = isEntry ? "Entry" : "Exit";
        ImageView icon = isEntry ? entry_icon : exit_icon;

        changeIconImage(selected.getText().contains("ON") ? "ON" : "OFF", icon);
        client.printMessage(command + type);
    }

    @FXML
    public void entryDoorControl(ActionEvent event) {
        handleDoorControl(event, true);
    }

    @FXML
    public void exitDoorControl(ActionEvent event) {
        handleDoorControl(event, false);
    }

    @FXML
    private void ShowHistory(ActionEvent event) throws IOException {
        client.clearBuffer();
        client.printMessage("History");

        ArrayList<ArrayList<String>> twoDList = new ArrayList<>();
        String itr = readValidHistoryCount();
        
        if (itr != null && !itr.isEmpty()) {
            int count = Integer.parseInt(itr);
            for (int i = 0; i < count; i++) {
                ArrayList<String> entry = new ArrayList<>();
                for (int j = 0; j < 3; j++) {
                    String msg = client.readMessage();
                    entry.add(msg != null ? msg : "N/A");
                }
                twoDList.add(entry);
            }
        }

        App.switchToSecondary(twoDList);
    }

    private String readValidHistoryCount() {
        long startTime = System.currentTimeMillis();
        while (System.currentTimeMillis() - startTime < 2000) {
            String response = client.readMessage();
            if (response != null && response.matches("\\d+")) {
                return response;
            }
        }
        return "0";
    }

    private void updateSpotImage(Label spotStatus, ImageView spotImage) {
        String imageName = spotStatus.getText().equals("Available") ? "park.png" : "nopark.png";
        spotImage.setImage(loadImage("images/" + imageName));
    }

    private void changeIconImage(String status, ImageView icon) {
        String imagePath = "led_" + (status.equals("ON") ? "on" : "off") + ".png";
        icon.setImage(loadImage("images/" + imagePath));
    }

    private void safeSleep(int millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
        }
    }

    public void stop() {
        running = false;
        if (client != null) {
            client.close();
        }
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
            System.err.println("Connection error: " + e.getMessage());
        }
    }

    public synchronized void printMessage(String msg) {
        if (output != null) {
            output.println(msg);
        }
    }

    public synchronized String readMessage() {
        try {
            return reader.readLine();
        } catch (java.io.IOException e) {
            return null;
        }
    }

    public synchronized void clearBuffer() {
        try {
            while (reader.ready()) {
                reader.readLine();
            }
        } catch (IOException e) {
            System.err.println("Error clearing buffer: " + e.getMessage());
        }
    }

    public void close() {
        try {
            if (server != null) server.close();
            if (reader != null) reader.close();
            if (output != null) output.close();
        } catch (IOException e) {
            System.err.println("Error closing connection: " + e.getMessage());
        }
    }
}