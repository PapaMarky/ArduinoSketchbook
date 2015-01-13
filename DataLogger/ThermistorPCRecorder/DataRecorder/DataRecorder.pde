import processing.serial.*;
import java.nio.charset.*;

class SerialDevice {
  PApplet sketch;
  
  public SerialDevice(PApplet s) {
    sketch = s;
  }
  
  public int countAvailableDevices() {
    return port_list.length;
  }
  
  public boolean isConnected() {
    return connected;
  }
  
  public Serial port() { return myPort; }
  
  public void OpenPort(int n) {
    if (n < 0 || n >= port_list.length) {
      println("Bad Port Number: " + n + " (port_list contains " + port_list.length + " entries)");
      return;
    }
    port_n = n;
    OpenPort();
  }
  
  public void OpenPort() {
    ClosePort();
    println("OpenPort " + port_n);
    myPort = new Serial(sketch, port_list[port_n], 9600);
    if (myPort != null) {
      connected = true;
    }
  }
  public void ClosePort() {
    if (myPort != null) {
      myPort.stop();
      myPort = null;
    }
    connected = false;
  }
  
  private String[] port_list;
  private int port_n = 0;
  private Serial myPort;
  private boolean connected;
  private boolean listsMatch(String[] plist1, String[] plist2) {
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
  private void CheckPorts() {
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
          println("old port list empty, new list not empty. Using new list");
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
          println("port list changed");
          for(int i = 0; i < n; i++) {
            if (newPortList[i] == portName) {
              port_n = i;
              found = true;
              println(" -- found old port in new list: " + portName);
              break;
            }
          }
          ClosePort();
          port_list = newPortList;
          OpenPort();
        } else {
          // the new list is empty
          println("port list has become empty. Closing port");
          ClosePort();
          port_list = newPortList;
        }
      }
    }
  }
};

class DataReaderDevice {

  class Data {
    public float value;
    public long time;
    public Data() {
      time = 0;
      value = 0;
    }
  }
  
  SerialDevice sd;
  PrintWriter output;
  
  private long start_t;
  private long last_t;
  
  final int MAX_DATA = 24 * 60 * 60; // 24 hours should be enough
  Data[] data;
  float min_data;
  float max_data;
  int nSamples = 0;

  final private int plot_x = 40;
  final private int plot_y = 40;
  final private int textSize = 30;
  
  final private float hLineSpace = 5.0;
  final private int lineWeight = 4;
  
  private int w = width - 2 * plot_x;
  private int h = height - 2 * plot_y;
  
  String filename = "";
  
  public void setup() {
    data = new Data[MAX_DATA];
    for(int i = 0; i < MAX_DATA; i++) {
      data[i] = new Data();
    }
   filename = year() +
   (month() < 10 ? "0" : "") + month() + 
   (day() < 10 ? "0" : "") + day() +
   "_" + 
   (hour() < 10 ? "0" : "") + hour() +
   (minute() < 10 ? "0" : "") + minute() +
   (second() < 10 ? "0" : "") + second() + ".csv";
    output = createWriter("data/" + filename);
    println("#################################");
    println(filename);
    println("#################################");
    
    last_t = 0;
    start_t = millis();
  }
  
  public DataReaderDevice(PApplet sketch) {
    sd = new SerialDevice(sketch);
    sd.CheckPorts();
  }
  
  boolean addSample(float sample) {
    if (nSamples >= MAX_DATA) {
      return false;
    }
    
    if (nSamples >= 2 && abs(data[nSamples - 1].value - sample) > 40.0) {
      return false;
    }
    
    data[nSamples].value = sample;
    last_t = millis();
    data[nSamples].time = last_t;
    
    if (nSamples == 0) {
      min_data = sample - 10;
      max_data = sample + 10;
      start_t = last_t;
    } else {
      if (sample - 1 < min_data) {
        min_data = sample - 1;
      }
      if (sample + 1 > max_data) {
        max_data = sample + 1;
      }
    }
    nSamples++;
    return true;
  }
  String DateAsString() {
    String s = year() + "/" + 
    (month() < 10 ? "0" : "") + month() + "/" + 
    (day() < 10 ? "0" : "") + day();
    return s;
  }
  String TimeAsString() {
    return (hour() < 10 ? "0" : "") + hour() + ":" + 
    (minute() < 10 ? "0" : "") + minute() + ":" + 
    (second() < 10 ? "0" : "") + second();
  }
  
  public String datestring() {
    String ds = DateAsString() + " " + TimeAsString();
    return ds;
  }
  
  private float fitX(long v) {
    // from zero to nSamples - 1
    return plot_x + ((float)(v - start_t) / (float)(last_t - start_t)) * w;
  }
  
  private float fitY(float v) {
    // from data_min to data_max
    float p = (v - min_data) / (max_data - min_data);
    return height - plot_y - (h * p);
  }
  
  private void drawTitle() {
    // draw title (filename)
    textSize(textSize);
    textAlign(LEFT, TOP);
    fill(0);
    float x = width/2 - textWidth(filename)/2;
    text(filename, x, plot_y);    
  }
  
  private void drawAxis() {
    // draw axis
    stroke(0);
    strokeWeight(lineWeight);
    line(plot_x, plot_y, plot_x, plot_y + h);
    line(plot_x, plot_y + h, plot_x + w, plot_y + h);
  }
  
  private void drawGrid() {
    textSize(textSize);
    textAlign(LEFT, BOTTOM);
    // draw the freezing line if it is inside the plot area
    if (max_data > 0 && min_data < 0) {
      stroke(color(192,0,0));
      float zero = fitY(0.0);
      line(plot_x, zero, plot_x + w, zero);
      text("0", 2*plot_x, zero + lineWeight, 2*plot_x);
    }
    
    stroke(color(192));
    float n = hLineSpace;
    while(n < max_data) {
      if (n > min_data) {
        float nn = fitY(n);
        line(plot_x, nn, plot_x + w, nn);
        text(" " + (int)n, plot_x, nn - lineWeight, 2*plot_x);
      }
      n += hLineSpace;
    }
    
    n = -hLineSpace;
    while(n > min_data) {
      if (n < max_data) {
        float nn = fitY(n);
        line(plot_x, nn, plot_x + w, nn);
        text(" " + (int)n, plot_x, nn - lineWeight, 2*plot_x);
      }
      n -= hLineSpace;
    }
  }
  
  private void drawData() {
    // fit the data to the graph
      
    stroke(color(0,0,192));
    
    int i = 0;
    float x0 = fitX(data[i].time);
    float y0 = fitY(data[i].value);
    i++;
    while(i < nSamples) {
      float x1 = fitX(data[i].time);
      float y1 = fitY(data[i].value);
      line(x0, y0, x1, y1);
      
      x0 = x1; y0 = y1;
      i++;
    }
  }
  //static final private DecimalFormat tempFormat = new DecimalFormat("0.00");
  
  private void drawTemperature() {
    if (nSamples < 1) {
      return;
    }
    String temp = String.format("%.2fC", data[nSamples - 1].value);
    textAlign(RIGHT, TOP);
    fill(128);
    textSize(textSize*6);
    //float tempWidth = textWidth(temp);
    text(temp, width - plot_x, plot_y);
  }
  
  private void plotData() {
  // need two points to draw a graph
    if (nSamples < 2) {
      return;
    }
    
    w = width - 2 * plot_x;
    h = height - 2 * plot_y;    

    // clear 
    background(255);
    
    drawGrid();
    drawTemperature();
    drawData();
    drawAxis();
    drawTitle();    
  }
  
  public void Update() {
    sd.CheckPorts();
    
    if (sd.isConnected()) {
      sd.port().write(0);
      if (sd.port().available() > 0) {
        //int n = sd.port().readBytes(inBuffer);
        String data = sd.port().readString();
        float data_f;
        try {
          data_f = Float.parseFloat(data);
        } catch (NumberFormatException ignore) {
          println("BAD DATA: " + data);
          return;
        }
        // discard bad samples
        if(!addSample(data_f)) {
          println("BAD DATA: " + data_f);
          return;
        }
        String buff = datestring() + "," + data_f;
        println(buff);
        output.println(buff);
        output.flush();
        plotData();
      } else {
        println("NO DATA");
      }
    }
  }
}

DataReaderDevice kb;

void setup() {
  background(255);
  size(1800, 1000);
  
  kb = new DataReaderDevice(this);
  kb.setup();
}

void draw() {
  kb.Update();
  delay(1000);
}



