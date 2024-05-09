
//refrance page : https://ecs.wgtn.ac.nz/Courses/ENGR101_2024T1/AVC_manuals
//This doccument uses camelCase 


//------- Imports -------
#include <iostream> // input-output library 
#include <math.h> //used for rounding function
#include "E101.h" // VUW camera library
#include <chrono>

using namespace::std;


//------- Global Varables ---------
//used for camera reading
const int totalXPixels = 320;
const int totalYPixesl = 240;

//Motor Varables
const int leftMotorPort = 5;  //port number for the left motor
const bool leftMotorReversed = true; // is the left motor fliped

const int rightMotorPort = 3; //port number for the right motor
const bool rightMotorReversed = false; // is the right motor fliped

const int cameraMotorPort = 1;//port number for the camera motor
const bool cameraMotorReversed = false; // is the camera motor fliped

//Black detection Varables
const float blackTolarance = 10; // The value by which red green and blue can differ and still be black
const int maxIntensity  = 60; // The maximum value of red + green + blue values to still be black

const int leftOfBox = 20; //px
const int rightOfBox = 300; //px
const int topOfBox = 20; //px
const int bottomOfBox = 200; //px

const int maxOffSet = rightOfBox-leftOfBox; 



char serverAddress[15] = {'1','3','0','.','1','9','5','.','3','.','5','3'}; // server address
int serverPort = 1024; //server port

//used to store instructions to drive each motor


//------------------------- Static Functions ---------------------------

//takes an the x and y value of a pixel and checks if its black
//if it is then it makes it green
bool isblack(int x,int y){
    int red = get_pixel(y,x,0);
    int green = get_pixel(y,x,1);
    int blue = get_pixel(y,x,2);
    int intensity = get_pixel(y,x,3);

    //checsk if the red green and blue values are within + or - blackTolarance of each other 
    //and intesity is less than the maximum intesity
    if((red - blackTolarance < green || red + blackTolarance > green) 
       && (red - blackTolarance < blue || red + blackTolarance > blue) 
       && (green - blackTolarance < blue || green + blackTolarance > blue)
        && intensity > maxIntensity ){
       return false;
   
    }else{
        //sets pixels detected as black to green
        set_pixel(y, x, 0, 255, 0);
        return true;
    }
}

//takes the row number
//returns a rowInfo struct
int readRow(int rowToRead){

    bool firstBlackPixel = true;

    int lastBlackPixelX;
    int firstBlackPixelX;

    for(int currentXPixel =leftOfBox; currentXPixel< rightOfBox; currentXPixel+=1){
		if(isblack(currentXPixel,rowToRead)){
            //sets the first black pixel if this is the first black pixel detected
			if(firstBlackPixel){
				output.firstBlackPixelX = currentXPixel;
				firstBlackPixel = false;
			}
			lastBlackPixelX = currentXPixel;        
        }
    }

    return (lastBlackPixelX - firstBlackPixelX)/2 + firstBlackPixelX;
}

//This takes a left, rght, top and bottom side and draws a box
void drawBox(int left,int right,int top,int bottom) {
	//This draws the green left and right lines
	for (int currentRow = 0; currentRow < (bottom - top) ; currentRow += 1) {
		set_pixel(top + currentRow, left, 0, 255, 0);
		set_pixel(top + currentRow, right, 0, 255, 0);
	}
	//This draws the blue top and bottom lines
	for (int currentCol = 0; currentCol < (right - left) ; currentCol += 1) {
		set_pixel(top, left + currentCol, 0, 0, 255);
		set_pixel(bottom, left + currentCol, 0, 0, 255);

	}
}

//maximum vlaue is the maximum value for the input range
//minimum value is the minimum value for the input range
//current vlaue is the value inisde the range to be written to the motors
//revers is weather to reverse the motor useful for left and right
//port is the port to be written to 
void motorDrive(double currentOffSet, bool reverse, int port) {
	
	
	//maps from 0 to 60 - 46 = 14
	int initalPWM = 55;
	//17
	int PWMRange = 65-initalPWM;
	
	cout << "calculated value" <<  (currentOffSet/((double)(2*maxOffSet)/(double)(PWMRange))) << endl;
	
	int pwm = (currentOffSet/((double)(2*maxOffSet)/(double)(PWMRange)));
	
	cout << "pwm:" << initalPWM+pwm << endl;
	
    //checks to make sure pwm is greater than 30 and less than 60 to not fry motors
    if (pwm < 30 && pwm > 60) {
        cout << "motorDrive:pwm out of range (30 to 60)" << endl;
        cout << "motorDrive:Current value of pwm:" << pwm << endl;
        return;
    }else {
        //fips pwm for reverse
        if (reverse) {
            pwm = initalPWM +pwm;
            pwm = 68 - (pwm-31);
            set_motors(port,pwm);
            
        }
        else {
			pwm = initalPWM +pwm;
            set_motors(port,pwm);
            
        }
    } 
}







//------- Dynamic Functions ---------
void openGate(){
    //attempts to connect to gate, will loop untill connected
    int connected = 0;
    //while(connected == 0){
	connected = connect_to_server(serverAddress,serverPort);
    cout << "error connectiong to gate, code:" << connected << endl;
    //}

    //sends request to server
    char pleaseMessage[] = {'P','l','e','a','s','e'};
    send_to_server(pleaseMessage);

    //reccives password from server
    char password[24];
    receive_from_server(password);
    cout << "password:" << password << endl;

    //sends password to server
    send_to_server(password);
}

void followLine(){
    take_picture(); 

    uint64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    int kp = 1;
    int ki = 1;
    int kd = 1;

    //for 320 pixesl this is 160
    int centerOfScreen = totalXPixels/2;

    //The distance from the center of the screen  to the average black pixel
    int error = readRow(bottomOfBox) - centerOfScreen;

    output = kp * error + kp *___ + kd *
    
	
    //draws the box of where it is checking
    drawBox(leftOfBox,rightOfBox,topOfBox-1,bottomOfBox+1);

    //sets the top average pixel to red
    set_pixel(topOfBox, topRow.averageX, 255, 0, 0);
    //sets the bottom average pixel to red
    set_pixel(bottomOfBox, bottomRow.averageX, 255, 0, 0);
    
    
    
    //sets the left motor speed 
    motorDrive(deltaX,leftMotorReversed,leftMotorPort);
    //sets the right motor speed
    motorDrive(deltaX,rightMotorReversed,rightMotorPort);


    
    //updates the screen
    update_screen();

    //drives the motors
    hardware_exchange();
}
void intersections(){
	
	
    
}
void pushPole(){
    
}


//------- Main ---------
int main() {
	
	int err = init(0);
	cout << "error:" << err << endl;
	open_screen_stream();
	
	//set_motors(leftMotorPort,56);
    //set_motors(rightMotorPort,40);
    //hardware_exchange();
	
    openGate();
    cout << "open gate passed" << endl;

	while(true){
        followLine();
        //nt pwm = 55;
        
        //set_motors(leftMotorPort,pwm);
        
        //pwm = 65-(pwm-31);
        
        //set_motors(rightMotorPort,pwm);
        
        //hardware_exchange();
        
        sleep1(20);
    }
    
    set_motors(leftMotorPort,45);
    set_motors(rightMotorPort,45);

    cout << "follow line passed" << endl;

    
    return 0;
}
