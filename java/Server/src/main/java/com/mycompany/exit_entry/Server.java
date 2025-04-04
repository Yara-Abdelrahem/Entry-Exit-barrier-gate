package com.mycompany.exit_entry;

import exit_entry.dal.CARDTO;
import exit_entry.dal.DAO;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Server {

    private ServerSocket serverSocket;
    private garComm c;
    private volatile boolean currentEntryState;
    private volatile boolean previousEntryState = false;
    private volatile boolean currentExitState;
    private volatile boolean previousExitState = false;
    private static final Object lock = new Object(); // Lock object for synchronization
    private static int[] idCarMapping = new int[3]; // Shared mapping

    public Server() {
        try {
            c = garComm.getInstance();
            c.start();
            serverSocket = new ServerSocket(5005);
            // Start the client-handling thread separately.
            new Thread(this::handleClients).start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void startServer() {
        while (true) {
            // Poll entry/exit states
            currentEntryState = c.getEntryState();
            currentExitState = c.getExitState();

            // Handle entry events
            if (currentEntryState && (currentEntryState != previousEntryState)) {
                new Thread(() -> {
                    c.sendCommand(c.OPEN_ENTRY_GATE_CMD);
                    try {
                        Thread.sleep(5000);
                    } catch (InterruptedException ex) {
                        Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    c.sendCommand(c.CLOSE_ENTRY_GATE_CMD);
                    Timestamp timeStamp = new Timestamp(System.currentTimeMillis());
                    CARDTO newCar = new CARDTO(timeStamp); // Ensure appropriate constructor exists
                    
                    try {
                        DAO.InsertCar(newCar);
                    } catch (SQLException ex) {
                        Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    synchronized (lock) {
                        // Find the first available spot that is both logically and physically free
                        for (int i = 0; i < idCarMapping.length; i++) {
                            if (idCarMapping[i] == 0 && !c.getSpotState(i)) {
                                idCarMapping[i] = newCar.getCarID();
                                System.out.println("entry: " + idCarMapping[i]);
                                break;
                            }
                        }
                    }
                }).start();
            }
                
            // Handle exit events
            if (currentExitState && (currentExitState != previousExitState)) {
                new Thread(() -> {
                    c.sendCommand(c.OPEN_EXIT_GATE_CMD);
                    try {
                        Thread.sleep(5000);
                    } catch (InterruptedException ex) {
                        Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    c.sendCommand(c.CLOSE_EXIT_GATE_CMD);

                    synchronized (lock) {
                        for (int i = 0; i < idCarMapping.length; i++) {
                            // Check if a car exists and the spot is now free
                            if (idCarMapping[i] != 0 && !c.getSpotState(i)) {
                                CARDTO newCar = new CARDTO(idCarMapping[i]);
                                try {
                                    DAO.SetOutTimeStamp(newCar);
                                } catch (SQLException ex) {
                                    Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
                                }
                                idCarMapping[i] = 0;
                            }
                        }
                    }
                }).start();
            }
// uncomment this 
//            previousEntryState = currentEntryState;
//            previousExitState = currentExitState;

            // Small delay to prevent busy looping
            try {
                Thread.sleep(100);
            } catch (InterruptedException ex) {
                Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    // Remaining methods (handleClients, handleClient, main) remain unchanged as per original code
    // ...

    private void handleClients() {
        while (true) {
            try {
                Socket clientSocket = serverSocket.accept();
                new Thread(() -> handleClient(clientSocket)).start();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void handleClient(Socket socket) {
        try (BufferedReader dis = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintStream ps = new PrintStream(socket.getOutputStream())) {
            String command;
            while ((command = dis.readLine()) != null) {
                if ("History".equalsIgnoreCase(command)) {
                    String[][] arrStr = null;
                    try {
                        arrStr = DAO.GetAllCars();
                    } catch (SQLException ex) {
                        Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    if (arrStr != null) {
                        ps.println(arrStr.length);
                        for (String[] car : arrStr) {
                            for (int i = 0; i < 3; i++) {
                                ps.println(car[i]);
                            }
                        }
                    } else {
                        ps.println("0");
                    }
                } else if ("OpenEntry".equalsIgnoreCase(command)) {
                    c.sendCommand(c.OPEN_ENTRY_GATE_CMD);
                    ps.println("OpenEntry command executed.");
                } else if ("CloseEntry".equalsIgnoreCase(command)) {
                    c.sendCommand(c.CLOSE_ENTRY_GATE_CMD);
                    ps.println("CloseEntry command executed.");
                } else if ("OpenExit".equalsIgnoreCase(command)) {
                    c.sendCommand(c.OPEN_EXIT_GATE_CMD);
                    ps.println("OpenExit command executed.");
                } else if ("CloseExit".equalsIgnoreCase(command)) {
                    c.sendCommand(c.CLOSE_EXIT_GATE_CMD);
                    ps.println("CloseExit command executed.");
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                socket.close();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        Server server = new Server();
        server.startServer();
    }
}