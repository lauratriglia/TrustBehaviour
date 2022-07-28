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
 * @file trustBehaviourModule.h
 * @brief module that provides an interface to the keepon
 */

#ifndef _trustBehaviour_MODULE_H_
#define _trustBehaviour_MODULE_H_
#include <iostream>
#include <string>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
#include <yarp/os/Log.h>

//within project includes  
#include <trustBehaviourThread.h>

#define REPLY_OK		createVocab('o', 'k', 0, 0)
#define REPLY_FAIL		createVocab('f', 'a', 'i', 'l')
#define CMD_DO		    createVocab('d', 'o',0,0)       

class trustBehaviourModule:public yarp::os::RFModule {
    
    std::string moduleName;                  // name of the module
    std::string robotName;                   // name of the robot 
    std::string robotPortName;               // name of robot port
    std::string inputPortName;               // name of the input port for events
    std::string outputPortName;              // name of output port
    std::string handlerPortName;             // name of handler port
    std::string configFile;                  // name of the configFile that the resource Finder will seek
	yarp::os::Semaphore mutex;               // semaphore handling the response function
    yarp::os::Port handlerPort;              // a port to handle messages 
    /*  */
	trustBehaviourThread *thread;            // pointer to a new thread to be created and started in configure() and stopped in close()

public:

	trustBehaviourModule(); // constructor default
    ~trustBehaviourModule();

    /**
    *  configure all the tutorial parameters and return true if successful
    * @param rf reference to the resource finder
    * @return flag for the success
    */
    bool configure(yarp::os::ResourceFinder &rf); 
   
    bool interruptModule();     //  interrupt, e.g., the ports                

    bool close(); // close and shut down the tutorial

    double getPeriod();  // unimplemented

    bool updateModule();  // unimplemented

	/**
    *  to respond through rpc port
    * @param command reference to bottle given to rpc port of module, alongwith parameters
    * @param reply reference to bottle returned by the rpc port in response to command
    * @return bool flag for the success of response else termination of module
    */
    bool respond(const yarp::os::Bottle& command, yarp::os::Bottle& reply);

};


#endif //_I_SERVO_EMOTION_MODULE_H_

//----- end-of-file --- ( next line intentionally left blank ) ------------------

