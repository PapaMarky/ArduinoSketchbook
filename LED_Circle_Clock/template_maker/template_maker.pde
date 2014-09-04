int[] angles = { 30, 10, 45, 35, 60, 38, 75, 67 };

static float DPI = 72;

float innerRadius = 5.5 * DPI; // should be 14
float outerRadius = 7 * DPI; // should be 18
float angle = PI/6.0;
float cx = outerRadius;
float cy = 0;

boolean letter = false;

void setupLetter() {
  size((int)(11 * DPI), (int)(8.5 * DPI));
  cy = outerRadius + 2;
  cx = /*outerRadius + 2; // */ width/2.0;
  letter = true;
}

void setupPng() {
  size((int)outerRadius*2 + 2, (int)outerRadius*2 + 2);
  cy = outerRadius + 1;
  cx = outerRadius + 1;
}

void setup() {
  setupLetter();
  //setupPng();
  noLoop();  // Run once and stop
}

void draw() {
  background(255);
  
  drawTemplate(0, letter?50:0);

  if (letter) {
    int off = (int)(1.25 * (outerRadius - innerRadius));
    drawTemplate(0, off+50);
    drawTemplate(0, 2*off+50);
    drawTemplate(0, 3*off+50);
  }
  String filename = (letter ? "circle_template_p.png" : "circle_template.png");
  save(filename);
}

void drawTemplate(int ox, int oy) {
  // Draw outer circle
  float X = ox + cx;
  float Y = oy + cy;
  // draw the lines
  float angle = TWO_PI/12.0;
  float half_angle = angle/2.0;
  float angle2 = angle / 5;
  float tWidth0 = (outerRadius - innerRadius) / 3.0;
  float tWidth1 = (outerRadius - innerRadius) / 4.0;

  fill(255);
  if (letter) {
    arc(X, Y, outerRadius*2, outerRadius*2, -HALF_PI - half_angle, -HALF_PI + half_angle);
    arc(X, Y, innerRadius*2, innerRadius*2, -HALF_PI - half_angle, -HALF_PI + half_angle);
  } else {
    ellipse(X, Y, outerRadius*2, outerRadius*2);
    // Draw inner circle
    ellipse(X, Y, innerRadius*2, innerRadius*2);
  }
  
  for(int i = 0; i <= 12; i++) {
    float a = angle * (float)i;
    if (letter) {
      a = a - half_angle;
    }
    float a1 = a + angle2;
    
    float x0 = X + cos(a) * innerRadius;
    float y0 = Y + sin(a) * innerRadius;
    float x1 = X + cos(a) * (innerRadius + tWidth0);
    float y1 = Y + sin(a) * (innerRadius + tWidth0);
    line(x0, y0, x1, y1);
    
    x0 = x1;
    y0 = y1;
    x1 = X + cos(a1) * (innerRadius + tWidth1);
    y1 = Y + sin(a1) * (innerRadius + tWidth1);
    line(x0, y0, x1, y1);
    
    x0 = x1;
    y0 = y1;
    x1 = X + cos(a1) * (outerRadius - tWidth1);
    y1 = Y + sin(a1) * (outerRadius - tWidth1);
    line(x0, y0, x1, y1);
    
    x0 = x1;
    y0 = y1;
    x1 = X + cos(a) * (outerRadius - tWidth0);
    y1 = Y + sin(a) * (outerRadius - tWidth0);
    line(x0, y0, x1, y1);
    
    x0 = x1;
    y0 = y1;
    x1 = X + cos(a) * (outerRadius);
    y1 = Y + sin(a) * (outerRadius);
    line(x0, y0, x1, y1);
  }
}
