/*
 *  Copyright (C) 2004  The Concord Consortium, Inc.,
 *  10 Concord Crossing, Concord, MA 01742
 *
 *  Web Site: http://www.concord.org
 *  Email: info@concord.org
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * END LICENSE */

/*
 * Created on Jan 12, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package org.concord.sensor.state;

import org.concord.framework.otrunk.OTBundle;
import org.concord.framework.otrunk.OTChangeListener;
import org.concord.framework.otrunk.OTChangeNotifying;
import org.concord.framework.otrunk.OTID;
import org.concord.framework.otrunk.OTObject;
import org.concord.framework.otrunk.OTObjectList;
import org.concord.framework.otrunk.OTObjectService;
import org.concord.framework.otrunk.OTResourceSchema;
import org.concord.framework.otrunk.OTServiceContext;
import org.concord.framework.text.UserMessageHandler;
import org.concord.sensor.DeviceConfig;
import org.concord.sensor.SensorDataManager;
import org.concord.sensor.device.impl.InterfaceManager;

/**
 * @author Scott Cytacki
 *
 */
public class OTInterfaceManager extends InterfaceManager 
	implements OTObject, OTChangeNotifying, OTBundle
{
	public static interface ResourceSchema extends OTResourceSchema 
	{
		OTObjectList getDeviceConfigs(); 
	}
	private ResourceSchema resources;
	/**
	 * @param h
	 */
	public OTInterfaceManager(ResourceSchema resources, 
			UserMessageHandler messageHandler) {
		super(messageHandler);
		
		this.resources = resources;
	}

	/* (non-Javadoc)
	 * @see org.concord.framework.otrunk.OTObject#getGlobalId()
	 */
	public OTID getGlobalId() {
		return resources.getGlobalId();
	}

	/* (non-Javadoc)
	 * @see org.concord.framework.otrunk.OTObject#getName()
	 */
	public String getName() {
		return resources.getName();
	}

	/* (non-Javadoc)
	 * @see org.concord.framework.otrunk.OTObject#setName(java.lang.String)
	 */
	public void setName(String name) {
		resources.setName(name);
	}

	public DeviceConfig [] getDeviceConfigs()
	{
		OTObjectList deviceConfigList = resources.getDeviceConfigs();
		DeviceConfig [] configs = new DeviceConfig [deviceConfigList.size()];
		for(int i=0; i<configs.length; i++) {
			configs [i] = (DeviceConfig)deviceConfigList.get(i);
		}
		return configs;
	}
	
	public void init()
	{
		
	}
	
	public OTObjectList getOTDeviceConfigs()
	{
		return resources.getDeviceConfigs();
	}
	
	public OTObjectService getOTObjectService() 
	{
		return resources.getOTObjectService();
	}
	
    public void addOTChangeListener(OTChangeListener listener)
    {
    	resources.addOTChangeListener(listener);
    }
    
    public void removeOTChangeListener(OTChangeListener listener)
    {
    	resources.removeOTChangeListener(listener);
    }

	/* (non-Javadoc)
     * @see org.concord.framework.otrunk.OTBundle#registerServices(org.concord.framework.otrunk.OTServiceContext)
     */
    public void registerServices(OTServiceContext serviceContext)
    {
    	serviceContext.addService(SensorDataManager.class, this);
    }    
    
	/* (non-Javadoc)
     * @see org.concord.framework.otrunk.OTBundle#initializeBundle(org.concord.framework.otrunk.OTServiceContext)
     */
    public void initializeBundle(OTServiceContext serviceContext)
    {
    	// nothing to do here
    }
}
