
#include "main.h"
#include "pros/adi.hpp"
#include "pros/rtos.hpp"
#include "pros/motors.hpp"
#include "pros/vision.hpp"

using namespace pros;

#define LEFT_WHEELS_PORT1 11
#define LEFT_WHEELS_PORT2 -12
#define LEFT_WHEELS_PORT3 -20
#define RIGHT_WHEELS_PORT1 -1
#define RIGHT_WHEELS_PORT2 2
#define RIGHT_WHEELS_PORT3 10
#define TOP_GEAR_PORT 18
#define INTAKE_PORT -17
#define SCORING_PORT 16

MotorGroup left_drivetrain ({LEFT_WHEELS_PORT1,LEFT_WHEELS_PORT2,LEFT_WHEELS_PORT3},v5::MotorGearset::blue);
MotorGroup right_drivetrain ({RIGHT_WHEELS_PORT1,RIGHT_WHEELS_PORT2,RIGHT_WHEELS_PORT3},v5::MotorGearset::blue);
MotorGroup intake ({INTAKE_PORT},v5::MotorGearset::green);
MotorGroup topGear ({TOP_GEAR_PORT},v5::MotorGearset::green);
MotorGroup scorer ({SCORING_PORT},v5::MotorGearset::green);

// Controller definitions
Controller master (CONTROLLER_MASTER);

/**
* This function is called when the center button on the V5 LCD is pressed.
* It can be used to perform actions such as starting a program or displayin
* a message.
*/
void on_center_button() {
}


/**
* Runs initialization code. This occurs as soon as the program is started.
*
* All other competition modes are blocked by initialize; it is recommended
* to keep execution time for this mode under a few seconds.
*/
void initialize() {
   pros::lcd::initialize();
   pros::lcd::set_text(1, "Hello PROS User!");
   pros::lcd::register_btn1_cb(on_center_button);
}


/**
* Runs while the robot is in the disabled state of Field Management System or
* the VEX Competition Switch, following either autonomous or opcontrol. When
* the robot is enabled, this task will exit.
*/
void disabled() {}


/**
* Runs after initialize(), and before autonomous when connected to the Field
* Management System or the VEX Competition Switch. This is intended for
* competition-specific initialization routines, such as an autonomous selector
* on the LCD.
*
* This task will exit when the robot is enabled and autonomous or opcontrol
* starts.
*/
void competition_initialize() {}


/**
* Runs the user autonomous code. This function will be started in its own task
* with the default priority and stack size whenever the robot is enabled via
* the Field Management System or the VEX Competition Switch in the autonomous
* mode. Alternatively, this function may be called in initialize or opcontrol
* for non-competition testing purposes.
*
* If the robot is disabled or communications is lost, the autonomous task
* will be stopped. Re-enabling the robot will restart the task, not re-start it
* from where it left off.
*/
void autonomous() {}



/**
* Runs the operator control code. This function will be started in its own task
* with the default priority and stack size whenever the robot is enabled via
* the Field Management System or the VEX Competition Switch in the operator
* control mode.
*
* If no competition control is connected, this function will run immediately
* following initialize().
*
* If the robot is disabled or communications is lost, the
* operator control task will be stopped. Re-enabling the robot will restart the
* task, not resume it from where it left off.
*/
void opcontrol() {
	bool top_goal_on =  false;
	bool middle_goal_on =  false;
	bool intake_on =  false;
	bool top_gear_on = false;

   while (true) {

		// Finish the code so that the left stick controls the drive! (Tank drive)
		int power = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = master.get_analog( E_CONTROLLER_ANALOG_RIGHT_X);

		int left = power + turn;
		int right = power - turn;

		left_drivetrain.move(left);
		right_drivetrain.move(right);

		//==========================================================================

		// Move intake based on Left bumpers
		if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)) {
			intake_on = !intake_on;
			top_gear_on = !top_gear_on;
		}
		// high goal scoring
		if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_R2)) {
			top_goal_on = !top_goal_on;
			middle_goal_on = false;
		}
		// middle goal scoring
		if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_R1)) {
			middle_goal_on = !middle_goal_on;
			top_goal_on = false;
		}

		// move intake based off states
		if (master.get_digital(E_CONTROLLER_DIGITAL_L2)){
			intake.move(127);
			topGear.move(127);
		}
		else if (intake_on == true){
			intake.move(-127);
			topGear.move(-127);
		}
		else{
			intake.move(0);
			topGear.move(0);
		}
		
		if (top_goal_on == true){
			scorer.move(127);
		}
		else if (middle_goal_on == true){
			scorer.move(-127);
		}
		else {
			scorer.move(0);
		}

		/* keybinds
		L1 = toggle intake on(1 click on, 2 clicks off)
		L2 = hold to reverse intake(will go back to normal direction after button is releasedx)
		R1 = toggle medium goal(1 click on, 2 clicks off)
		R2 = toggle high goal(1 click on, 2 clicks off)
		*/


		pros::delay(20);
   }

}