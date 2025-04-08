/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package exit_entry.dal;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import org.apache.derby.jdbc.ClientDriver;
import java.sql.Timestamp;

public class DAO {

    private static int idCounter;

    static {
        createDatabaseIfNotExists();
        createTableIfNotExists();
        initializeIdCounter();
    }

    private static void initializeIdCounter() {
        String url = "jdbc:derby://localhost:1527/root";
        String user = "root";
        String password = "root";
        String query = "SELECT MAX(CARID) AS max_id FROM CARSTIMELINE";
        try (Connection connection = DriverManager.getConnection(url, user, password);
             Statement statement = connection.createStatement();
             ResultSet rs = statement.executeQuery(query)) {
            if (rs.next()) {
                int maxId = rs.getInt("max_id");
                
                idCounter = maxId + 1;
                System.out.println("maxid: "+Integer.toString(idCounter));
            } else {
                idCounter = 1; 
            }
        } catch (SQLException e) {
            e.printStackTrace();
            idCounter = 1; 
        }
    }

    private static void createDatabaseIfNotExists() {
        String url = "jdbc:derby://localhost:1527/root;create=true";
        String user = "root";
        String password = "root";
        try (Connection connection = DriverManager.getConnection(url, user, password)) {
        System.out.println("Database created or exists");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private static void createTableIfNotExists() {
        String url = "jdbc:derby://localhost:1527/root";
        String user = "root";
        String password = "root";
        String checkTableExistsSQL = "SELECT COUNT(*) FROM SYS.SYSTABLES WHERE TABLENAME = 'CARSTIMELINE'";
        String createTableSQL = "CREATE TABLE CARSTIMELINE ("
                + "CARID INT NOT NULL PRIMARY KEY, "
                + "INTIMESTAMP TIMESTAMP, "
                + "OUTTIMESTAMP TIMESTAMP)";
        try (Connection connection = DriverManager.getConnection(url, user, password);
             Statement statement = connection.createStatement();
             ResultSet rs = statement.executeQuery(checkTableExistsSQL)) {
            if (rs.next() && rs.getInt(1) == 0) {
                statement.executeUpdate(createTableSQL);
                System.out.println("Table 'CARSTIMELINE' created successfully.");
            }else{
                System.out.println("Table 'CARSTIMELINE' was created previously.");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static int InsertCar(CARDTO Car) throws SQLException {
        int result = -1;
        DriverManager.registerDriver(new ClientDriver());
        try (Connection connection = DriverManager.getConnection("jdbc:derby://localhost:1527/root", "root", "root")) {
            PreparedStatement statement = connection.prepareStatement("INSERT INTO ROOT.CARSTIMELINE (CARID, INTIMESTAMP) VALUES (?, ?)");
            Car.setCarID(idCounter);
            result = idCounter++;
            statement.setInt(1, Car.getCarID());
            statement.setTimestamp(2, Car.getCarInTimeStamp());
            statement.executeUpdate();
        }
        return result;
    }

    public static boolean SetOutTimeStamp(CARDTO Car) throws SQLException {
        boolean status = false;
        String url = "jdbc:derby://localhost:1527/root";
        String user = "root";
        String password = "root";
        String selectQuery = "SELECT * FROM ROOT.CARSTIMELINE WHERE CARID = ?";
        String updateQuery = "UPDATE ROOT.CARSTIMELINE SET OUTTIMESTAMP = ? WHERE CARID = ?";
        try (Connection connection = DriverManager.getConnection(url, user, password);
             PreparedStatement selectStmt = connection.prepareStatement(selectQuery);
             PreparedStatement updateStmt = connection.prepareStatement(updateQuery)) {
            selectStmt.setInt(1, Car.getCarID());
            try (ResultSet Rs = selectStmt.executeQuery()) {
                if (Rs.next()) {
                    Timestamp currentTimestamp = new Timestamp(System.currentTimeMillis());
                    updateStmt.setTimestamp(1, currentTimestamp);
                    updateStmt.setInt(2, Car.getCarID());
                    int rowsAffected = updateStmt.executeUpdate();
                    if (rowsAffected > 0) {
                        Car.setCarOutTimeStamp(currentTimestamp);
                        status = true;
                    }
                }
            }
        }
        return status;
    }

    public static String[][] GetAllCars() throws SQLException {
        String url = "jdbc:derby://localhost:1527/root";
        String user = "root";
        String password = "root";
        String query = "SELECT * FROM ROOT.CARSTIMELINE";

        try (Connection connection = DriverManager.getConnection(url, user, password);
             PreparedStatement statement = connection.prepareStatement(query, ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_READ_ONLY);
             ResultSet resultSet = statement.executeQuery()) {
            resultSet.last();
            int rowCount = resultSet.getRow();
            resultSet.beforeFirst();
            ResultSetMetaData metaData = resultSet.getMetaData();
            int columnCount = metaData.getColumnCount();
            String[][] data = new String[rowCount][columnCount];
            int rowIndex = 0;
            while (resultSet.next()) {
                for (int col = 0; col < columnCount; col++) {
                    data[rowIndex][col] = resultSet.getString(col + 1);
                }
                rowIndex++;
            }
            return data;
        }
    }
}