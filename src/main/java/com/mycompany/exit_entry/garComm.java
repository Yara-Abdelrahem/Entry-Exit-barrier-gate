
package com.mycompany.exit_entry;

import com.fazecast.jSerialComm.SerialPort;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 *
 * @author adhamwalaa
 */
public class garComm implements Communication {
    
    private final static byte START_BYTE = (byte)0xAA;
    private final static byte END_BTYE = (byte)0x55;
    
    private SerialPort port;
    private OutputStream outStream;
    private InputStream inStream;
    private boolean connected;
    private byte connectionAttempts;
    private final byte maxConnectionAttempts = 5;
    private static garComm instance;
   
    private static final Object inStreamLock = new Object();
    
    static{
        instance = null;
    }
    
    /*False->not available True->Available*/
    private boolean[] spots;
    
    public static garComm getInstance(){
        if(instance == null){
            instance = new garComm();
        }
        return instance;
    }
    
    private boolean connectToStm32(){
        SerialPort[] portNames = SerialPort.getCommPorts();
        System.out.println("Available Ports:");
        for (SerialPort port : portNames) {
            System.out.println(port.getSystemPortName());
        }
        if(portNames.length == 0){
            return false;
        }
        
        port = portNames[0];
        port.setBaudRate(9600);
        port.setNumDataBits(8);
        port.setNumStopBits(1);
        port.setParity(SerialPort.NO_PARITY);
        port.setComPortTimeouts(SerialPort.TIMEOUT_SCANNER, 0, 0);
        if(!port.openPort()){
            return false;
        }
        
        outStream = port.getOutputStream();
        inStream = port.getInputStream();
       
        return true;
    }
    
    
    private garComm(){
        connectionAttempts = 0;
        while((connectionAttempts<maxConnectionAttempts)&&(!connected)){
            connected = connectToStm32();
            connectionAttempts++;
        }
        
        if(connected){
            System.out.println("Connected to STM32 Successfully on: " +  port.getSystemPortName());
        }
        else{
            System.out.println("Error Connecting to STM32");
        }
        spots = new boolean[3];
    }
    
    public synchronized boolean send(byte[] command){
        if((!connected) || (outStream == null)){
            return false;
        }
        
        try{   
            outStream.write(command, 0, command.length);
            outStream.flush();
        }
        catch(IOException ex){
            ex.printStackTrace();
        }
        catch(NullPointerException ex){
            ex.printStackTrace();
        }
        catch(IndexOutOfBoundsException ex){
            ex.printStackTrace();
        }
        return true;
    }
    
    public synchronized boolean receive(byte[] data){
        
        if((!connected) || (inStream == null)){
            return false;
        }
        
        try{   
            int noOfBytes = inStream.read(data, 0, data.length);
            if(noOfBytes == -1){
                return false;
            }
        }
        catch(IOException ex){
            ex.printStackTrace();
        }
        catch(NullPointerException ex){
            ex.printStackTrace();
        }
        catch(IndexOutOfBoundsException ex){
            ex.printStackTrace();
        }
        return true;
    }    
    
    private byte[] encodeCommand(byte command){
        byte[] encodedCMD = new byte[4];
        encodedCMD[0] = START_BYTE;
        encodedCMD[1] = command;
        byte[] checkSumArr = {START_BYTE,command};
        encodedCMD[2] = calcCheckSum(checkSumArr);
        encodedCMD[3] = END_BTYE;
        return encodedCMD;
    }
    
    private void sendCommand(byte command){
        byte[] encodedCMD = encodeCommand(command);
        try{
            outStream.write(encodedCMD, 0, 4);
            outStream.flush();
        }
        catch(IndexOutOfBoundsException ex){
            ex.printStackTrace();
        }
        catch(IOException ex){
            ex.printStackTrace();
        }
    }
    
    private boolean processCarSpotsData(byte[] data){
        byte[] checkSumArr = new byte[2];
        byte checkSumVal = 0;
        if(data.length < 4){
            System.out.println("Error in length of received data");
            return false;
        }
        try{
            checkSumArr[0] = data[0];
            checkSumArr[1] = data[1];
            checkSumVal = calcCheckSum(data);
            /*if(checkSumVal != data[2]){
                System.out.println("Error in received frame");
                return false;
            }
            else if(data[0] != START_BYTE){
                System.out.println("Error in received frame");
                return false;    
            }
            else if(data[3] != END_BTYE){
                System.out.println("Error in received frame");
                return false; 
            }
            else{*/
                System.err.println("Car Spots: ");
                for(int i = 0;i<spots.length;i++){
                    spots[i] = (((0x01 << i) & (data[1])) != (1 << i));
                    System.out.println("Car Spot " + i +" " +spots[i]);
                }
                System.err.println();
            //}
        }
        catch(IndexOutOfBoundsException ex){
            ex.printStackTrace();
        }
        catch(NullPointerException ex){
            ex.printStackTrace();
        }
        return true;
    }
    
    public void updateCarSpots(){
        Thread th = new Thread(){
            public void run(){
                byte[] data = new byte[4];
                while(connected){ 
                    try{
                        Thread.sleep(1000);
                    }
                    catch(InterruptedException ex){
                        ex.printStackTrace();
                    }
                    sendCommand(GET_PARKING_SPOTS_CMD);
                    if(receive(data)){
                        processCarSpotsData(data);
                    }
                }
            }
        };
        th.start();
    }
    
    public void start(){
        updateCarSpots();
    }
    
    private byte calcCheckSum(byte[] data){
        byte sum = 0;
        for(byte b: data){
            sum ^= b;
        }
        return sum;
    }
}
