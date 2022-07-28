// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C)2019  Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
 *
 * Revision: Laura Triglia	
 * email: laura.triglia@iit.it
 *
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

/**
 * @file trustBehaviourThread.cpp
 * @brief Implementation of trustBehaviourThread.h.
 */

#include <trustBehaviourThread.h>


using namespace yarp::dev;
using namespace yarp::os;
using namespace yarp::sig;
using namespace std;


trustBehaviourThread::trustBehaviourThread() :Thread() {
}

trustBehaviourThread::trustBehaviourThread(yarp::os::ResourceFinder &_rf):Thread(){
	rf = _rf;
}

trustBehaviourThread::~trustBehaviourThread() {
    // do nothing
}

bool trustBehaviourThread::threadInit() 
{
    enum states {
		INTRO = 1,
		FRIENDLY = 2,
		NEUTRAL = 3,
		UNFRIENDLY = 4
	};

	//    ptime time_start(microsec_clock::local_time());
    yInfo("trustBehaviourThread::threadInit: initialization in the method");

	yInfo("Open Output Port Port");
    if (!outPort.open(getName("/outport:out").c_str())) {
        yError(": unable to open port ");
        return false;  // unable to open; let RFModule know so that it won't run
    }

	yInfo("Open input Port");
	if (!inPort.open(getName("/inport:i").c_str())) {
		yError(": unable to open port to send unmasked events ");
		return false;  // unable to open; let RFModule know so that it won't run
	}
	
	clientRPCInteractionInterface.setRpcMode(true):
	 if(!clientRPCInteractionInterface.open(getName("/cmd:rpc").c_str())){
        yError("unable to open port to send interaction command ");
        return false;
    }

	// Load configurations

	yInfo("Thread correctly initialised \n");
    return true;
}


void trustBehaviourThread::run() {
	Bottle outputMessage;

	while (!isStopping() != true) {
	   
		switch (current_state) {
		case INTRO: 
			//face_recognition module needs to be in function
			// trust evaluation
			if(trust > threshold){
				current_case = FRIENDLY;
			}
			else if(trust = threshold){
				current_case = NEUTRAL;
			}
			else if (trust < threshold){
				current_case = UNFRIENDLY
			}
			break;
		}
		case FRIENDLY:
			action = "intro";
			cmd.clear();
			cmd.addString("exe");
			cmd.addString(action);

			sendRpcCommand(clientRPCInteractionInterface, cmd);
			// emotion setting
			setFaceModular("raise","smile")
			break;
		case NEUTRAL:
			action = "intro";
			cmd.clear();
			cmd.addString("exe");
			cmd.addString(action);

			sendRpcCommand(clientRPCInteractionInterface, cmd);
			// emotion setting
			setFaceModular("neutral","neutral")
			break;
		case UNFRIENDLY:
			action = "intro";
			cmd.clear();
			cmd.addString("exe");
			cmd.addString(action);

			sendRpcCommand(clientRPCInteractionInterface, cmd);
			// emotion setting
			setFaceModular("sad", "frowned")
			break;
		

		Time::delay(0.05);
	}
}

void trustBehaviourThread::onStop(){
	outPort.interrupt();
	outPort.close();

	inPort.interrupt();
	inPort.close();

	yDebug("Executing code in onStop");    
}

void trustBehaviourThread::threadRelease() {
    yDebug("Executing code in threadRelease");
}

void trustBehaviourThread::setName(string str) {
	this->name = str;
}

std::string trustBehaviourThread::getName(const char* p) {
	string str(name);
	str.append(p);
	return str;
}

void iCubCortisolDrivenThread::setFaceModular(std::string affectEyebr, std::string affectMouth)
{
    //currentEyebr = affectEyebr;
    //currentMouth = affectMouth;
    
    std::vector<std::string> vals;

    vals = getValuesLEDSModular(affectEyebr, affectMouth);

    std::string lebVal = vals[0];
    std::string rebVal = vals[1];
    std::string mouVal = vals[2];
    
    yarp::os::Bottle out;

    if (outputEmotionsPort.getOutputCount()) 
    {
        out.fromString(lebVal);
        outputEmotionsPort.write(out);
        yarp::os::Time::delay(0.05);
        out.clear();

        out.fromString(rebVal);
        outputEmotionsPort.write(out);
        yarp::os::Time::delay(0.05);
        out.clear();

        out.fromString(mouVal);
        outputEmotionsPort.write(out);
    }
}

