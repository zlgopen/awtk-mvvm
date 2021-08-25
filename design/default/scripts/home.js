ViewModel('home', {
  data: {
    bed_room: {temp:20, humidity:40},
    living_room: {temp:20, humidity:40}
  },
  computed: {
    bed_room_info : {
      get : function () {
        return 'bedroom: temp=' + this.bed_room.temp + ' humi=' + this.bed_room.humidity; 
      }
    },
    living_room_info : {
      get: function () {
        return 'livingroom: temp=' + this.living_room.temp + ' humi=' + this.living_room.humidity; 
      }
    }
  },
  methods: {
    onResult: function(args) {
      if(args.room == 'bed_room') {
        this.bed_room.temp = args.temp;
        this.bed_room.humidity = args.humidity;
      } else {
        this.living_room.temp = args.temp;
        this.living_room.humidity = args.humidity;
      }
      this.notifyPropsChanged();
    },
    adjustBedRoom: function(args) {
      navigateTo({
        target:'room_settings',
        room : 'bed_room',
        temp : this.bed_room.temp,
        humidity : this.bed_room.humidity,
        onResult : this.onResult
      });
    },
    adjustLivingRoom: function(args) {
      navigateTo({
        target:'room_settings',
        room : 'living_room',
        temp: this.living_room.temp,
        humidity : this.living_room.humidity,
        onResult : this.onResult
      });
    },
    quit: function(args) {
      quit();
    }
  }
});
