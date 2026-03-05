**CurveTrack**
--
**A G-force data logger that aids drivers in improving their performance on the track.**
--

Description
--
  Right now the project is in development stage, it only logs timestamps and g-forces on MicroSD card but GPS coordinates will be added later along with a user-friendly Python GUI that analyzes the data logged using the ESP32 to show the driver: the exact layout of his drive using GPS, lap time, corner entry speed, corner exit speed and a G-G diagram to aid in trail-braking effectively.

Architecture
--
* Microcontroller: ESP32

* Inertial Measurement Unit (IMU): MPU6050 (6-axis Accel/Gyro) via I2C

* Storage: DFRobot MicroSD Module V1.0 via SPI

* Input: Interrupt-driven tactile button for calibration of the IMU.

Features
--
* Logs G-forces on 3 axes along with a timestamp on the MicroSD card
* Calibrates the MPU6050 to avoid drift with a press of a button by taking samples from every axis and calculating the offset
* Implemented a filter to ignore button bounces after a button press occurs
* Uses hardware interrupts (IRAM_ATTR) on GPIO 12 for IMU calibration to ensure the responsiveness during calibration and SD write cycles

Roadmap
--
- [x] G-Force logging
- [x] Writing data on MicroSD (timestamps including G-Forces)
- [ ] Implement GPS module (NEO-M8N probably) 
- [ ] Automatic lap tracker with an LCD screen showing lap time
- [ ] Python app to visualize traction circle using G-G diagram and provide the driver with full lap data
- [ ] Learn more about code optimization and optimize my code to get even more accurate data
- [ ] Whatever else seems interesting/useful to add!
