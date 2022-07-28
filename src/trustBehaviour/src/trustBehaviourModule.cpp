// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
  * Copyright (C)2019  Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
  * 
  * Author: Dario Pasquali
  * email: dario.pasquali@iit.it
  * 
  * Permission is granted to copy, distribute, and/or modify this program
  * under the terms of the GNU General Public License, version 2 or any
  * later version published by the Free Software Foundation.
  *
  * A copy of the license can be found at
  * http://www.robotcub.org/icub/license/gpl.txt
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
  * Public License for more details
*/

/**
 * @file trustBehaviourModule.cpp
 * @brief Implementation of the trustBehaviourModule to control Blinking with Serial.
 */

#include "trustBehaviourModule.h"

using namespace yarp::os;
using namespace yarp::sig;
using namespace std;

/* 
 * Configure method. Receive a previously initialized
 * resource finder object. Use it to configure your module.
 * If you are migrating from the old Module, this is the 
 *  equivalent of the "open" method.
 */

trustBehaviourModule::trustBehaviourModule() {
    //do nothing       
}

trustBehaviourModule::~trustBehaviourModule() {
    // do nothing
}

bool trustBehaviourModule::configure(yarp::os::ResourceFinder &rf) {
    /* Process all parameters from both command-line and .ini file */

    /* get the module name which will form the stem of all module port names */

    moduleName = rf.check("name", Value("/RPC_PORT_NAME"), "module name (string)").asString();
	
    /*
    * before continuing, set the module name before getting any other parameters, 
    * specifically the port names which are dependent on the module name
    */
    setName(moduleName.c_str());
   
    /*
    * attach a port of the same name as the module (prefixed with a /) to the module
    * so that messages received from the port are redirected to the respond method
    */
    handlerPortName =  "";
    handlerPortName += getName();         // use getName() rather than a literal

    if (!handlerPort.open(handlerPortName.c_str())) {           
        cout << getName() << ": Unable to open port " << handlerPortName << endl;  
        return false;
    }

    attach(handlerPort);                  // attach to port
    if (rf.check("config")) {
        configFile=rf.findFile(rf.find("config").asString().c_str());
        if (configFile=="") {
            return false;
        }
    }
    else {
        configFile.clear();
    }
	
    /* create the thread and pass pointers to the module parameters */
    thread = new trustBehaviourThread(rf);
    thread->setName(getName().c_str());
    
    /* now start the thread to do the work */
    thread->start(); // this calls threadInit() and it if returns true, it then calls run()

    return true ;       // let the RFModule know everything went well
                        // so that it will then run the module
}

bool trustBehaviourModule::interruptModule() {
    handlerPort.interrupt();
    return true;
}

bool trustBehaviourModule::close() {
    /* stop the thread */
    yDebug("stopping the thread ");
    thread->stop();
	handlerPort.interrupt();
	handlerPort.close();
    return true;
}

bool trustBehaviourModule::respond(const Bottle& command, Bottle& reply) {
        
    reply.clear(); 
    if (command.get(0).asString()=="quit") {
        reply.addString("quitting");
        return false;     
    }
    else if (command.get(0).asString()=="help") {

		reply.addVocab(Vocab::encode("many"));
		reply.addString("One addString for each line");
		return true;
    }
	
	mutex.wait();
	switch (command.get(0).asVocab()) {
	case CMD_DO: 
        yDebug("RPC command do");
        break;
	}
	reply.addVocab(REPLY_OK);
	mutex.post();
}

/* Called periodically every getPeriod() seconds */
bool trustBehaviourModule::updateModule()
{
    return true;
}

double trustBehaviourModule::getPeriod()
{
    /* module periodicity (seconds), called implicitly by myModule */
    return 1;
}

