var temperature_timer = {
  temp : 0,
  onTimer: function() {
    this.temp++;
    this.notifyPropsChanged();

    return this.temp < 10 ? RET_REPEAT : RET_REMOVE;
  },
  onMount: function() {
    timerAdd(this.onTimer.bind(this), 1000);
  },
  quit: function(args) {
    exit();
  }
}

