/*
 * Last modification information:
 * $Revision: 1.3 $
 * $Date: 2005-02-08 21:29:42 $
 * $Author: scytacki $
 *
 * Licence Information
 * Copyright 2004 The Concord Consortium 
*/
package org.concord.sensor.device;

import org.concord.sensor.ExperimentConfig;
import org.concord.sensor.ExperimentRequest;


/**
 * SensorDevice
 * Class name and description
 *
 * Date created: Dec 24, 2004
 *
 * @author scott<p>
 *
 */
public interface SensorDevice
{
	/**
	 * This method is called just after the device is started.
	 * The returned value is used for the milli seconds between
	 * read() calls. It is an approximation, the caller doesn't guarruntee
	 * to call read() exactly this many ms.  Implementors should return 
	 * smallest reasonable value.  Most likely the data will be displayed
	 * in realtime, so a faster response time is better. 
	 * @return
	 */
	public int getRightMilliseconds();
	
	/**
	 * This is the first method called in the life cycle of this object.
	 * The passed in string can contain whatever configuration information
	 * this device needs.  For a serial port device it probably needs to 
	 * contain the port name.  For a usb device this might not be necessary.
	 * @param openString
	 */
	public void open(String openString);
	
	/**
	 * This is the last method called in the life cycle of this object.  
	 * The object should release any resources it is holding.  It should
	 * close serial ports, usb ports, or expansion card handles.   
	 * The object will not be used again after this is called,  A new 
	 * object will be created instead.
	 */
	public void close();
	
	/**
	 * This method is called after open but before start.  The request is 
	 * a general sensor configuration request.  The device should try to
	 * meet this request.  If the device can id sensors then it should just
	 * assume the appropriate sensors are attached based on the request.  
	 * 
	 * If the device can id sensors then it should verify the appropriate
	 * sensors are attached.  If not then it should return a ExperimentConfig
	 * with an isValid that returns false.  The sensors should be set to the 
	 * actual sensors even if they are wrong.  If the device returns an invalid
	 * config with sensors that don't match then a message will be shown to the
	 * user asking them to plug in the appropriate sensors.  The getInvalidReaon
	 * might be used in this case.  If getSensorConfig() returns null then 
	 * getInvalidReason will be displayed to the user.
	 * 
	 * This method might be called more than once, especially if it reports
	 * the wrong sensors are attached.
	 * @param request
	 * @return
	 */
	public ExperimentConfig configure(ExperimentRequest request);
	
	/**
	 * This method is called after configure returns a valid ExperimentConfig
	 * If the external device has its own timer and needs to be started this
	 * is the method where that should be done.
	 * 
	 * After stop has been called this method might be called again.
	 * @return
	 */
	public boolean start();

	/**
	 * After the device is started this method is called again and again
	 * it should be called within the value returned by getRightMilliseconds()
	 * If it takes longer than getRightMilliseconds() to execute then it will
	 * be called again as soon as possible.
	 * 
	 * It should place values read from the device in the values buffer, starting
	 * at the offset.  The data should be in "samples" each sample might consist
	 * of multiple values.  For example if a temperature and pressure sensor is
	 * attached then each sample should have 2 values (t,p).
	 * The number and order of the values in each sample should match the
	 * SensorConfigs in the ExperimentConfig returned by configure. 
	 * 
	 * If the sensor device returned false for getExactPeriod then
	 * an addition value should be returned before the others for each
	 * sample.  This value is the time the sample was taken.  It should
	 * be in seconds since the call to start().
	 * 
	 * After writing a sample to the values buffer the method should advance 
	 * by nextSampleOffset.   This is sometimes referred to as a "stride".
	 * So if the nextSampleOffset == 5 and there are 2 values in each sample
	 * then the method should skip 3 values after each sample. 
	 * 
	 * It should return the number of samples read.  0 means no samples were
	 * read.  -1 means there was an error. getErrorMessage() will be called 
	 * to find out what the error was.
	 * 
	 * If the numberOfSamples property in the Experiment request has already
	 * been reached then this should return -2.    
	 * 
	 * @param values
	 * @param offset
	 * @param nextSampleOffset
	 * @param reader
	 * @return
	 */
	public int read(float [] values, int offset, int nextSampleOffset,
			DeviceReader reader);

	/**
	 * This method is called after start.  It might be called more than once.
	 * the wasRunning param should be mostly true.  The timer will be stoped
	 * before this method is called, the read method will not be called after
	 * or while this method is executing.
	 *   
	 * If for some reason the timer was stopped long before this method is called
	 * then wasRunning will be false.
	 * @param wasRunning
	 */
	public void stop(boolean wasRunning);

	/**
	 * Called after an error in start or read.
	 * @param error
	 * @return
	 */
	public String getErrorMessage(int error);
	
	/**
	 * Is the device attached?
	 * @return
	 */
	public boolean isAttached();
	
	/**
	 * Can the device determine if sensors are attached.  Even if the device
	 * can only tell some sensor is attached, but it doesn't know which sensor
	 * it should still return true here.
	 * @return
	 */
	public boolean canDetectSensors();
	
	/**
	 * This will be called to figure out the setup of the current device
	 * If the device cannot detect sensors then it should just fill
	 * out the ExperimentConfig with the device name, 0 for the number
	 * of sensors, and valid should be false.
	 * @return
	 */
	public ExperimentConfig getCurrentConfig();
}