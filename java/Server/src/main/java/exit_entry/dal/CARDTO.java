/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package exit_entry.dal;
import java.sql.Timestamp;

/**
 *
 * @author youssef
 */
public class CARDTO {
    private int CarID;
    private Timestamp CarInTimeStamp;
    private Timestamp CarOutTimeStamp;

    public CARDTO(int CarID, Timestamp CarInTimeStamp) {
        this.CarID = CarID;
        this.CarInTimeStamp = CarInTimeStamp;
    }
    
    public CARDTO( Timestamp CarInTimeStamp) {
        this.CarInTimeStamp = CarInTimeStamp;
    }
    
    public CARDTO(int CarID) {
        this.CarID = CarID;
    }
    

    public int getCarID() {
        return CarID;
    }

    public Timestamp getCarInTimeStamp() {
        return CarInTimeStamp;
    }

    public Timestamp getCarOutTimeStamp() {
        return CarOutTimeStamp;
    }

    public void setCarID(int CarID) {
        this.CarID = CarID;
    }

    public void setCarInTimeStamp(Timestamp CarInTimeStamp) {
        this.CarInTimeStamp = CarInTimeStamp;
    }

    public void setCarOutTimeStamp(Timestamp CarOutTimeStamp) {
        this.CarOutTimeStamp = CarOutTimeStamp;
    }
}
