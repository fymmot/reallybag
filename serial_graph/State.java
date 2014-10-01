enum State {
  BASE {
    public State highValue(BlowListener bl) {
      if (count++ > minCount) {
        count = 0;
        //System.out.println("Blowing! "+count);
        stateStartTime = System.currentTimeMillis();  // we start blowing
        return BLOW; // change state to blow
      } else {
        //System.out.println("Blowing! "+count);
        return BASE;
      }
    }
    public State baseValue(BlowListener bl) {
      count = 0;
      return BASE;
    }
    public State lowValue(BlowListener bl) {
      if (count++ > minCount) {
        count = 0;
        //System.out.println("Sucking! "+count);
        stateStartTime = System.currentTimeMillis();  // we start blowing
        return SUCK;
      } else {
        //System.out.println("Sucking! "+count);
        return BASE;
      }
    }
  }
  , 
  BLOW {
    public State highValue(BlowListener bl) {
      stateDuration = System.currentTimeMillis()-stateStartTime;
      if (stateDuration > puffSuckDuration && !inLong) {
        inLong = true;
        bl.longBlowStart();
        System.out.println("Long blow!");
      }
      return BLOW;
    }
    public State baseValue(BlowListener bl) {
      System.out.println("Stopped blowing after "+stateDuration/1000.0+ " seconds!");
      if (stateDuration < puffSuckDuration) {
        System.out.println("Puff!");
        bl.puff();
      } else {
        inLong=false;
        bl.longBlowStop();
      }
      return BASE;
    }
    public State lowValue(BlowListener bl) {
      System.out.println("Sucking after blowing!");

      return SUCK;
    }
  }
  , 
  SUCK {
    public  State highValue(BlowListener bl) {
      System.out.println("Blowing after sucking!");
      return BLOW;
    }
    public State baseValue(BlowListener bl) {
      System.out.println("Stopped sucking after "+stateDuration/1000.0+ " seconds!");
      if (stateDuration < puffSuckDuration) {
        System.out.println("Sip!");
        bl.sip();
      } else {
        inLong=false;
        bl.longSuckStop();
      }
      return BASE;
    }
    public State lowValue(BlowListener bl) {
      stateDuration = System.currentTimeMillis()-stateStartTime;
      if (stateDuration > puffSuckDuration  && !inLong) {
        inLong = true;
        System.out.println("Long suck!");
        bl.longSuckStart();
      }
      return SUCK;
    }
  };

  public abstract State highValue(BlowListener bl);
  public abstract State baseValue( BlowListener bl);
  public abstract State lowValue(BlowListener bl);

  /** Use this count to decide when to switch states
   e.g. blowing has to occur for at least "count"
   steps before moving from
   BASE to BLOW */
  private static int count = 0;
  /** How many steps to count before changing state */
  private static int minCount = 3;

  /** Used for knowing how long we've been in a state */
  private static long stateStartTime, stateDuration;

  /** how long is a puff ?? how long is a suck??? */
  private static int puffSuckDuration = 200; // ms

  private static boolean inLong = false;
};

