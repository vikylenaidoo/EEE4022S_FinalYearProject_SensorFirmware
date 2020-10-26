/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package dima_userinterface;

import java.nio.ByteBuffer;
import java.util.logging.Level;
import java.util.logging.Logger;
import jssc.*;
import java.lang.IndexOutOfBoundsException;
import java.nio.ByteOrder;
import java.util.zip.CRC32;
import java.util.zip.Checksum;


/**
 *
 * @author vikylenaidoo
 */
public class MainGUI extends javax.swing.JFrame {

    //----------------------------CONSTANTS----------------------------//
    public static final int CRC32_POLY = 0x4C11DB7;
    public static final int CRC32_RESET = 0xFFFFFFFF;
    
    
    //-----------------------------ATTRIBUTES--------------------------------//
    public byte[] message;
    public byte[] heartbeat;
    /** structure of message
     * +----------------------------+
     * |$   |type   |data   |crc32  |
     * +----------------------------+
     * 1    1       4       4           bytes
     * 
     * message types:
     * 0    heartbeat
     * 1    kill
     * 2    revive
     * 3    throttle
     * 4    steering
     * 5    tail
     * 6    @todo include control commands
     * 7
     */
    
    public boolean isNewMessageAvailable;
    public boolean isExit;
    public boolean sendError;
    private SerialPort port;
    
    
    //------------------------------PUBLIC FUNCTIONS---------------------------//
    /*update the display data from the sensors*/
    public void updateOutput(byte[] data){
        if(data.length == 68){
            //check crc32
            Checksum crc32 = new CRC32();
            crc32.update(data, 0, 64);
            
            ByteBuffer bb = ByteBuffer.wrap(data, 64, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            int crc_rx = bb.getInt();
            
            if( crc_rx != (int)crc32.getValue()){
                System.out.printf("invalid data received: %x \t %x \n", crc32.getValue(), crc_rx);
                return;
            }
            else{
                //System.out.printf("valid data received: %x \t %x \n", crc32.getValue(), crc_rx);
            }
            
            
            //display update
            int i = 0;
            
            /*-------------------Acc: 2bytes 2s complement ------------------*/
            bb = ByteBuffer.wrap(data, i, 2);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            String out = String.format("%d \n", bb.getShort());
            txfAccX.setText(out);
            //txfAccX.setText(""+bb.getShort());
            i+=2;
                        
            bb = ByteBuffer.wrap(data, i, 2);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getShort());
            txfAccY.setText(out);
            i+=2;
            
            bb = ByteBuffer.wrap(data, i, 2);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getShort());            
            txfAccZ.setText(out);
            i+=2;
            
            /*------------gyro: 2bytes 2s complement-----------------*/
            bb = ByteBuffer.wrap(data, i, 2);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getShort());            
            txfGyroX.setText(out);
            i+=2;
                        
            bb = ByteBuffer.wrap(data, i, 2);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getShort());
            txfGyroY.setText(out);
            i+=2;
            
            bb = ByteBuffer.wrap(data, i, 2);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getShort());           
            txfGyroZ.setText(out);
            i+=2;
            
            /*------------mag 2 bytes each---------------------*/
            bb = ByteBuffer.wrap(data, i, 2);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getShort());            
            txfMagX.setText(out);
            i+=2;
            
            bb = ByteBuffer.wrap(data, i, 2);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getShort());            
            txfMagY.setText(out);
            i+=2;
            
            bb = ByteBuffer.wrap(data, i, 2);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getShort());            
            txfMagZ.setText(out);
            i+=2;
            
            bb = ByteBuffer.wrap(data, i, 2);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out  = Integer.toUnsignedString(bb.getShort()); //= String.format("%d \n", bb.getShort()); //unsigned           
            txfMagH.setText(out);
            i+=2;
            
            /*--------------baroPressure 4 bytes-------------*/
            bb = ByteBuffer.wrap(data, i, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getInt());            
            txfPress.setText(out);
            i+=4;
            
            /*--------------baro temp 4 bytes-------------*/
            bb = ByteBuffer.wrap(data, i, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getInt());            
            txfTemp.setText(out);
            i+=4;
            
            /*--------------GNSS data----------------*/
            
            //lat 4 bytes
            bb = ByteBuffer.wrap(data, i, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%f \n", bb.getInt()*1e-7);            
            txfLat.setText(out);
            i+=4;

            //lon 4 bytes
            bb = ByteBuffer.wrap(data, i, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%f \n", bb.getInt() *1e-7);            
            txfLong.setText(out);
            i+=4;  
            
            //velN 4 bytes
            bb = ByteBuffer.wrap(data, i, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getInt());            
            txfVelN.setText(out);
            i+=4;
            
            //velE 4 bytes
            bb = ByteBuffer.wrap(data, i, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getInt());            
            txfVelE.setText(out);
            i+=4;
            
            //velD 4 bytes
            bb = ByteBuffer.wrap(data, i, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getInt());            
            txfVelD.setText(out);
            i+=4;
            
            //height 4 bytes
            bb = ByteBuffer.wrap(data, i, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getInt());            
            txfHeight.setText(out);
            i+=4;
            
            //hmsl 4 bytes
            bb = ByteBuffer.wrap(data, i, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getInt());            
            txfHmsl.setText(out);
            i+=4;
            
            //gspeed 4 bytes
            bb = ByteBuffer.wrap(data, i, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%d \n", bb.getInt());            
            txfGSpeed.setText(out);
            i+=4;
            
            //heading 4 bytes
            bb = ByteBuffer.wrap(data, i, 4);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            out = String.format("%f \n", bb.getInt()*1e-5);            
            txfHeading.setText(out);
            i+=4;


            
            
            
        }
        else{
            System.out.println("Data length incorrect!!!");
        }
    }
    
    
    private void createHeartbeat(){
        this.heartbeat = new byte[10];
        heartbeat[0] = '$';
        heartbeat[1] = 0; //message type
        
        for(int i=2;i<6; i++){ //data=0
            heartbeat[i] = 0;
        }
        
        //crc32
        Checksum crc32 = new CRC32();
        crc32.update(heartbeat, 1, 5);

        ByteBuffer bb = ByteBuffer.allocate(4);
        byte[] crcArray = bb.putInt((int)crc32.getValue()).array();
        for(int i=0; i<4; i++){
            heartbeat[i+6] = crcArray[i];
        }
    }
    
    public SerialPort getSerialPort(){
        return this.port;
    }
    
    public void initSerial(){
        //serial setup
        String[] portNames = SerialPortList.getPortNames();
        cmbSerial.removeAllItems();
        for(int i = 0; i < portNames.length; i++){
            System.out.println(portNames[i]);
            cmbSerial.addItem(portNames[i]);
        }
        
        try{
            //open an configure serial port
            this.port = new SerialPort(portNames[0]);//new SerialPort("/dev/tty0")
            port.openPort();
            port.setParams(port.BAUDRATE_9600, port.DATABITS_8, port.STOPBITS_1, port.PARITY_NONE, false, false);
           
        }
        catch(SerialPortException e){
            System.out.println("initialise serial \n" + e.toString());
        } 
        catch(IndexOutOfBoundsException e){
            System.out.println("initialise serial \n" + e.toString());
        }
    }
    
    //-------------------------------CONSTRUCTOR-------------------------------//
    /**
     * Creates new form MainGUI
     */
    public MainGUI() {
        
        try {
            //javax.swing.UIManager.setLookAndFeel(javax.swing.UIManager.getSystemLookAndFeelClassName());
            javax.swing.UIManager.setLookAndFeel(javax.swing.UIManager.getCrossPlatformLookAndFeelClassName());
             
        } catch (Exception ex) {
            java.util.logging.Logger.getLogger(MainGUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } 
        
        initComponents();
        message = new byte[10];
        createHeartbeat();
        isNewMessageAvailable = false;
        isExit = false;
        initSerial();
        
    }
    
    
    //----------------------------PRIVATE FUNCTIONS-----------------------------//

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jMenu1 = new javax.swing.JMenu();
        btnGrpManual = new javax.swing.ButtonGroup();
        jPanel3 = new javax.swing.JPanel();
        jPanel4 = new javax.swing.JPanel();
        jPanel9 = new javax.swing.JPanel();
        jLabel23 = new javax.swing.JLabel();
        jLabel22 = new javax.swing.JLabel();
        jLabel1 = new javax.swing.JLabel();
        jTabbedPane1 = new javax.swing.JTabbedPane();
        jPanel1 = new javax.swing.JPanel();
        sldManual = new javax.swing.JSlider();
        rbtnThrottle = new javax.swing.JRadioButton();
        rbtnSteering = new javax.swing.JRadioButton();
        rbtnTail = new javax.swing.JRadioButton();
        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        lblSliderManual = new javax.swing.JLabel();
        jSeparator2 = new javax.swing.JSeparator();
        jPanel2 = new javax.swing.JPanel();
        jLabel11 = new javax.swing.JLabel();
        jSeparator3 = new javax.swing.JSeparator();
        jPanel5 = new javax.swing.JPanel();
        btnKill = new javax.swing.JButton();
        btnRevive = new javax.swing.JButton();
        btnSend = new javax.swing.JButton();
        jPanel6 = new javax.swing.JPanel();
        jLabel5 = new javax.swing.JLabel();
        jLabel6 = new javax.swing.JLabel();
        txfGyroZ = new javax.swing.JTextField();
        txfGyroX = new javax.swing.JTextField();
        txfGyroY = new javax.swing.JTextField();
        jLabel7 = new javax.swing.JLabel();
        txfAccX = new javax.swing.JTextField();
        txfAccY = new javax.swing.JTextField();
        txfAccZ = new javax.swing.JTextField();
        jLabel8 = new javax.swing.JLabel();
        txfMagX = new javax.swing.JTextField();
        txfMagY = new javax.swing.JTextField();
        txfMagZ = new javax.swing.JTextField();
        jSeparator1 = new javax.swing.JSeparator();
        jLabel9 = new javax.swing.JLabel();
        txfLat = new javax.swing.JTextField();
        txfLong = new javax.swing.JTextField();
        jLabel10 = new javax.swing.JLabel();
        txfVelN = new javax.swing.JTextField();
        txfVelE = new javax.swing.JTextField();
        txfVelD = new javax.swing.JTextField();
        jLabel12 = new javax.swing.JLabel();
        txfHeight = new javax.swing.JTextField();
        jLabel13 = new javax.swing.JLabel();
        txfHmsl = new javax.swing.JTextField();
        jLabel14 = new javax.swing.JLabel();
        txfGSpeed = new javax.swing.JTextField();
        jLabel15 = new javax.swing.JLabel();
        txfHeading = new javax.swing.JTextField();
        jSeparator4 = new javax.swing.JSeparator();
        jLabel16 = new javax.swing.JLabel();
        txfPress = new javax.swing.JTextField();
        jLabel17 = new javax.swing.JLabel();
        txfTemp = new javax.swing.JTextField();
        jLabel18 = new javax.swing.JLabel();
        jLabel19 = new javax.swing.JLabel();
        jLabel20 = new javax.swing.JLabel();
        jLabel21 = new javax.swing.JLabel();
        jLabel25 = new javax.swing.JLabel();
        jLabel27 = new javax.swing.JLabel();
        txfMagH = new javax.swing.JTextField();
        jPanel8 = new javax.swing.JPanel();
        cmbSerial = new javax.swing.JComboBox<>();
        btnSerial = new javax.swing.JButton();

        jMenu1.setText("jMenu1");

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosed(java.awt.event.WindowEvent evt) {
                formWindowClosed(evt);
            }
        });

        jPanel3.setBackground(new java.awt.Color(8, 134, 186));

        jPanel4.setBackground(new java.awt.Color(254, 254, 254));

        jLabel23.setIcon(new javax.swing.ImageIcon(getClass().getResource("/image/DIMA2.jpg"))); // NOI18N

        javax.swing.GroupLayout jPanel9Layout = new javax.swing.GroupLayout(jPanel9);
        jPanel9.setLayout(jPanel9Layout);
        jPanel9Layout.setHorizontalGroup(
            jPanel9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel9Layout.createSequentialGroup()
                .addComponent(jLabel23)
                .addGap(0, 0, Short.MAX_VALUE))
        );
        jPanel9Layout.setVerticalGroup(
            jPanel9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jLabel23, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );

        jLabel22.setIcon(new javax.swing.ImageIcon(getClass().getResource("/image/logo.png"))); // NOI18N
        jLabel22.setAlignmentX(0.5F);

        jLabel1.setFont(new java.awt.Font("Ubuntu", 3, 24)); // NOI18N
        jLabel1.setForeground(new java.awt.Color(255, 113, 0));
        jLabel1.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel1.setText("DIMA USER CONTROL APP");

        javax.swing.GroupLayout jPanel4Layout = new javax.swing.GroupLayout(jPanel4);
        jPanel4.setLayout(jPanel4Layout);
        jPanel4Layout.setHorizontalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel22, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGap(18, 18, 18)
                .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 305, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(jPanel9, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        jPanel4Layout.setVerticalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel22, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
            .addComponent(jPanel9, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addGap(26, 26, 26)
                .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 64, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        sldManual.setMajorTickSpacing(1);
        sldManual.setSnapToTicks(true);
        sldManual.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                sldManualStateChanged(evt);
            }
        });

        btnGrpManual.add(rbtnThrottle);
        rbtnThrottle.setText("Throttle");

        btnGrpManual.add(rbtnSteering);
        rbtnSteering.setText("Steering");
        rbtnSteering.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rbtnSteeringActionPerformed(evt);
            }
        });

        btnGrpManual.add(rbtnTail);
        rbtnTail.setText("Tail");

        jLabel2.setText("MANUAL MODE: Send commands to directly control the robot");

        jLabel3.setText("100");

        jLabel4.setText("0");

        lblSliderManual.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        lblSliderManual.setLabelFor(sldManual);
        lblSliderManual.setText(Integer.toString(sldManual.getValue()));

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addComponent(jLabel4, javax.swing.GroupLayout.PREFERRED_SIZE, 31, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                                .addComponent(jLabel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addGroup(jPanel1Layout.createSequentialGroup()
                                    .addComponent(rbtnThrottle)
                                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 48, Short.MAX_VALUE)
                                    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                        .addComponent(rbtnSteering)
                                        .addGroup(jPanel1Layout.createSequentialGroup()
                                            .addGap(15, 15, 15)
                                            .addComponent(lblSliderManual)))
                                    .addGap(44, 44, 44)
                                    .addComponent(rbtnTail, javax.swing.GroupLayout.PREFERRED_SIZE, 113, javax.swing.GroupLayout.PREFERRED_SIZE)))
                            .addComponent(sldManual, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 464, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(1, 1, 1)
                        .addComponent(jLabel3, javax.swing.GroupLayout.PREFERRED_SIZE, 31, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jSeparator2, javax.swing.GroupLayout.Alignment.TRAILING))
                .addContainerGap())
        );

        jPanel1Layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {rbtnSteering, rbtnTail, rbtnThrottle});

        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGap(25, 25, 25)
                .addComponent(jLabel2, javax.swing.GroupLayout.PREFERRED_SIZE, 44, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jSeparator2, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel1Layout.createSequentialGroup()
                                .addGap(36, 36, 36)
                                .addComponent(jLabel3))
                            .addGroup(jPanel1Layout.createSequentialGroup()
                                .addGap(35, 35, 35)
                                .addComponent(jLabel4)))
                        .addGap(81, 81, 81))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
                        .addGap(13, 13, 13)
                        .addComponent(sldManual, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(lblSliderManual)
                        .addGap(33, 33, 33)))
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(rbtnThrottle)
                    .addComponent(rbtnSteering)
                    .addComponent(rbtnTail))
                .addContainerGap(73, Short.MAX_VALUE))
        );

        jPanel1Layout.linkSize(javax.swing.SwingConstants.VERTICAL, new java.awt.Component[] {rbtnSteering, rbtnTail, rbtnThrottle});

        rbtnThrottle.setSelected(true);

        jTabbedPane1.addTab("Manual Mode", jPanel1);

        jLabel11.setText("CONTROL MODE: Change the setpoint for the controller");

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(65, 65, 65)
                .addComponent(jLabel11, javax.swing.GroupLayout.DEFAULT_SIZE, 431, Short.MAX_VALUE)
                .addGap(67, 67, 67))
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jSeparator3)
                .addContainerGap())
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(22, 22, 22)
                .addComponent(jLabel11, javax.swing.GroupLayout.PREFERRED_SIZE, 44, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jSeparator3, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(241, Short.MAX_VALUE))
        );

        jTabbedPane1.addTab("Control Mode", jPanel2);

        btnKill.setBackground(new java.awt.Color(254, 95, 95));
        btnKill.setText("KILL");
        btnKill.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnKillActionPerformed(evt);
            }
        });

        btnRevive.setBackground(new java.awt.Color(107, 220, 254));
        btnRevive.setText("REVIVE");
        btnRevive.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnReviveActionPerformed(evt);
            }
        });

        btnSend.setBackground(new java.awt.Color(124, 251, 115));
        btnSend.setText("SEND COMMAND");
        btnSend.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSendActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel5Layout = new javax.swing.GroupLayout(jPanel5);
        jPanel5.setLayout(jPanel5Layout);
        jPanel5Layout.setHorizontalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel5Layout.createSequentialGroup()
                .addGap(31, 31, 31)
                .addComponent(btnSend, javax.swing.GroupLayout.PREFERRED_SIZE, 198, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(btnKill, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(btnRevive, javax.swing.GroupLayout.DEFAULT_SIZE, 140, Short.MAX_VALUE))
                .addGap(60, 60, 60))
        );
        jPanel5Layout.setVerticalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel5Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel5Layout.createSequentialGroup()
                        .addComponent(btnKill, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(16, 16, 16)
                        .addComponent(btnRevive, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(btnSend, javax.swing.GroupLayout.PREFERRED_SIZE, 90, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(22, Short.MAX_VALUE))
        );

        jPanel6.setBackground(new java.awt.Color(254, 202, 239));

        jLabel5.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel5.setText("ON BOARD DATA");

        jLabel6.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel6.setText("GYRO X:Y:Z");

        txfGyroZ.setText("jTextField1");

        txfGyroX.setText("jTextField1");
        txfGyroX.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                txfGyroXActionPerformed(evt);
            }
        });

        txfGyroY.setText("jTextField1");

        jLabel7.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel7.setText("ACC X:Y:Z");

        txfAccX.setText("jTextField1");

        txfAccY.setText("jTextField1");

        txfAccZ.setText("jTextField1");

        jLabel8.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel8.setText("MAG X:Y:Z");

        txfMagX.setText("jTextField1");

        txfMagY.setText("jTextField1");

        txfMagZ.setText("jTextField1");
        txfMagZ.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                txfMagZActionPerformed(evt);
            }
        });

        jLabel9.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel9.setText("LAT:LON");

        txfLat.setText("jTextField1");

        txfLong.setText("jTextField1");

        jLabel10.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel10.setText("VEL N:E:D");

        txfVelN.setText("jTextField1");

        txfVelE.setText("jTextField1");

        txfVelD.setText("jTextField1");

        jLabel12.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel12.setText("HEIGHT");

        txfHeight.setText("jTextField1");

        jLabel13.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel13.setText("HMSL");

        txfHmsl.setText("jTextField1");

        jLabel14.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel14.setText("GROUND SPEED");

        txfGSpeed.setText("jTextField1");

        jLabel15.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel15.setText("HEADING");

        txfHeading.setText("jTextField1");

        jSeparator4.setOrientation(javax.swing.SwingConstants.VERTICAL);

        jLabel16.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel16.setText("BARO PRESSURE");

        txfPress.setText("jTextField1");

        jLabel17.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel17.setText("TEMP");

        txfTemp.setText("jTextField1");

        jLabel18.setText("jLabel18");

        jLabel19.setText("jLabel18");

        jLabel20.setText("jLabel18");

        jLabel21.setText("jLabel18");

        jLabel25.setText("jLabel18");

        jLabel27.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        jLabel27.setText("MAG HALL");

        txfMagH.setText("jTextField1");

        javax.swing.GroupLayout jPanel6Layout = new javax.swing.GroupLayout(jPanel6);
        jPanel6.setLayout(jPanel6Layout);
        jPanel6Layout.setHorizontalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel6Layout.createSequentialGroup()
                .addGap(12, 12, 12)
                .addComponent(jSeparator1)
                .addGap(12, 12, 12))
            .addGroup(jPanel6Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(jLabel6, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 104, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jLabel7, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 104, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jLabel8, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 104, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jLabel9, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 104, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jLabel10, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 104, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jLabel16, javax.swing.GroupLayout.PREFERRED_SIZE, 104, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel12, javax.swing.GroupLayout.PREFERRED_SIZE, 104, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel14, javax.swing.GroupLayout.PREFERRED_SIZE, 104, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel27, javax.swing.GroupLayout.PREFERRED_SIZE, 104, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel6Layout.createSequentialGroup()
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel6Layout.createSequentialGroup()
                                .addGap(60, 60, 60)
                                .addComponent(jLabel5, javax.swing.GroupLayout.PREFERRED_SIZE, 140, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(txfMagH, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(txfGSpeed, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(txfHeight, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(txfPress, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(txfGyroX, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(24, 24, 24)
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(txfGyroZ, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfAccZ, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfMagZ, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfVelD, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfTemp, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfHmsl, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfHeading, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(jPanel6Layout.createSequentialGroup()
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(txfVelN)
                            .addComponent(txfAccX)
                            .addComponent(txfMagX)
                            .addComponent(txfLat))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(txfGyroY)
                            .addComponent(txfAccY)
                            .addComponent(txfMagY)
                            .addComponent(txfVelE)
                            .addGroup(jPanel6Layout.createSequentialGroup()
                                .addGap(2, 2, 2)
                                .addComponent(jSeparator4, javax.swing.GroupLayout.PREFERRED_SIZE, 12, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(jLabel17, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 63, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel13, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 63, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel15, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 63, javax.swing.GroupLayout.PREFERRED_SIZE)))
                            .addComponent(txfLong))))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel18)
                    .addComponent(jLabel19)
                    .addComponent(jLabel20)
                    .addComponent(jLabel21)
                    .addComponent(jLabel25))
                .addGap(0, 0, Short.MAX_VALUE))
        );

        jPanel6Layout.linkSize(javax.swing.SwingConstants.HORIZONTAL, new java.awt.Component[] {txfAccX, txfAccZ, txfGSpeed, txfGyroX, txfGyroY, txfGyroZ, txfHeading, txfHeight, txfHmsl, txfMagH, txfMagZ, txfPress, txfTemp, txfVelD});

        jPanel6Layout.setVerticalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel6Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel5)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jSeparator1, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(30, 30, 30)
                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel6Layout.createSequentialGroup()
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jLabel6, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfGyroX, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel18)
                            .addComponent(txfGyroY, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfGyroZ, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jLabel7, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfAccX, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel19)
                            .addComponent(txfAccY, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfAccZ, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jLabel8, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfMagX, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel20)
                            .addComponent(txfMagY, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfMagZ, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jLabel9, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfLat, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel25)
                            .addComponent(txfLong, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jLabel10, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfVelN, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel21)
                            .addComponent(txfVelE, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txfVelD, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel6Layout.createSequentialGroup()
                                .addGap(26, 26, 26)
                                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(jLabel16, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(txfPress, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(txfTemp, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(jLabel12, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(txfHeight, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(txfHmsl, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(jLabel14, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(txfGSpeed, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(txfHeading, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(jLabel27, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(txfMagH, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addContainerGap(56, Short.MAX_VALUE))
                            .addGroup(jPanel6Layout.createSequentialGroup()
                                .addGap(18, 18, 18)
                                .addComponent(jSeparator4)
                                .addContainerGap())))
                    .addGroup(jPanel6Layout.createSequentialGroup()
                        .addGap(230, 230, 230)
                        .addComponent(jLabel17, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel13, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel15, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addContainerGap())))
        );

        cmbSerial.setModel(new javax.swing.DefaultComboBoxModel<>(new String[] { "Item 1", "Item 2", "Item 3", "Item 4" }));

        btnSerial.setText("Choose Port");
        btnSerial.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSerialActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel8Layout = new javax.swing.GroupLayout(jPanel8);
        jPanel8.setLayout(jPanel8Layout);
        jPanel8Layout.setHorizontalGroup(
            jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel8Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(btnSerial, javax.swing.GroupLayout.PREFERRED_SIZE, 148, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cmbSerial, javax.swing.GroupLayout.PREFERRED_SIZE, 148, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(54, 54, 54))
        );
        jPanel8Layout.setVerticalGroup(
            jPanel8Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel8Layout.createSequentialGroup()
                .addGap(32, 32, 32)
                .addComponent(cmbSerial, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(btnSerial)
                .addGap(31, 31, 31))
        );

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel3Layout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jPanel4, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel6, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 35, Short.MAX_VALUE)
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jTabbedPane1)
                    .addComponent(jPanel5, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel8, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGap(18, 18, 18))
        );
        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel3Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jPanel8, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel4, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGap(35, 35, 35)
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel3Layout.createSequentialGroup()
                        .addComponent(jTabbedPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 367, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jPanel5, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jPanel6, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(56, 56, 56))
        );

        jTabbedPane1.getAccessibleContext().setAccessibleName("tabMode");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(jPanel3, javax.swing.GroupLayout.PREFERRED_SIZE, 728, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void btnSendActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSendActionPerformed
        // TODO add your handling code here:
        this.message[0] = '$';
        
        //set message type
        if(rbtnThrottle.isSelected()){
            this.message[1] = 3;
        }
        else if(rbtnSteering.isSelected()){
            this.message[1] = 4;
        }
        else if(rbtnTail.isSelected()){
            this.message[1] = 5;
        }
        
        //set message data
        int data = sldManual.getValue();
        ByteBuffer b = ByteBuffer.allocate(4);
        byte[] dataArray = b.putInt(data).array();
        for(int i=0; i<4; i++){
            message[i+2] = dataArray[i];
        }
        
        //crc32 checksum
        try{
            /*int crc32 = this.calculateCRC32(message, 2, 4);

            */
            
            Checksum crc32 = new CRC32();
            crc32.update(message, 1, 5);
            
            ByteBuffer bb = ByteBuffer.allocate(4);
            byte[] crcArray = bb.putInt((int)crc32.getValue()).array();
            for(int i=0; i<4; i++){
                message[i+6] = crcArray[i];
            }
            
            System.out.println(Integer.toHexString((int)crc32.getValue()));
            
            this.isNewMessageAvailable = true;
        
        }
        catch(IndexOutOfBoundsException e){
            this.sendError = true;
        }
    }//GEN-LAST:event_btnSendActionPerformed

    private void rbtnSteeringActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rbtnSteeringActionPerformed
        // TODO add your handling code here:
        
    }//GEN-LAST:event_rbtnSteeringActionPerformed

    private void sldManualStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_sldManualStateChanged
        // TODO add your handling code here:
        lblSliderManual.setText(Integer.toString(sldManual.getValue()));
    }//GEN-LAST:event_sldManualStateChanged

    private void formWindowClosed(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_formWindowClosed
        
        isExit = true;
        try {
            if(this.port != null){
                this.port.closePort();
            }
        } catch (SerialPortException ex) {
            Logger.getLogger(MainGUI.class.getName()).log(Level.SEVERE, null, ex);
        } catch (NullPointerException ex ){
            Logger.getLogger(MainGUI.class.getName()).log(Level.SEVERE, null, ex);
        }
        
    }//GEN-LAST:event_formWindowClosed

    private void btnKillActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnKillActionPerformed
        message[0] = '$';
        message[1] = 1; //message type
        for(int i=0;i<4;i++){ //data = 0
            message[i+2] = 0;
        }
        //crc32
        Checksum crc32 = new CRC32();
        crc32.update(message, 1, 5);

        ByteBuffer bb = ByteBuffer.allocate(4);
        byte[] crcArray = bb.putInt((int)crc32.getValue()).array();
        for(int i=0; i<4; i++){
            message[i+6] = crcArray[i];
        }
        
        //set flag
        isNewMessageAvailable = true;
        
    }//GEN-LAST:event_btnKillActionPerformed

    private void btnReviveActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnReviveActionPerformed
        message[0] = '$';
        message[1] = 2; //message type
        for(int i=0;i<4;i++){ //data = 0
            message[i+2] = 0;
        }
        //crc32
        Checksum crc32 = new CRC32();
        crc32.update(message, 1, 5);

        ByteBuffer bb = ByteBuffer.allocate(4);
        byte[] crcArray = bb.putInt((int)crc32.getValue()).array();
        for(int i=0; i<4; i++){
            message[i+6] = crcArray[i];
        }
        //set flag
        isNewMessageAvailable = true;
    }//GEN-LAST:event_btnReviveActionPerformed

    private void txfMagZActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_txfMagZActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_txfMagZActionPerformed

    private void btnSerialActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSerialActionPerformed
        // TODO add your handling code here:
        try{
            //open an configure serial port
            String portName = cmbSerial.getSelectedItem().toString();
            this.port = new SerialPort(portName);
            port.openPort();
            port.setParams(port.BAUDRATE_9600, port.DATABITS_8, port.STOPBITS_1, port.PARITY_NONE, false, false);

            System.out.println("--------successfully initialise serial: " + portName + "----------------\n");

        }
        catch(SerialPortException e){
            System.out.println("initialise serial \n" + e.toString());
        }
    }//GEN-LAST:event_btnSerialActionPerformed

    private void txfGyroXActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_txfGyroXActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_txfGyroXActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        //* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) { //Nimbus
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(MainGUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(MainGUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(MainGUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(MainGUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        //* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new MainGUI().setVisible(true);
                System.out.println("gui test");
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.ButtonGroup btnGrpManual;
    private javax.swing.JButton btnKill;
    private javax.swing.JButton btnRevive;
    private javax.swing.JButton btnSend;
    private javax.swing.JButton btnSerial;
    private javax.swing.JComboBox<String> cmbSerial;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel10;
    private javax.swing.JLabel jLabel11;
    private javax.swing.JLabel jLabel12;
    private javax.swing.JLabel jLabel13;
    private javax.swing.JLabel jLabel14;
    private javax.swing.JLabel jLabel15;
    private javax.swing.JLabel jLabel16;
    private javax.swing.JLabel jLabel17;
    private javax.swing.JLabel jLabel18;
    private javax.swing.JLabel jLabel19;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel20;
    private javax.swing.JLabel jLabel21;
    private javax.swing.JLabel jLabel22;
    private javax.swing.JLabel jLabel23;
    private javax.swing.JLabel jLabel25;
    private javax.swing.JLabel jLabel27;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JLabel jLabel7;
    private javax.swing.JLabel jLabel8;
    private javax.swing.JLabel jLabel9;
    private javax.swing.JMenu jMenu1;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JPanel jPanel6;
    private javax.swing.JPanel jPanel8;
    private javax.swing.JPanel jPanel9;
    private javax.swing.JSeparator jSeparator1;
    private javax.swing.JSeparator jSeparator2;
    private javax.swing.JSeparator jSeparator3;
    private javax.swing.JSeparator jSeparator4;
    private javax.swing.JTabbedPane jTabbedPane1;
    private javax.swing.JLabel lblSliderManual;
    private javax.swing.JRadioButton rbtnSteering;
    private javax.swing.JRadioButton rbtnTail;
    private javax.swing.JRadioButton rbtnThrottle;
    private javax.swing.JSlider sldManual;
    private javax.swing.JTextField txfAccX;
    private javax.swing.JTextField txfAccY;
    private javax.swing.JTextField txfAccZ;
    private javax.swing.JTextField txfGSpeed;
    private javax.swing.JTextField txfGyroX;
    private javax.swing.JTextField txfGyroY;
    private javax.swing.JTextField txfGyroZ;
    private javax.swing.JTextField txfHeading;
    private javax.swing.JTextField txfHeight;
    private javax.swing.JTextField txfHmsl;
    private javax.swing.JTextField txfLat;
    private javax.swing.JTextField txfLong;
    private javax.swing.JTextField txfMagH;
    private javax.swing.JTextField txfMagX;
    private javax.swing.JTextField txfMagY;
    private javax.swing.JTextField txfMagZ;
    private javax.swing.JTextField txfPress;
    private javax.swing.JTextField txfTemp;
    private javax.swing.JTextField txfVelD;
    private javax.swing.JTextField txfVelE;
    private javax.swing.JTextField txfVelN;
    // End of variables declaration//GEN-END:variables
}
