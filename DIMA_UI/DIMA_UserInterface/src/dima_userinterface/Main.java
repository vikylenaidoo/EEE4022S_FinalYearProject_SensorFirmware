/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package dima_userinterface;

import java.nio.ByteBuffer;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JDialog;
import javax.swing.JOptionPane;
import jssc.SerialPort;
import jssc.SerialPortException;
import jssc.SerialPortList;
import jssc.SerialPortTimeoutException;


/**
 *
 * @author vikylenaidoo
 */
public class Main {
   
    
    public static void main(String[] args){
        
        MainGUI GUI = new MainGUI();
        GUI.setVisible(true);
        
        
       
        
        try {
            SerialPort port = GUI.getSerialPort();
            

            
            while(!GUI.isExit){ //10Hz loop to send serial message
                Thread.sleep(100, 0);
                
                if(port == null){
                    break;
                }
                else if(port.isOpened()){
                    if(GUI.sendError){
                        GUI.sendError = false;
                        new JOptionPane().showMessageDialog(GUI, "Error occured when sending command, please try again");
                        continue;
                    }
                    
                    if(GUI.isNewMessageAvailable){ //send message
                        boolean w = port.writeBytes(GUI.message);

                        /*if(w){
                            for(int i=0; i<GUI.message.length; i++){
                                System.out.print(GUI.message[i]+"/");
                                //System.out.print("");
                            }
                            System.out.println("");
                        }*/
                        
                        GUI.isNewMessageAvailable = false;
                    }
                    else{//send heartbeat message
                        boolean w = port.writeBytes(GUI.heartbeat);
                        /*if(w){
                            System.out.println(LocalDateTime.now());
                        }*/
                    }
                    
                    try {
                        //try read any data from the serial port
                        byte[] read_buffer = port.readBytes(68, 10);
                        
                        /*for(int i=0; i<4; i++){
                            System.out.printf("%s \t", Integer.toHexString(Byte.toUnsignedInt(read_buffer[i+64])));
                        }
                        System.out.println("");*/
                        
                        GUI.updateOutput(read_buffer);
                        
                        
                        
                    } catch (SerialPortTimeoutException ex) {
                        //Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
                        //no data to be read
                    }
                }
            }
            
            if(GUI.isExit){
                System.out.println("------------exiting------------");
            }
            
            //close serial port
            //port.closePort();
            
            
        } catch (SerialPortException ex) {
            Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
            //System.out.println("main " + ex.toString());
        } catch (InterruptedException ex){
            System.out.println("main: " + ex.toString());
        }
        
        
        
        
        
        
        
    }
    
   
    
}
