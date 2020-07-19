// Model
function Calculator (req) {
  this.expr = '';
}

Calculator.prototype.addChar= function(args) {
  this.expr += args;
  this.notifyPropsChanged();

  return true;
}

Calculator.prototype.removeChar = function(args) {
  this.expr = this.expr.slice(0, -1);
  this.notifyPropsChanged();

  return true;
}

Calculator.prototype.eval = function(args) {
  this.expr = eval(this.expr).toString();
  this.notifyPropsChanged();

  return true;
}

// Model creator
function createCalculator(req) {
  return new Calculator(req);
}

