
function Home() {
  this.bed_room = {temp:10, humidity:20}
  this.living_room = {temp:10, humidity:20}
}

Home.prototype.onResult = function(args) {
  if(args.room == 'bed_room') {
    this.bed_room.temp = args.temp;
    this.bed_room.humidity = args.humidity;
  } else {
    this.living_room.temp = args.temp;
    this.living_room.humidity = args.humidity;
  }
  this.notifyPropsChanged();

  return true;
}

Home.prototype.adjustBedRoom = function(args) {
  navigateTo({
    target:'js_room_settings',
    room : 'bed_room',
    temp : this.bed_room.temp,
    humidity : this.bed_room.humidity,
    onResult : Home.prototype.onResult.bind(this)
  }); 

  return true;
}

Home.prototype.adjustLivingRoom = function(args) {
  navigateTo({
    target:'js_room_settings',
    room : 'living_room',
    temp: this.living_room.temp,
    humidity : this.living_room.humidity,
    onResult : Home.prototype.onResult.bind(this)
  }); 

  return true;
}

Home.prototype.quit = function(args) {
  quit();
  return true;
}

Object.defineProperty(Home.prototype, 'bed_room_info', {
  get: function () {
    return 'bedroom: temp=' + this.bed_room.temp + ' humi=' + this.bed_room.humidity; 
  }
})

Object.defineProperty(Home.prototype, 'living_room_info', {
  get: function () {
    return 'livingroom: temp=' + this.living_room.temp + ' humi=' + this.living_room.humidity; 
  }
})

var home = new Home();

