import processing.serial.*;
import cc.arduino.Arduino;


Arduino arduino;
PFont font;
int xPos=0;
boolean firstLoop = true;
String arduinoPort = "/dev/tty.usbserial-A6008cpG";

void setup(){
  size(1000,300);
  smooth();
  String ports[];
  ports = Arduino.list();
  for(int i=0;i<ports.length;i++) {
      if (ports[i].equals(arduinoPort)) {
	  arduino = new Arduino(this, ports[i]);
      }
  }
  if (arduino == null) {
      throw new RuntimeException("could not find Arduino port");
  }

  font = loadFont("MinimaSSK-96.vlw");
  textFont(font, 48);

  background(0);
}

void draw(){
    if (firstLoop) {
	delay(1500);
	firstLoop = false;
    }

    int value = arduino.analogRead(0);
    int yPos = (int)map(value, 0, 1023, height, 20);
    xPos++;

    if (xPos > width) {
	fill(0);
	noStroke();
	rect(min(xPos+30, width-100), max(yPos-70, 0), 100, 45);
	xPos = 0;
    }

    fill(30, 5);
    noStroke();
    rect(0, 0, width, height);

    fill(0);
    rect(xPos, 1, 160, height-2);
    rect(min(xPos+30, width-100), max(yPos-70, 0), 100, 45);

    plot(xPos, yPos, value);

    fill(255);
    text(""+value, min(xPos+30, width-100), max(yPos-30, 40));

    noFill();
    stroke(128);
    rect(0, 0, width-1, height-1);

}

void plot(int x, int y, int value) {
    
    fill(255, map(value, 0, 1023, 255, 0), 0);
    ellipse(x, y, 30, 30);
}

