import processing.serial.*;
import java.util.*;

String[] port_list;
String[] font_list;

int margin = 10;
int font_n = 0;
int port_n = 0;
Serial myPort;
float portX, portY, portW = 220 + 2* margin, portTextSize = 16, portLabelW;
boolean mouseIsInPortBox = false;
boolean connected = false;
boolean screenIsLocked = false;
PFont portFont;

float distance = -1;
String inchesStr;

class MdWidget {
  int _x, _y, _w, _h;
  
  public MdWidget() {
  }
    
  /**
   * Set position within the parent window
   */
  public void MoveTo(int x, int y) {
    _x = x; _y = y;
  }
  
  public int windowX() {
    return _parent == null ? _x : _parent.windowX() + _x;
  }
  
  public int windowY() {
    return _parent == null ? _y : _parent.windowY() + _y;
  }
  
  public void draw() {
  }
  
  MdWidget _parent;
};

class MdWindow extends MdWidget {
  Vector<MdWidget> _children = new Vector<MdWidget>();
  
  @Override
  public void draw() {
    for(MdWidget child : _children) {
      child.draw();
    }
  }
}

class MdText extends MdWidget {
  String _text;
  
  void SetText(String s) {
    _text = s;
  }
  
  @Override
  public void draw() {
    if (_text == null || _text.isEmpty()) {
      return;
    }
    
    text(_text, screenX(), screenY());
  }
}

boolean listsMatch(String[] plist1, String[] plist2) {
  if (plist1 == null) {
    //println("listsMatch: plist1 is null");
    return false;
  }
  if (plist2 == null) {
    //println("listsMatch: plist2 is null");
    return false;
  }
  if(plist1.length != plist2.length) {
    //println("listsMatch: lengths: " + plist1.length + " != " + plist2.length);
    return false;
  }
  for (int i = 0; i < plist1.length; i++) {
    if (!plist1[i].equals(plist2[i])) {
      return false;
    }
  }
  return true;
}
void ClosePort() {
  if (myPort != null) {
    myPort.stop();
    myPort = null;
  }
  connected = false;
}

void OpenPort() {
  ClosePort();
  myPort = new Serial(this, port_list[port_n], 9600);
  if (myPort != null) {
    connected = true;
  }
}

void CheckPorts() {
  String[] plist = Serial.list();
  String[] newPortList = new String[0];
  for(String port : plist) {
    int indx = port.indexOf("tty.usb"); // Mac OS
    int indx_win = port.indexOf("COM"); // Windows
    if (indx == -1 && indx_win == -1) {
      //println("skipping " + port);
      continue;
    }
    //println("appending " + port);
    newPortList = append(newPortList, port);
    //println("newPortList length: " + newPortList.length);
  }
  // Now compare the lists
  if (!listsMatch(newPortList, port_list)) {
    // Cases:
    // - old list empty
    //   - new list empty
    //   - new list not empty
    // - old list not empty
    //   - new list empty
    //   - new list not empty
    //println("LIST CHANGED!");
    if (port_list == null || port_list.length <= 0) {
      // if both are null/empty, nothing has changed
      if (newPortList != null && newPortList.length > 0) {
        // old empty, new not empty : just use new
        port_list = newPortList;
        OpenPort();
      }
    } else {
      if (newPortList != null && newPortList.length > 0) {
        // both lists have entries
        // Look for old port in new list
        boolean found = false;
        String portName = port_list[port_n];
        int n = Math.min(newPortList.length, port_list.length);
        /*
        println("old len: " + ((port_list == null) ? -1 : port_list.length) + 
          ", new len: " + ((newPortList == null) ? -1 : newPortList.length) +
          ", min: " + n);
          */
        port_n = 0;
        for(int i = 0; i < n; i++) {
          if (newPortList[i] == portName) {
            port_n = i;
            found = true;
            break;
          }
        }
        ClosePort();
        port_list = newPortList;
        OpenPort();
      } else {
        // the new list is empty
        ClosePort();
        port_list = newPortList;
      }
    }
  }
}

boolean setup_portwidget() {
  CheckPorts();
  //println(font_n + ": " + font_list[font_n]);
  portFont = createFont("Arial-Black", portTextSize, true);
//  portFont = createFont("Dialog", portTextSize, true);
  if (portFont == null) {
    println("FAILED TO LOAD FONT");
    return false;
  }
  portX = portY = margin;
  //portW = 0;
  textFont(portFont);
  textSize(portTextSize);
  textAlign(LEFT, TOP);
  if (port_list != null) {
    for(String port : port_list) {
      
      float w = textWidth(port);
      if (w > portW) {
        portW = w + 2 * margin;
        //println("new width: " + portW);
      }
    }
  }
  portLabelW = textWidth("port: ");
  return true;
}
void draw_distance() {
  if (connected && inchesStr != null) {
    float x = portX;
    float y = portY + portTextSize + margin;
    fill(0);
    if (screenIsLocked) {
      text("Click Port To Reset", x, y);
    } else {
      String s = inchesStr;
      int i = s.indexOf('.');
      if (i >= 0) {
        s = s.substring(0, i);
      }
      text(s + "\"", x, y);
    }
  }
}

void draw_portwidget() {
  textFont(portFont);
  textSize(portTextSize);
  textAlign(LEFT, TOP);
  fill(0);
  text("port:", portX, portY);
  //text("" + font_n + ": " + font_list[font_n], portX, portY);
  fill(mouseIsInPortBox ? 0 : 255);
  float x = margin + portLabelW;
  float y = margin;
  rect(x, y, portW, portTextSize + margin);
  fill(mouseIsInPortBox ? 255 : 0);
  if (port_list != null && port_list.length > port_n) {
    text(port_list[port_n], x + margin, y);
  }
}

void setup() {
  background(255);
  size(350, 130);
  font_list = PFont.list();
  if (!setup_portwidget()) {
    exit();
  }
  //println(port_list);
  //println(join(PFont.list(), ", "));
}

void pingSensor() {
    String inBuffer = null;
    if (myPort == null) {
      connected = false;
      return;
    }
    myPort.write(65);
    delay(100);
    if (myPort.available() > 0) {
      inBuffer = myPort.readString();
    }
    if (inBuffer != null) {
      //print(inBuffer);
      int i = inBuffer.indexOf(' ');
      inchesStr = inBuffer.substring(i + 1).trim();
      float inches = 0.0;
      if (inchesStr.matches("^[0-9.]+$")) {
        try {
          inches = Float.valueOf(inchesStr).floatValue();
          //println("" + inches + " inches");
          distance = inches;
          connected = true;
        } catch(NumberFormatException ignore) {
          println("NumberFormatException: " + ignore.getLocalizedMessage());
        }
      }
    }
    else {
      //println("READ FAILED");
      connected = false;
    }
}

void LockScreen() {
  if (screenIsLocked) {
    return;
  }
  println("LOCK THE SCREEN");
//  String[] initStrings = {"/System/Library/CoreServices/Menu Extras/user.menu/Contents/Resources/CGSession", "-suspend"};
  String[] initStrings = {"open", "-a", "/System/Library/Frameworks/ScreenSaver.framework/Versions/A/Resources/ScreenSaverEngine.app"};
  ProcessBuilder pb = new ProcessBuilder(initStrings);
  //pb.inheritIO();
  Process locker;
  try {
    locker = pb.start();
    if (locker.waitFor() == 0) {
      println("Locker is done: Status: " + locker.exitValue());
      screenIsLocked = true;
      locker.destroy();
      locker = null;
    }
  } catch (Exception ignore) {
    println("ignoring exception: " + ignore.getLocalizedMessage());
  }
}

long m = 0;
boolean wasRed = false;
long redStartTime = 0;

void draw() {
  long m1 = millis();

  if (m1 - m > 1000) {
    m = m1;
    CheckPorts();
    pingSensor();
  }
  /* if (screenIsLocked) {
    background(#6795ff);
    wasRed = false;
    redStartTime = 0;
  } else */ if (connected) {
    if (distance < 0) {
      background(255);
      wasRed = false;
      redStartTime = 0;
      screenIsLocked = false; // need a better way to determine this
    } else if (distance < 20) {
      background(#67ff95);
      wasRed = false;
      redStartTime = 0;
    } else if (distance < 50) {
      background(#fdff67);
      wasRed = false;
      redStartTime = 0;
      screenIsLocked = false;
    } else {
      background(#ff6767);
      if (wasRed) {
        if (m1 - redStartTime > 1900) {
          LockScreen();
        }
      } else {
        redStartTime = m1;
      }
      wasRed = true;
    }
  } else {
    background(128);
  }
  draw_portwidget();
  draw_distance();
}

void mouseDragged() {
  mouseMoved();
}

void mouseMoved() {
  if (mouseX > margin + portLabelW && mouseX < margin + portLabelW + portW &&
  mouseY > portY && mouseY <= portY + portTextSize + margin) {
    mouseIsInPortBox = true;
  } else {
    mouseIsInPortBox = false;
  }
}

void mouseClicked() {
  if (screenIsLocked && mouseIsInPortBox) {
    println("CLICK! resetting lock state");
    screenIsLocked = connected = false;
  } 
}







