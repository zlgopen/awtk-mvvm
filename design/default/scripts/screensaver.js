ViewModel('screensaver', {
  data: {
    time: 10000
  },
  methods: {
    canSetTime: function() {
      return Number(this.time) >= 1000;
    },
    setTime: function() {
      setScreenSaverTime(Number(this.time));
    }
  },
});
