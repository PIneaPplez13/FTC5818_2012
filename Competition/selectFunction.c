#define LEFT 2
#define RIGHT 1
#define ORANGE 3

mode = 1;
pressed = False;

int selectMode(){

  if(nNxtButtonPressed == LEFT){
  	mode = 1;

  }if(nNxtButtonPressed == RIGHT){
  	mode = 2;

  }if(nNxtButtonPressed == ORANGE){
  	pressed = True;
  	return mode;
  }
}

task main(){

  selectMode();
	waitForStart();

  while (true){

  	if(mode == 1){
  		autonomousOne();

  	}else{
  		autonomousTwo();
  	}
  }
}
