/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.exit_entry;
import java.util.Vector;
import java.time.LocalDateTime;

/**
 *
 * @author adhamwalaa
 */
public class carLog {
    private String state;
    private LocalDateTime time;
    
    public carLog(String state,LocalDateTime time){
        this.state = state;
        this.time = time;
    }
}
