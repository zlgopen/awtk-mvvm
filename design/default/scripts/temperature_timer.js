ViewModel('temperature_timer', {
  data: {
    value: 0,
    timer: 10
  },

  methods: {
    onTimer: function () {
      if (this.value < 100) {
        this.value++;
      }
      this.timer--;
      console.log(this.value);
      this.notifyPropsChanged();
      return this.timer > 0 ? RET_REPEAT : RET_REMOVE;
    }
  },

  onWillMount: function (req) {
    console.log('onWillMount');
  },
  onMount: function () {
    timerAdd(this.onTimer, 1000);
    console.log('onMount');
  },
  onWillUnmount: function () {
    console.log('onWillUnmount');
  },
  onUnmount: function () {
    console.log('onUnmount');
  }
});
