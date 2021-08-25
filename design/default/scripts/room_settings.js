ViewModel('room_settings', {
  data: {
    room: 'none',
    temp: 0,
    humidity: 0
  },
  methods: {
    return: function(args) {
      this.req.onResult({temp: this.temp, humidity: this.humidity, room: this.room});
    }
  },
  onWillMount: function(req) {
    this.req = req;
    this.room = req.room || 'none';
    this.temp = req.temp || 0;
    this.humidity = req.humidity || 0;
  }
});
