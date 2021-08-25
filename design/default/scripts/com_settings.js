ViewModel('com_settings', {
  data: {
    type: 0,
    ip: '192.168.1.1',
    port: '8088',
    device: 'COM1',
    baudrate: '9600',
    parity: 0
  },
  computed: {
    summary : {
      get : function () {
        var summary = '';
  
        if(this.type == 0) {
          var parity_name = ["None", "Odd", "Even"];
          summary = 'UART: ' + this.device + ' ' + this.baudrate + ' ' + parity_name[this.parity];
        } else {
          summary = "SOCKET: " + this.ip + ' ' + this.port;
        }
  
        return summary;
      }
    }
  }
});
