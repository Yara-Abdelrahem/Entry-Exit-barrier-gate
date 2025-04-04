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
import org.apache.derby.jdbc.ClientDriver;
import java.sql.Timestamp;
import java.util.Date;

/**
 *
 * @author youssef
 */

public class DAO
{
    private static int idCounter;
    
    static{
        idCounter=4;
    }
    
    public static int InsertCar(CARDTO Car) throws SQLException
    {
        int result=-1;
        DriverManager.registerDriver(new ClientDriver());
        Connection  connection= DriverManager.getConnection("jdbc:derby://localhost:1527/root","root","root");
        PreparedStatement statement=connection.prepareStatement("INSERT INTO ROOT.CARSTIMELINE (CARID, INTIMESTAMP) VALUES (?, ?)");
        Car.setCarID(idCounter);
        result =idCounter++;
        statement.setInt(1, Car.getCarID());
        statement.setTimestamp(2, Car.getCarInTimeStamp());
        result=statement.executeUpdate();
        statement.close();
        connection.close();
        return result;
    }

    public static boolean SetOutTimeStamp(CARDTO Car) throws SQLException {
        boolean status = false;
        String url = "jdbc:derby://localhost:1527/root";
        String user = "root";
        String password = "root";
        String selectQuery = "SELECT * FROM ROOT.CARSTIMELINE WHERE CARID = ? ";
        String updateQuery = "UPDATE ROOT.CARSTIMELINE SET OUTTIMESTAMP = ? WHERE CARID = ? ";
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
        } catch (SQLException e) {
            e.printStackTrace(); 
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

            // Move cursor to last row to get row count
            resultSet.last();
            int rowCount = resultSet.getRow();
            resultSet.beforeFirst(); // Move cursor back to beginning

            // Get column count
            ResultSetMetaData metaData = resultSet.getMetaData();
            int columnCount = metaData.getColumnCount();

            // Create 2D array to store results
            String[][] data = new String[rowCount][columnCount];

            int rowIndex = 0;
            while (resultSet.next()) {
                for (int col = 0; col < columnCount; col++) {
                    data[rowIndex][col] = resultSet.getString(col + 1); // Columns in ResultSet start from 1
                }
                rowIndex++;
            }
            return data;
        }
    }
}