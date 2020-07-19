function ComSettings() {
  this.type = 0;

  this.ip = "192.168.1.1"
  this.port = "8088"

  this.device = "COM1"
  this.baudrate = "9600"
  this.parity = 0
}

Object.defineProperty(ComSettings.prototype, 'summary', {
  get: function () {
    var summary = '';

    if(this.type == 0) {
      var parity_name = ["None", "Odd", "Even"];
      summary = 'UART: ' + this.device + ' ' + this.baudrate + ' ' + parity_name[this.parity];
    } else {
      summary = "SOCKET: " + this.ip + ' ' + this.port;
    }

    return summary;
  }
})

function createComSettings(req) {
  return new ComSettings();
}

