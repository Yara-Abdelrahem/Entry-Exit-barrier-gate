package com.mycompany.entry.exit_gate;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import java.io.IOException;
import java.util.ArrayList;

public class App extends Application {

    private static Scene scene;
    private static Stage stage;
    private static Parent primaryRoot; // Cache the primary root

    @Override
    public void start(Stage primaryStage) throws IOException {
        // Load primary root once and cache it
        primaryRoot = loadFXML("primary");
        scene = new Scene(primaryRoot, 1100, 1080);
        stage = primaryStage;
        stage.setScene(scene);
        stage.setTitle("Entry Exit Barrier Gate");
        stage.show();
    }

    static void setRoot(String fxml) throws IOException {
        if ("primary".equals(fxml)) {
            scene.setRoot(primaryRoot); // Reuse cached primary root
        } else {
            scene.setRoot(loadFXML(fxml));
        }
    }

    private static Parent loadFXML(String fxml) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(App.class.getResource(fxml + ".fxml"));
        return fxmlLoader.load();
    }

    public static void main(String[] args) {
        launch();
    }

    public static void switchToSecondary(ArrayList<ArrayList<String>> data) throws IOException {
        FXMLLoader loader = new FXMLLoader(App.class.getResource("secondary.fxml"));
        Parent root = loader.load();

        SecondaryController controller = loader.getController();
        controller.receiveData(data);

        scene.setRoot(root);
    }
}