
class BlowDetector implements BlowListener {
  public State state;
  public color c = color(100, 100, 100);
  public int r, g, b;
  public float baseValue = 842;

  boolean longBlow = false;
  boolean longSuck = false;

  public BlowDetector () {
    state = State.BASE;
    println(this);
    println(c);
  }

  public void newValue(float value) {
    int margin = 7;
    if (value > baseValue+margin) {
      state = state.highValue(this);
    } else if (value < baseValue+margin && value > baseValue-margin) {
      state = state.baseValue(this);
    } else if (value < baseValue - margin) {
      state =  state.lowValue(this);
    }

    if (longBlow) {
      r++;
      g++;
      b++;
      c = color(r, g, b);
    }
    if (longSuck) {
      r--;
      g--;
      b--;
      c = color(r, g, b);
    }
  }

  public void puff() {
    r=(int)(Math.random()*255);
    g=(int)(Math.random()*255);
    b=(int)(Math.random()*255);
    c = color(r, g, b);
  }
  public void sip() {
    r=(int)(Math.random()*255);
    g=(int)(Math.random()*255);
    b=(int)(Math.random()*255);
    c = color(r, g, b);
  }

  public void longBlowStart() {
    println("long blow start");
    longBlow = true;
  }
  public void longBlowStop() {
    println("long blow stop");
    longBlow = false;
  }
  public void longSuckStart() {
    println("long suck start");
    longSuck = true;
  }
  public void longSuckStop() {
    println("long suck stop");
    longSuck = false;
  }
}

