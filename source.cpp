#include <iostream>
#include <string.h>
#include "Leap.h"

using namespace Leap;

const int totalcombination = 3;
int combination_cnt = 1;

class SampleListener : public Listener {
public:
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	//virtual void onDisconnect(const Controller&);
	//virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	//virtual void onFocusGained(const Controller&);
	//virtual void onFocusLost(const Controller&);
	//virtual void onDeviceChange(const Controller&);
	//virtual void onServiceConnect(const Controller&);
	//virtual void onServiceDisconnect(const Controller&);

private:
};

bool authed = 0;
const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };
const std::string stateNames[] = { "STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END" };

void auth_check(int total){
	if (combination_cnt<=totalcombination){
		switch (combination_cnt){
		case 1:
			if (total==8)	break;
			else return;
		case 2:
			if (total == 1) break;
			else return;
		case 3:
			if (total == 6) break;
			else return;
		}

	combination_cnt++;
	return;
	}
	else{
		std::cout << "Unlocked, Press enter." << std::endl;
		authed = true;
		std::cin.get();
	}
}

void SampleListener::onInit(const Controller& controller) {
	//std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
	//std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}


void SampleListener::onFrame(const Controller& controller) {
	const Frame frame = controller.frame();

	HandList hands = frame.hands();

	std::string clockwiseness;
	int totalfinger=0;

	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		const Hand hand = *hl;

		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";

		const FingerList fingers = hand.fingers();
		FingerList efl = hand.fingers().extended();

		totalfinger += hand.fingers().extended().count();

		for (FingerList::const_iterator fl = efl.begin(); fl != efl.end(); ++fl) {
			const Finger finger = *fl;

			//std::cout << handType << "  " << fingerNames[finger.type()] << std::endl;
		}

		//Find clockwise or anticlockwise
		const GestureList gestures = frame.gestures();
		for (int g = 0; g < gestures.count(); ++g) {
			Gesture gesture = gestures[g];

			if (gesture.type() == Gesture::TYPE_CIRCLE){
				CircleGesture circle = gesture;
	
				if (circle.pointable().direction().angleTo(circle.normal()) <= PI / 2) {
					clockwiseness = "clockwise";
				}
				else {
					clockwiseness = "counterclockwise";
				}
				//std::cout << clockwiseness<< "     " << std::endl;
			}
		}
		//if (totalfinger==3) 
	}
	//std::cout << clockwiseness << "  " << totalfinger << std::endl;

	auth_check(totalfinger);
}


int main(int argc, char** argv) {
	// Create a sample listener and controller
	SampleListener listener;
	Controller controller;

	std::cout << std::endl << "Locked Application" << std::endl;
	
	// Have the sample listener receive events from the controller
	controller.addListener(listener);

	if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);

	// Keep this process running until Auth is passed
	while (authed == false){
		std::cin.get();
		if (authed == true){
			std::cout << "Welcome User, Press enter to exit";
			controller.removeListener(listener);
			return 0;
		}
	}
	// Remove the sample listener when done
}

