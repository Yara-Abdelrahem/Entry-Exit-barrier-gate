/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Interface.java to edit this template
 */
package com.mycompany.exit_entry;

/**
 *
 * @author adhamwalaa
 */
public interface Communication {
    
    public final static byte GET_PARKING_SPOTS_CMD = (byte)0x11;
    public final static byte OPEN_ENTRY_GATE_CMD = (byte)0x22;
    public final static byte OPEN_EXIT_GATE_CMD = (byte)0x33;
    

    
    boolean send(byte[] command);
    boolean receive(byte[] data);
    void start();
}
