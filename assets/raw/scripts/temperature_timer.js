function Temperature () {
  this.value = 0;
  console.log('Temperature');
}

Temperature.prototype.onTimer = function() {
  this.value++;
  this.notifyPropsChanged();

  return this.value < 10 ? RET_REPEAT : RET_REMOVE;
}

Temperature.prototype.onWillMount = function(req) {
  console.log('onWillMount');
}

Temperature.prototype.onMount = function() {
  timerAdd(this.onTimer.bind(this), 1000);
  console.log('onMount');
}

Temperature.prototype.onWillUnmount = function() {
  console.log('onWillUnmount');
}

Temperature.prototype.onUnmount = function() {
  console.log('onUnmount');
}

Temperature.prototype.quit = function() {
  exit();
}

function createTemperatureTimer(req) {
  return new Temperature(); 
}
