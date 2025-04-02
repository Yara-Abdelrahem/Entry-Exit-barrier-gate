module com.mycompany.entry.exit_gate {
    requires javafx.controls;
    requires javafx.fxml;

    opens com.mycompany.entry.exit_gate to javafx.fxml;
    exports com.mycompany.entry.exit_gate;
}
