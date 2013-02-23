#define LEFT 2
#define RIGHT 1
#define ORANGE 3

int mode = 1;
bool pressed = false;

int selectMode(){

	while(pressed == false){

  	if(nNxtButtonPressed == LEFT){
  		mode = 1;
  		pressed = true;

  	}if(nNxtButtonPressed == RIGHT){
  		mode = 2;
  		pressed = true;
  	}
  }
  return mode;
}

task main(){

  selectMode();

  while (true){

  	if(mode == 1){
  		//autonomousOne();

  	}else{
  		//autonomousTwo();
  	}
  }
}
