package com.mycompany.exit_entry;

import exit_entry.dal.CARDTO;
import exit_entry.dal.DAO;
import java.io.*;
import java.net.*;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.logging.*;

public class Server {
    private ServerSocket serverSocket;
    private garComm c;
    private volatile boolean currentEntryState;
    private volatile boolean previousEntryState = false;
    private volatile boolean currentExitState;
    private volatile boolean previousExitState = false;
    private static final Object lock = new Object();
    private static int[] idCarMapping = new int[3];

    public Server() {
        try {
            c = garComm.getInstance();
            c.start();
            serverSocket = new ServerSocket(5005);
            new Thread(this::handleClients).start();
        } catch (IOException e) {
            logger.log(Level.SEVERE, "Server initialization failed", e);
        }
    }

    public void startServer() {
        new Thread(() -> {
            while (true) {
                currentEntryState = c.getEntryState();
                currentExitState = c.getExitState();

                handleEntryEvents();
                handleExitEvents();

                previousEntryState = currentEntryState;
                previousExitState = currentExitState;

                safeSleep(100);
            }
        }).start();
    }

    private void handleEntryEvents() {
        if (currentEntryState && currentEntryState != previousEntryState) {
            new Thread(() -> {
                c.sendCommand(c.OPEN_ENTRY_GATE_CMD);
                safeSleep(5000);
                c.sendCommand(c.CLOSE_ENTRY_GATE_CMD);
                processNewCarEntry();
            }).start();
        }
    }

    private void processNewCarEntry() {
        Timestamp timeStamp = new Timestamp(System.currentTimeMillis());
        CARDTO newCar = new CARDTO(timeStamp);
        
        try {
            DAO.InsertCar(newCar);
            synchronized (lock) {
                for (int i = 0; i < idCarMapping.length; i++) {
                    if (idCarMapping[i] == 0 && !c.getSpotState(i)) {
                        idCarMapping[i] = newCar.getCarID();
                        break;
                    }
                }
            }
        } catch (SQLException ex) {
            logger.log(Level.SEVERE, "Database error", ex);
        }
    }

    private void handleExitEvents() {
        if (currentExitState && currentExitState != previousExitState) {
            new Thread(() -> {
                c.sendCommand(c.OPEN_EXIT_GATE_CMD);
                safeSleep(5000);
                c.sendCommand(c.CLOSE_EXIT_GATE_CMD);
                processCarExit();
            }).start();
        }
    }

    private void processCarExit() {
        synchronized (lock) {
            for (int i = 0; i < idCarMapping.length; i++) {
                if (idCarMapping[i] != 0 && !c.getSpotState(i)) {
                    CARDTO car = new CARDTO(idCarMapping[i]);
                    try {
                        DAO.SetOutTimeStamp(car);
                    } catch (SQLException ex) {
                        logger.log(Level.SEVERE, "Database error", ex);
                    }
                    idCarMapping[i] = 0;
                }
            }
        }
    }

    private void handleClients() {
        while (true) {
            try {
                Socket clientSocket = serverSocket.accept();
                new Thread(() -> handleClient(clientSocket)).start();
            } catch (IOException e) {
                logger.log(Level.SEVERE, "Client connection failed", e);
            }
        }
    }

    private void handleClient(Socket socket) {
        try (BufferedReader dis = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintStream ps = new PrintStream(socket.getOutputStream())) {
            
            String command;
            while ((command = dis.readLine()) != null) {
                ps.flush(); // Clear output buffer before each response
                
                if ("History".equalsIgnoreCase(command)) {
                    handleHistoryRequest(ps);
                } else {
                    handleCommand(command, ps);
                }
            }
        } catch (IOException e) {
            logger.log(Level.SEVERE, "Client handling error", e);
        } finally {
            try {
                socket.close();
            } catch (IOException ex) {
                logger.log(Level.SEVERE, "Socket close error", ex);
            }
        }
    }

    private void handleHistoryRequest(PrintStream ps) {
        try {
            String[][] arrStr = DAO.GetAllCars();
            if (arrStr != null) {
                ps.println(arrStr.length);
                for (String[] car : arrStr) {
                    ps.println(String.join(",", car[0], car[1], car[2]));
                }
            } else {
                ps.println("0");
            }
        } catch (SQLException ex) {
            ps.println("0");
        }
        ps.flush();
    }

    private void handleCommand(String command, PrintStream ps) {
        switch (command.toLowerCase()) {
            case "openentry":
                c.sendCommand(c.OPEN_ENTRY_GATE_CMD);
                break;
            case "closeentry":
                c.sendCommand(c.CLOSE_ENTRY_GATE_CMD);
                break;
            case "openexit":
                c.sendCommand(c.OPEN_EXIT_GATE_CMD);
                break;
            case "closeexit":
                c.sendCommand(c.CLOSE_EXIT_GATE_CMD);
                break;
            default:
                ps.println(getSpotStatusString());
        }
        ps.flush();
    }

    private String getSpotStatusString() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 3; i++) {
            sb.append(c.getSpotState(i) ? "1" : "0");
        }
        return sb.toString();
    }

    private void safeSleep(int millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
        }
    }

    private static final Logger logger = Logger.getLogger(Server.class.getName());

    public static void main(String[] args) {
        Server server = new Server();
        server.startServer();
    }
}