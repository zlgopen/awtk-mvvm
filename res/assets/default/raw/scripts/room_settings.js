
function RoomSettings(req) {
  this.req = req;
  this.room = req.room || 'none';
  this.temp = req.temp || 0;
  this.humidity = req.humidity || 0;
}

RoomSettings.prototype["return"] = function(args) {
  this.req.onResult({temp:this.temp, humidity:this.humidity, room:this.room});

  return 0;
}

function createRoomSettings(req) {
  var roomSettings = new RoomSettings(req);

  return roomSettings;
}


