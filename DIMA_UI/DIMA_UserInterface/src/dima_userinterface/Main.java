/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package dima_userinterface;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JDialog;
import javax.swing.JOptionPane;
import jssc.SerialPort;
import jssc.SerialPortException;
import jssc.SerialPortList;


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
